/*
 *   Copyright (c) International Business Machines Corp., 2007, 2013
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/fsp/reboot.h>
#include <linux/proc_fs.h>
#include <linux/crash_dump.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/reg.h>
#include <asm/dcr.h>

#include <asm/fsp/reg.h>
#include <asm/fsp2/dcr.h>

#include <linux/seq_file.h>

#include <linux/fsp/pra.h>
#include <linux/fsp/reset.h>
#include <linux/fsp/hdcr.h>
#include "fsp.h"

ATOMIC_NOTIFIER_HEAD(pre_kdump_notifier_list);

EXPORT_SYMBOL(pre_kdump_notifier_list);

//extern void flush_dcache_all(void);

u32 mfcmu(u32 reg)
{
	u32 data;
	unsigned long flags;

	local_irq_save(flags);
	mtdcr(DCRN_CMU_ADDR, reg);
	data = mfdcr(DCRN_CMU_DATA);
	local_irq_restore(flags);

	return data;
}

void mtcmu(u32 reg, u32 data)
{
	unsigned long flags;

	local_irq_save(flags);
	mtdcr(DCRN_CMU_ADDR, reg);
	mtdcr(DCRN_CMU_DATA, data);
	local_irq_restore(flags);
}

static void crcs_set(u32 new)
{
	u32 old;
	unsigned long flags;

	local_irq_save(flags);
	old = mfcmu(CMUN_CRCS);
	mtcmu(CMUN_CRCS, old | new);
	local_irq_restore(flags);
}

#ifdef CONFIG_FSP2
irqreturn_t reset_warning_irq(int irq, void *data)
{
	u32 crcs;

	show_regs(get_irq_regs());

	crcs = mfcmu(CMUN_CRCS);

	switch (crcs & CRCS_STAT_MASK) {
		case CRCS_STAT_CHIP_RST_B:
			panic("Received chassis-initiated reset request");
		default:
			panic("Unknown external reset: CRCS=0x%x", crcs);
	}

	/* Should never get here */
	return IRQ_HANDLED;
}
#endif

static void reset(u32 crcs, u32 flavor)
{

	//pra_write(PRA_SFT_RST_FLAVOR, flavor);

	local_irq_disable();

	if (crcs == CRCS_CORE || crcs == CRCS_CHIP)
		//flush_dcache_all();

#ifdef CONFIG_FSP2
	/* Disable critical irqs so we don't get a reset timer interrupt */
	mtmsr(mfmsr() & ~MSR_CE);
#endif

	isync();

	/* Sayonara! */
	crcs_set(crcs);
}
#define chip_reset(flavor) reset(CRCS_CHIP, flavor)
#define system_reset(flavor) reset(CRCS_SYS, flavor)

void fsp_restart(char *cmd)
{
	if (cmd == NULL)
		chip_reset(LINUX_REBOOT_CMD_RESTART);
	else if (!strcmp(cmd, LINUX_REBOOT_CMD_TOOLRESET_STR))
		system_reset(LINUX_REBOOT_CMD_TOOLRESET);
	else if (!strncmp(cmd, LINUX_REBOOT_CMD_COREDUMP_STR,
			  strlen(LINUX_REBOOT_CMD_COREDUMP_STR))) {
		unsigned int type, data_a, data_b, data_c;
		sscanf(cmd, LINUX_REBOOT_CMD_COREDUMP_STR " %x %x %x %x",
		       &type, &data_a, &data_b, &data_c);
	//	pra_write(PRA_PABEND, type);
	//	pra_write(PRA_PABEND_A, data_a);
	//	pra_write(PRA_PABEND_B, data_b);
	//	pra_write(PRA_PABEND_C, data_c);
		panic("User-initiated panic");
	}
	else
		panic("Bad restart cmd");
}

void fsp_panic(char *str)
{
	chip_reset(LINUX_REBOOT_CMD_RESTART2);
}

void fsp_power_off(void)
{
	chip_reset(LINUX_REBOOT_CMD_POWER_OFF);
}

void fsp_halt(void)
{
	chip_reset(LINUX_REBOOT_CMD_HALT);
}

