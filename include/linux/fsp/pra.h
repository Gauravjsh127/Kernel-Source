/*
 * Copyright (c) International Business Machines Corp., 2006, 2011
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef _LINUX_FSP_PRA_H_
#define _LINUX_FSP_PRA_H_

#include <linux/types.h>

/*
 * NOTE: These values are also defined in FSP boot code.  If you change or add
 * any values, be sure to make corresponding changes to the boot code PRA
 * defines!
 *
 * For more information, see the FSP architecture and FSP kernel design
 * documents.
 */
#define PRA_CHECKSUM			0
#define PRA_CALLOUT			1
#define PRA_BCU_STATE			2
#define PRA_RR_COUNT			3
#define PRA_SFT_RST_FLAVOR		4
#define PRA_CURR_CHKPT			5
#define PRA_MEM_INIT			6
#define PRA_MEM_INIT_STATIC		7
#define PRA_USER0			8
#define PRA_USER1			9
#define PRA_USER2			10
#define PRA_USER3			11
#define PRA_USER4			12
#define PRA_USER5			13
#define PRA_USER6			14
#define PRA_USER7			15
#define PRA_BOOT_FLAGS			16
#define PRA_ANCHOR_SND			17
#define PRA_BOOTSCRIPT			18
#define PRA_BOOT_STATUS			19
#define PRA_FFDC_LR			20
#define PRA_BOOT_DUMP			21
#define PRA_PABEND			22
#define PRA_PABEND_A			23
#define PRA_PABEND_B			24
#define PRA_PABEND_C			25
#define PRA_ORIG_RESET			26
#define PRA_NETBOOT_DHCP_ERR		27
#define PRA_NETBOOT_TFTP_ERR		28
#define PRA_BOOT_ERR_VALUE		29
#define PRA_BOOT_ERR_FLAGS		30
#define PRA_BOOT_THRESHOLD_TIME		31
#define PRA_BOOT_THRESHOLD_COUNT	32
#define PRA_ANCHOR_FFS			33
#define PRA_ANCHOR_ALT_FFS		34
#define PRA_ORIG_RSTINFO		35
#define PRA_BOOT_RR_COUNT		36
#define PRA_NUMENTRIES			37 /* Must always be the largest key */

/*
 * PRA_CALLOUT
 *
 * 0000.0000.0011.1111.1111.2222.2222.2233
 * 0123.4567.8901.2345.6789.0123.4567.8901
 *			 TT TTTT XXXX XXXX
 *			 || |||| callout summary: SW,HW,ECC error ...
 *			 || |||icache parity error
 *			 || ||dcache parity error
 *			 || |tlb parity error
 *                       || gpr parity error
 *                       |fpr parity error
 *                       L1/L2 error
 */

#define PRA_SWERR		0 /* SW error */
#define PRA_HWERR		1 /* HW error */
#define PRA_HWSWERR		2 /* HW or SW error */
#define PRA_ECCERR		3 /* Uncorrectable ECC error */
#define PRA_TLBCACHEERR		4 /* TLB cache parity error */
#define PRA_WPAPARERR		5 /* wpa parity error */
#define PRA_OOM			6 /* out of memory */
#define PRA_FLASHERR		7 /* flash data integrity problems Nov2008 */
#define PRA_CMU			8 /* CMU error */
#define PRA_CONF_LOGIC		9 /* Configuration Logic parity error */
#define PRA_MASK_SUMMARY	0xff /* mask for callout summary */

/* The following bits are or'ed into PRA_CALLOUT, if the err occurs */
#define PRA_CALLOUT_ICACHE	0x00000100 /* icache parity error */
#define PRA_CALLOUT_DCACHE	0x00000200 /* dcache parity error */
#define PRA_CALLOUT_TLB		0x00000400 /* tlb parity error */
#define PRA_CALLOUT_GPR		0x00000800 /* GPR parity error */
#define PRA_CALLOUT_FPR		0x00001000 /* FPR parity error */
#define PRA_CALLOUT_L1_L2	0x00002000 /* L1/L2 error */

int pra_read(unsigned int key, u32 *data);
int pra_read_previous(unsigned int key, u32 *data);
int pra_write(unsigned int key, u32 data);
void pra_crash_save(void);

#endif /* _LINUX_FSP_PRA_H_ */