/*
 * The following code implements an interface to the RESET hardware of
 * the FSP-1. In the /proc/reset directory the abstract type of
 * reset can be read out.
 *
 * Please note that the bits in the TSR and in the DBSR are cleared
 * after initializing this driver. Since those bits would survive
 * a core reset this is needed to make this software function
 * well.
 *
 * For FSP-1 DD1.0 and DD2.0 the reset related registers like DBSR and
 * CRCS are totally unrelibable.
 *
 * Author: Frank Haverkamp <haverkam@de.ibm.com>
 */

#define PERROR(fmt, args...) printk(KERN_ERR "FSP RESET: "fmt, ## args)

#define TSR_WRS_MASK(x)		(((x)>>28)&0x3)	/* WDT Reset Status Mask */
#define TSR_WRS_MASK_VALUE	0x30000000	/* last reset */

#define PROCDIR_RESET		"reset"		/* name of proc dir */
#define PROCDIR_CURRENT		"current"	/* file current reason */
#define PROCDIR_ORIGINAL        "original"      /* file original reason */
#define PROCDIR_CALLOUT         "callout"       /* file callout */

#define PROCDIR_CURR		"curr"		/* subdir current status */
#define PROCDIR_SIDE		"side"		/* subdir side status */

#define PROCDIR_SPECIFIC	"specific"	/* debug */
#define PROCDIR_WATCHDOG	"bootwatchdog"	/* bootwd status */

#define PROCDIR_DISR		"disr"		/* disr read/write access */

#define PROCDIR_DRCR		"drcr"		/* drcr read/write access */

#define PROCDIR_PANIC		"panic_reason"  /* reason for the panic */

#define WATCHDOG_BANK_NORMAL	"Default"
#define WATCHDOG_BANK_SWAPPED	"Alternate"

/**
 * We need those string tables to print out the correct reset originator,
 * type and reason.
 */

static const char *rst_origin_str[] = {
	 "Unknown", "Core", "Asic", "Power", "External",
};

static const char *rst_type_str[] = {
	 "Unknown", "Core", "Chip", "System",
};

static const char *rst_reason_str[] = {
	"Unknown",
	"CoreWatchdog",
	"Restart",
	"Panic",
	"Halt",
	"PowerOff",
	"PowerCycle",
	"Pinhole",
	"Parport",
	"Bootcode",
	"HostInitiated",
	"BootWatchdog",
	"UnitCheckTimer",
	"ExternalTool",
	"PAbend",
	"ChassisInitiated",
	"PCIe",
};

static const char *callout_str[] = {
	"SoftwareError",
	"HardwareError",
	"HardwareSoftwareError",
	"EccError",
	"TLBCacheError",
	"WPAParityError",
	"OutOfMemory",
	"FlashError",
	"CMUError",
	"ConfigLogicParityError",
};

/**
 * data structure to store the internal data
 */
typedef struct rstinfo_t {
	struct proc_dir_entry *procdir_reset; /* ptr to proc entry */
	struct proc_dir_entry *procdir_curr;
	struct proc_dir_entry *procdir_currside;
	unsigned long pvr;

	uint32_t cached_crcs;
	uint32_t cached_dbsr;
	uint32_t cached_tsr;
	uint32_t cached_flavor;
	uint32_t cached_bwtc;
	uint32_t cached_pabend;
	uint32_t cached_pabend_a;
	uint32_t cached_pabend_b;
	uint32_t cached_pabend_c;

	rst_origin_t origin;
	rst_type_t type;
	rst_reason_t reason;
} rstinfo_t;

/**
 * global data to store the module status information.
 */
static rstinfo_t rstinfo;

/**
 * This function reads out and masks the reason for the reset of the
 * system.
 */
static int readproc_specific(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char * buf;
	int len = 0;
	int end = 0;
	int transfer = 0;
	int offset = (int)*ppos;
	rstinfo_t *info = PDE_DATA(file_inode(file));

	buf = (char *)kmalloc(1024, GFP_KERNEL);

	/* specific */
	len += sprintf(buf+len, "DBSR:\t\t0x%08x\n", info->cached_dbsr);
	len += sprintf(buf+len, "CRCS:\t\t0x%08x\n", info->cached_crcs);
	len += sprintf(buf+len, "TSR:\t\t0x%08x\n", info->cached_tsr);
	len += sprintf(buf+len, "BWTC:\t\t0x%08x\n", info->cached_bwtc);
	len += sprintf(buf+len, "RST_FLAVOR:\t0x%08x\n", info->cached_flavor);
	len += sprintf(buf+len, "PABEND:\t\t0x%08x\n", info->cached_pabend);
	len += sprintf(buf+len, "PABEND_A:\t0x%08x\n", info->cached_pabend_a);
	len += sprintf(buf+len, "PABEND_B:\t0x%08x\n", info->cached_pabend_b);
	len += sprintf(buf+len, "PABEND_C:\t0x%08x\n", info->cached_pabend_c);
	len += sprintf(buf+len, "PVR:\t\t0x%08lx\n", info->pvr);
	len += sprintf(buf+len,
		       "Debugging information only! May change "
		       "in the future and is likely not correct\n"
		       "in the present.  Do not use in your software!\n");

	if (offset >= len)
		goto exit;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

exit:
	kfree(buf);
	return transfer;
}


static const char *timeout_str[] = {
#ifdef CONFIG_FSP1
	"500us", "1 ms", "250 ms", "500ms", "1sec", "8sec", "16sec",
	"32sec (default)"
#else
	"100ms", "200ms", "400ms", "800ms", "3sec", "13sec", "26sec",
	"52sec"
#endif
};

/**
 * Get the status of the boot watchdog.
 */
static int readproc_bwd(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char * buf;
	int len = 0;
	int enabled = 0;
	int timeout = 0;
	int end = 0;
	int transfer = 0;
	unsigned int bank = 0;
	int offset = (int)*ppos;
	rstinfo_t *info = PDE_DATA(file_inode(file));
	unsigned int bwtc = info->cached_bwtc;

	buf = (char *)kmalloc(1024, GFP_KERNEL);

	enabled = ((bwtc & BWTC_WDTE) != 0);
	timeout = (bwtc >> 8) & 0x07;

#ifdef CONFIG_FSP1
	bank = (bwtc & BWTC_BANK_MSB) >> 2;
#endif
	bank |= (bwtc & BWTC_BANK) >> 1;

	len += sprintf(buf+len, "Status:\t%s\n", enabled?"Enabled":"Disabled");
	len += sprintf(buf+len, "Time:\t%s\n", timeout_str[timeout]);
	len += sprintf(buf+len, "Bank:\t%u\n", bank);

	if (offset >= len)
		goto exit;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

exit:
	kfree(buf);
	return transfer;
}


/**
 * Get the panic reason.  This guides behavior in the KDUMP environment.
 */
static int readproc_panic(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char buf[32];
	int len = 0;
	int end = 0;
	int transfer = 0;
	u32 flavor, type;
	const char* reason = "UNKNOWN";
	int offset = (int)*ppos;
	
	/* NOTE: This approach works because PRA keys are set in the production
	 * kernel and can't be modified in the KDUMP environment. */
	/* if(!pra_read(PRA_SFT_RST_FLAVOR, &flavor)) {
		if(flavor == LINUX_REBOOT_CMD_HOSTINIT)
			reason = "HostInitiated";
		else if(flavor == LINUX_REBOOT_CMD_RESTART2) {
			if(!pra_read(PRA_PABEND, &type) && type) {
				reason = "PAbend";
			} else {
				reason = "Panic";
			}
		}
	}
 */
	len = sprintf(buf, "REASON:\t%s\n", reason);

	if (offset >= len)
		return 0;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

	return transfer;
}

static int readproc_disr(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char buf[32];
	u32 read_val;
	int len = 0;
	int end = 0;
	int transfer = 0;
	int offset = (int)*ppos;

	read_val = disr_read();
	len += sprintf(buf+len, "%08x\n", read_val);

	if (offset >= len)
		return 0;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

	return transfer;
}

static int writeproc_disr(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	int len = 0;
	u32 bits = 0;
	char buffer[10]; /* max size is a single 32 bit hex value with '\0',
			 no 0x prefix */
	char lbuf[9];

	if (!count)
	return 0;

	if (count > 9) { /* 8 + '\0' */
		PERROR("PROC DISR: invalid command\n");
		return -EFAULT;
	}

	len = (count > sizeof(buffer) - 1)?(sizeof(buffer) - 1):(count);
	if (copy_from_user((void*)buffer, (void*)buf, len)) {
		PERROR("Could not copy user buffer!\n");
		return -EFAULT;
	}

	strncpy(lbuf, buffer, 8);
	lbuf[8]='\0';
	bits = simple_strtoul(lbuf, NULL, 16);

	if (bits & DISR_RSVD) {
		PERROR("PROC DISR: attempting write to reserved area: %08x\n",
		       bits);
		return -EFAULT;
	}

	disr_clear_and_set(~bits & ~DISR_RSVD, bits, NULL);

	return len;
}


static int readproc_drcr(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char buf[32];
	u32 read_val;
	int len = 0;
	int end = 0;
	int transfer = 0;
	int offset = (int)*ppos;

	read_val = drcr_read();
	len += sprintf(buf+len, "%08x\n", read_val);

	if (offset >= len)
		return 0;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

	return transfer;
}

static int writeproc_drcr(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	int len = 0;
	u32 bits = 0;
	char buffer[10]; /* max size is a single 32 bit hex value with '\0',
			 no 0x prefix */
	char lbuf[9];

	if (!count)
	return 0;

	if (count > 9) { /* 8 + '\0' */
		PERROR("PROC DRCR: invalid command\n");
		return -EFAULT;
	}

	len = (count > sizeof(buffer) - 1)?(sizeof(buffer) - 1):(count);
	if (copy_from_user((void*)buffer, (void*)buf, len)) {
		PERROR("Could not copy user buffer!\n");
		return -EFAULT;
	}

	strncpy(lbuf, buffer, 8);
	lbuf[8]='\0';
	bits = simple_strtoul(lbuf, NULL, 16);

	/* printk("Clearing DRCR bits: 0x%x", bits); */
	drcr_clear(bits);

	return len;
}

static int readproc_rstinfo(char *buf, int count, rst_origin_t origin,
					rst_type_t type, rst_reason_t reason)
{
	int len = 0;

	len += sprintf(buf+len, "Code:\t%02d %02d %02d\n",
		       origin, type, reason);
	len += sprintf(buf+len, "Origin:\t%s\n", rst_origin_str[origin]);
	len += sprintf(buf+len, "Type:\t%s\n",	 rst_type_str[type]);
	len += sprintf(buf+len, "Reason:\t%s\n", rst_reason_str[reason]);
	return len;
}

static int readproc_current(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char * buf;
	int len;
	int end = 0;
	int transfer = 0;
	int offset = (int)*ppos;
	rstinfo_t *info =  PDE_DATA(file_inode(file));

	buf = (char *)kmalloc(1024, GFP_KERNEL);

	len = readproc_rstinfo(buf, count,
				info->origin,
				info->type,
				info->reason);

	if (offset >= len)
		goto exit;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

exit:
	kfree(buf);
	return transfer;
}

static int readproc_original(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char * buf;
	int len = 0;
	int end = 0;
	int transfer = 0;
	rst_reason_t reason = RST_REASON_UNKNOWN;
	int offset = (int)*ppos;

	buf = (char *)kmalloc(1024, GFP_KERNEL);

//	pra_read(PRA_ORIG_RESET, &reason);

	if(reason * 4 >= sizeof(rst_reason_str))
		reason = RST_REASON_UNKNOWN;

	len += sprintf(buf+len, "Code:\t%02d\n", reason);
	len += sprintf(buf+len, "Reason:\t%s\n", rst_reason_str[reason]);

	if (offset >= len)
		goto exit;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

exit:
	kfree(buf);
	return transfer;
}

static int readproc_callout(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	char * buf;
	int len = 0;
	int end = 0;
	int transfer = 0;
	uint32_t callout = PRA_SWERR;
	int offset = (int)*ppos;

	buf = (char *)kmalloc(1024, GFP_KERNEL);

	//pra_read(PRA_CALLOUT, &callout);
	len += sprintf(buf+len, "Code:\t\t\t0x%08x\n", callout);
	len += sprintf(buf+len, "Summary:\t\t%s\n", callout_str[callout & PRA_MASK_SUMMARY]);
	len += sprintf(buf+len, "Icache Parity Error:\t%s\n",
		       (callout & PRA_CALLOUT_ICACHE) ? "True" : "False");
	len += sprintf(buf+len, "Dcache Parity Error:\t%s\n",
		       (callout & PRA_CALLOUT_DCACHE) ? "True" : "False");
	len += sprintf(buf+len, "TLB Parity Error:\t%s\n",
		       (callout & PRA_CALLOUT_TLB) ? "True" : "False");
	len += sprintf(buf+len, "GPR Parity Error:\t%s\n",
		       (callout & PRA_CALLOUT_GPR) ? "True" : "False");
	len += sprintf(buf+len, "FPR Parity Error:\t%s\n",
		       (callout & PRA_CALLOUT_FPR) ? "True" : "False");
	len += sprintf(buf+len, "L1/L2 Parity Error:\t%s\n",
		       (callout & PRA_CALLOUT_L1_L2) ? "True" : "False");

	if (offset >= len)
		goto exit;

	end = len - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);

exit:
	kfree(buf);
	return transfer;
}

static int readproc_generic(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	const char *what = PDE_DATA(file_inode(file));
	int ret;
	uint32_t pra_bootstatus;
	unsigned long bwtc;
	unsigned int bank;
	char * buf;
	int end = 0;
	int transfer = 0;
	int offset = (int)*ppos;

	buf = (char *)kmalloc(1024, GFP_KERNEL);

	switch (what[1]) {
	case 'o': /* origin */
		ret = sprintf(buf, "%s\n", rst_origin_str[rstinfo.origin]);
		goto exit_success;
	case 't': /* type */
		ret = sprintf(buf, "%s\n", rst_type_str[rstinfo.type]);
		goto exit_success;
	case 'r': /* reason */
		ret = sprintf(buf, "%s\n", rst_reason_str[rstinfo.reason]);
		goto exit_success;
	case 'i': /* ipl */
		bwtc = rstinfo.cached_bwtc;
#ifdef CONFIG_FSP1
		if (bwtc & BWTC_CONFIG1) {
			printk(KERN_WARNING "FSP RESET: BWTC_CONFIG1 set, "
			       "ipl state unreliable\n");
		}
		bank = (bwtc & BWTC_BANK_LSB ? 1 : 0);
		if (bwtc & BWTC_NANDBOOT) {
			bank |= (bwtc & BWTC_BANK_MSB ? 2 : 0);
		}
#else
		bank = bwtc & BWTC_BANK ? 1 : 0;
#endif
		ret = sprintf(buf, "%u\n", bank);
		goto exit_success;
	default: ;
	}

	//ret = pra_read(PRA_BOOT_STATUS, &pra_bootstatus);
	if (ret < 0) {
		printk("FSP RESET: cannot read PRA\n");
		goto exit;
	}

	switch (what[1]) {
	case 'p': /* progress */
		ret = sprintf(buf, "%u\n", (pra_bootstatus >> 8) & 0xff);
		goto exit_success;
	case 's': /* spl */
		ret = sprintf(buf, "%u\n", pra_bootstatus & 0x01);
		goto exit_success;
	case 'l': /* os (linux) */
		ret = sprintf(buf, "%u\n", (pra_bootstatus >> 1) & 0x01);
		goto exit_success;
	default:
		BUG();
		ret = -EINVAL;
		goto exit;
	}

exit_success:
	if (offset >= ret) {
		ret = 0;
		goto exit;
	}

	end = ret - offset;
	transfer = (count < end) ? count : end;

	copy_to_user(ubuf, &buf[offset], transfer);
	*ppos = (loff_t)(offset + transfer);
	ret = transfer;

exit:
	kfree(buf);
	return ret;
}

static int writeproc_progress(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	uint32_t pra_bootstatus;
	char buffer[10]; /* enough for values 0..255, even octal */
	int len = 0, ret;
	unsigned int val = 0;

	if (!count) {
		printk(KERN_INFO "FSP RESET: empty write to progress\n");
		return 0;
	}

	len = (count > sizeof(buffer) -1) ? (sizeof(buffer) -1) : count;

	if (copy_from_user(buffer, buf, len)) {
		printk(KERN_ERR "FSP RESET: Could not copy data from user\n");
		return -EFAULT;
	}

	buffer[len] = '\0';
	if (buffer[len - 1] == '\n')
		buffer[len - 1] = '\0';

	val = simple_strtoul(buffer, NULL, 0);
	if (val > 255) {
		printk(KERN_INFO "FSP RESET: value %u too big for progress\n",
			val);
		return -E2BIG;
	}

	/* read/change/write the PRA field */
	//ret = pra_read(PRA_BOOT_STATUS, &pra_bootstatus);
	if (ret < 0) {
		printk(KERN_ERR "FSP RESET: cannot read PRA\n");
		return ret;
	}

	pra_bootstatus = (pra_bootstatus & 0xffff00ff) | ((val & 0xff) << 8);

	//ret = pra_write(PRA_BOOT_STATUS, pra_bootstatus);
	if (ret < 0) {
		printk(KERN_ERR "FSP RESET: cannot update PRA\n");
		return ret;
	}

	return len;
}

/******************************************************************************
 *
 * Loading and Unloading
 *
 *****************************************************************************/

typedef struct procentry_t {
	char *name;			/* name of the entry */
	struct file_operations *fo;		/* read proc entry function */
	void *data;			/* value for proc.data field.
					   NULL means to put rstinfo there */
} procentry_t;

static const struct file_operations current_fo = {
.owner = THIS_MODULE,
.read  = readproc_current,
};

static const struct file_operations original_fo = {
.owner = THIS_MODULE,
.read  = readproc_original,
};

static const struct file_operations callout_fo = {
.owner = THIS_MODULE,
.read  = readproc_callout,
};

static const struct file_operations specific_fo = {
.owner = THIS_MODULE,
.read  = readproc_specific,
};

static const struct file_operations watchdog_fo = {
.owner = THIS_MODULE,
.read  = readproc_bwd,
};

static const struct file_operations disr_fo = {
.owner = THIS_MODULE,
.read  = readproc_disr,
.write  = writeproc_disr,
};

static const struct file_operations drcr_fo = {
.owner = THIS_MODULE,
.read  = readproc_drcr,
.write  = writeproc_drcr,
};

static const struct file_operations panic_fo = {
.owner = THIS_MODULE,
.read  = readproc_panic,
};


static const struct file_operations generic_fo = {
.owner = THIS_MODULE,
.read  = readproc_generic,
.write  = writeproc_progress,
};


static procentry_t procdir_reset_table[] = {
	{ name: PROCDIR_CURRENT,   fo: &current_fo  },
	{ name: PROCDIR_ORIGINAL,  fo: &original_fo },
	{ name: PROCDIR_CALLOUT,   fo: &callout_fo },
	{ name: PROCDIR_SPECIFIC,  fo: &specific_fo },
	{ name: PROCDIR_WATCHDOG,  fo: &watchdog_fo },
	{ name: PROCDIR_DISR,      fo: &disr_fo },
	{ name: PROCDIR_DRCR,      fo: &drcr_fo },
	{ name: PROCDIR_PANIC,     fo: &panic_fo },
	{ NULL, NULL, NULL, NULL },
};

/* data contains info what to print: cp == curr/progress */
static procentry_t procdir_curr_table[] = {
	{ name: "progress", fo: &generic_fo, data: "cp" },
	{ name: "origin",   fo: &generic_fo, data: "co" },
	{ name: "type",	    fo: &generic_fo, data: "ct" },
	{ name: "reason",   fo: &generic_fo, data: "cr" },
	{ NULL, NULL, NULL, NULL },
};

static procentry_t procdir_currside_table[] = {
	{ name: "ipl", fo: &generic_fo, data: "ci" },
	{ name: "spl", fo: &generic_fo, data: "cs" },
	{ name: "os",  fo: &generic_fo, data: "cl" },
	{ NULL, NULL, NULL, NULL },
};

/**
 * Thanks to Shaun providing this little example how do deal with proc
 * entries very nicely.
 */
static struct proc_dir_entry *
shauns_create_proc_entry(const char * name, mode_t mode,
			 struct proc_dir_entry * base,
			 struct file_operations *fo,
			 void * data)
{
	struct proc_dir_entry *tmp;

	if (!mode)
		mode = 0644;
	tmp = proc_create_data(name, mode, base, fo, data);

	if (tmp == NULL) {
		PERROR("/proc entry '%s' write_proc[%x] "
		       "read_proc[%x] data[%x]\n", name,
		       (int)fo->write, (int)fo->read, (int)data);
	}
	return tmp;
}


/**
 * Create a new subdirectory above rootdir. The initialized NULL terminated
 * table of entries must be passed to this function and the name the new
 * directory should have.
 */
static struct proc_dir_entry * __init
new_procdir(void *info, struct proc_dir_entry *rootdir,
	    procentry_t *table, const char *name)
{
	procentry_t *ptr = NULL;
	struct proc_dir_entry *procdir = NULL;

	/* create directory */
	procdir = proc_mkdir(name, rootdir);
	if (procdir == NULL) {
		PERROR("Cannot create .../%s\n", name);
		return NULL;
	}

	/* create entries */
	for (ptr = table; ptr->name != NULL; ptr++) {

		/* no this will be an entry */
		if (!shauns_create_proc_entry(ptr->name, 0, procdir,
					      ptr->fo,
					      ptr->data ? ptr->data : info)) {
			PERROR("Cannot create .../%s\n", ptr->name);
			return NULL;
		}
	}
	return procdir;
}

#if defined (CONFIG_FSP1)
static int find_reason_sw(rstinfo_t *info)
{
	switch (info->cached_flavor) {
		case LINUX_REBOOT_CMD_RESTART2:
			if (info->cached_pabend)
				return RST_REASON_PABEND;
			else
				return RST_REASON_PANIC;
		case LINUX_REBOOT_CMD_HALT:
			return RST_REASON_HALT;
		case LINUX_REBOOT_CMD_BOOTCODE:
			return RST_REASON_BOOTCODE;
		case LINUX_REBOOT_CMD_POWER_OFF:
			return RST_REASON_POWER_OFF;
		case LINUX_REBOOT_CMD_RESTART:
			return RST_REASON_RESTART;
		case LINUX_REBOOT_CMD_HOSTINIT:
			return RST_REASON_HOST_INITIATED;
		default:
			return RST_REASON_UNKNOWN;
	}
}

static void find_reason_fsp1(rstinfo_t *info)
{
	uint32_t crcs_masked = info->cached_crcs & CRCS_STAT_MASK;
	uint32_t dbsr_masked = info->cached_dbsr & DBSR_MRR;

	switch (dbsr_masked) {
		case DBSR_MRR_SYSTEM: info->type = RST_TYPE_SYSTEM; break;
		case DBSR_MRR_CHIP:   info->type = RST_TYPE_CHIP;   break;
		case DBSR_MRR_CORE:   info->type = RST_TYPE_CORE;   break;
	}

	/* Boot watchdog */
	if (info->cached_crcs & CRCS_WATCHE) {
		info->reason = RST_REASON_BOOT_WATCHDOG;
		info->origin = RST_ORIG_ASIC;
		return;
	}

	/* Core watchdog */
	switch (TSR_WRS_MASK(info->cached_tsr)) {
		case WRS_CORE:
		case WRS_CHIP:
		case WRS_SYSTEM:
			info->reason = RST_REASON_CORE_WATCHDOG;
			info->origin = RST_ORIG_ASIC;
			return;
	}

	/* All other cases */
	switch (crcs_masked) {
		case CRCS_STAT_POR:
			info->reason = RST_REASON_POWER_CYCLE;
			info->origin = RST_ORIG_EXTERNAL;
			return;
		case CRCS_STAT_PHR:
			info->reason = RST_REASON_PINHOLE;
			info->origin = RST_ORIG_EXTERNAL;
			return;
		case CRCS_STAT_HOST_CORE:
		case CRCS_STAT_HOST_CHIP:
		case CRCS_STAT_HOST_SYS:
		case CRCS_STAT_PSI_CHIP:
			info->reason = RST_REASON_HOST_INITIATED;
			info->origin = RST_ORIG_EXTERNAL;
			return;
		case CRCS_STAT_CRCS_SYS:
			info->reason = RST_REASON_EXT_TOOL;
			info->origin = RST_ORIG_CORE;
			return;
		case CRCS_STAT_CRCS_CHIP:
			info->reason = find_reason_sw(info);
			info->origin = RST_ORIG_CORE;
			return;
		default:
			info->reason = RST_REASON_UNKNOWN;
			info->origin = RST_ORIG_UNKNOWN;
			return;
	}
}

#elif defined(CONFIG_FSP2)

static void find_reason_fsp2(rstinfo_t *info)
{
	uint32_t tmp;

	//pra_read(PRA_ORIG_RSTINFO, &tmp);
	info->reason = tmp >> 16;
	info->origin = (tmp >> 8) & 0xFF;
	info->type = tmp & 0xFF;
}
#endif /* CONFIG_FSP1 */

/* Determine reset reason from hardware (fsp1) or pra (fsp2) */
static void find_reason(rstinfo_t *info)
{
#if defined(CONFIG_FSP1)
	find_reason_fsp1(info);
#elif defined(CONFIG_FSP2)
	find_reason_fsp2(info);
#else
#error Only fsp1 and fsp2 are supported
#endif
}


static int __init fsp_reset_init(void)
{
	memset(&rstinfo, 0, sizeof(rstinfo_t));
	rstinfo.pvr = mfspr(SPRN_PVR);
	rstinfo.cached_crcs = mfcmu(CMUN_CRCS);
	rstinfo.cached_dbsr = mfspr(SPRN_DBSR);
	rstinfo.cached_tsr = mfspr(SPRN_TSR);
	rstinfo.cached_bwtc = mfcmu(CMUN_BWTC);

	//pra_read(PRA_SFT_RST_FLAVOR, &rstinfo.cached_flavor);
	//pra_read(PRA_PABEND, &rstinfo.cached_pabend);
	//pra_read(PRA_PABEND_A, &rstinfo.cached_pabend_a);
	//pra_read(PRA_PABEND_B, &rstinfo.cached_pabend_b);
	//pra_read(PRA_PABEND_C, &rstinfo.cached_pabend_c);

	rstinfo.type = RST_TYPE_UNKNOWN;
	rstinfo.origin = RST_ORIG_UNKNOWN;
	rstinfo.reason = RST_REASON_UNKNOWN;

	/* /proc/reset */
	rstinfo.procdir_reset = new_procdir(&rstinfo, NULL,
			procdir_reset_table, PROCDIR_RESET);
	if (rstinfo.procdir_reset == 0)
		goto errout;

	/* /proc/reset/curr */
	rstinfo.procdir_curr = new_procdir(&rstinfo, rstinfo.procdir_reset,
			procdir_curr_table, PROCDIR_CURR);
	if (rstinfo.procdir_curr == 0)
		goto err_curr;

	/* /proc/reset/curr/side */
	rstinfo.procdir_currside = new_procdir(&rstinfo, rstinfo.procdir_curr,
			procdir_currside_table, PROCDIR_SIDE);
	if (rstinfo.procdir_currside == 0)
		goto err_currside;

	/* determine reset reason */
	find_reason(&rstinfo);

	/**
	 * We need to clear some registers after we have done the
	 * detection. The results are cached.  Writing a 1 clears the
	 * bits.
	 */
	mtspr(SPRN_DBSR, (rstinfo.cached_dbsr & DBSR_MRR));
	mtspr(SPRN_TSR, (rstinfo.cached_tsr & TSR_WRS_MASK_VALUE));

	/*Do not clear PRA keys in a dumped kernel. */
	if(!(is_kdump_kernel()) )
	{
		/*
	 	 * Clear out the reset flavor so that any future spurious resets will
	 	 * be correctly interpreted as type "unknown".
	 	 */
	//	pra_write(PRA_SFT_RST_FLAVOR,  0);

		/* Clear out the callout information.  Otherwise the next panic could
	 	 * have unrelated callout information associated with it.
	 	 */
	//	pra_write(PRA_CALLOUT, 0);

		/* reset values to be able to detect panic correctly */
	//	pra_write(PRA_PABEND,   0x00000000);
	//	pra_write(PRA_PABEND_A, 0x00000000);
	//	pra_write(PRA_PABEND_B, 0x00000000);
	//	pra_write(PRA_PABEND_C, 0x00000000);
	}

#ifdef CONFIG_FSP2
{
	u32 crcs;

	/* Enable CHIP_RST_B (Chassis initiated reset) */
	crcs = mfcmu(CMUN_CRCS);
	crcs |= CRCS_EXTCR;
	mtcmu(CMUN_CRCS, crcs);
}
#endif

	return 0;

err_currside:
	remove_proc_entry(PROCDIR_CURR, rstinfo.procdir_reset);
err_curr:
	remove_proc_entry(PROCDIR_RESET, NULL);
errout:
	return -EIO;
}
device_initcall(fsp_reset_init);
