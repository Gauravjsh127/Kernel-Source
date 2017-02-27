/*
 * Copyright (c) International Business Machines Corp., 2006, 2012
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <linux/crash_dump.h>
#include <linux/sched.h>
#include <asm/reg.h>
#include <asm/dcr.h>
#include <asm/io.h>

#include <linux/fsp/pra.h>
#include <asm/fsp/reg.h>
#include "fsp.h"


#include <linux/of_irq.h>
#include <linux/of_address.h>

#include <asm/fsp/reg.h>
#include <asm/fsp2/dcr.h>

#include <linux/fsp/reboot.h>
#include <linux/fsp/pra.h>
#include <linux/fsp/hdcr.h>


#define IRQF_DISABLED 0

static u32 mfl2(u32 reg)
{
	u32 data;
	unsigned long flags;

	local_irq_save(flags);
	mtdcr(DCRN_L2CDCRAI, reg);
	data = mfdcr(DCRN_L2CDCRDI);
	local_irq_restore(flags);

	return data;
}

static void mtl2(u32 reg, u32 data)
{
	unsigned long flags;

	local_irq_save(flags);
	mtdcr(DCRN_L2CDCRAI, reg);
	mtdcr(DCRN_L2CDCRDI, data);
	local_irq_restore(flags);
}

static void show_l2_regs(void)
{
	printk("L2 Controller:\n");
	printk("MCK:      0x%08x\n", mfl2(L2MCK));
	printk("INT:      0x%08x\n", mfl2(L2INT));
	printk("PLBSTAT0: 0x%08x\n", mfl2(L2PLBSTAT0));
	printk("PLBSTAT1: 0x%08x\n", mfl2(L2PLBSTAT1));
	printk("ARRSTAT0: 0x%08x\n", mfl2(L2ARRSTAT0));
	printk("ARRSTAT1: 0x%08x\n", mfl2(L2ARRSTAT1));
	printk("ARRSTAT2: 0x%08x\n", mfl2(L2ARRSTAT2));
	printk("CPUSTAT:  0x%08x\n", mfl2(L2CPUSTAT));
	printk("RACSTAT0: 0x%08x\n", mfl2(L2RACSTAT0));
	printk("WACSTAT0: 0x%08x\n", mfl2(L2WACSTAT0));
	printk("WACSTAT1: 0x%08x\n", mfl2(L2WACSTAT1));
	printk("WACSTAT2: 0x%08x\n", mfl2(L2WACSTAT2));
	printk("WDFSTAT:  0x%08x\n", mfl2(L2WDFSTAT));
	printk("LOG0:     0x%08x\n", mfl2(L2LOG0));
	printk("LOG1:     0x%08x\n", mfl2(L2LOG1));
	printk("LOG2:     0x%08x\n", mfl2(L2LOG2));
	printk("LOG3:     0x%08x\n", mfl2(L2LOG3));
	printk("LOG4:     0x%08x\n", mfl2(L2LOG4));
	printk("LOG5:     0x%08x\n", mfl2(L2LOG5));
}

static void show_plbopb_regs(u32 base, int num)
{
	printk("\nPLBOPB Bridge %d:\n", num);
	printk("GESR0: 0x%08x\n", mfdcr(base + PLB4OPB_GESR0));
	printk("GESR1: 0x%08x\n", mfdcr(base + PLB4OPB_GESR1));
	printk("GESR2: 0x%08x\n", mfdcr(base + PLB4OPB_GESR2));
	printk("GEARU: 0x%08x\n", mfdcr(base + PLB4OPB_GEARU));
	printk("GEAR:  0x%08x\n", mfdcr(base + PLB4OPB_GEAR));
}

static void show_bus_regs(void)
{
	show_l2_regs();

	printk("\nPLB6 Controller:\n");
	printk("BC_SHD: 0x%08x\n", mfdcr(DCRN_PLB6_SHD));
	printk("BC_ERR: 0x%08x\n", mfdcr(DCRN_PLB6_ERR));

	printk("\nPLB6-to-PLB4 Bridge:\n");
	printk("ESR:  0x%08x\n", mfdcr(DCRN_PLB6PLB4_ESR));
	printk("EARH: 0x%08x\n", mfdcr(DCRN_PLB6PLB4_EARH));
	printk("EARL: 0x%08x\n", mfdcr(DCRN_PLB6PLB4_EARL));

	printk("\nPLB4-to-PLB6 Bridge:\n");
	printk("ESR:  0x%08x\n", mfdcr(DCRN_PLB4PLB6_ESR));
	printk("EARH: 0x%08x\n", mfdcr(DCRN_PLB4PLB6_EARH));
	printk("EARL: 0x%08x\n", mfdcr(DCRN_PLB4PLB6_EARL));

	printk("\nPLB6-to-MCIF Bridge:\n");
	printk("BESR0: 0x%08x\n", mfdcr(DCRN_PLB6MCIF_BESR0));
	printk("BESR1: 0x%08x\n", mfdcr(DCRN_PLB6MCIF_BESR1));
	printk("BEARH: 0x%08x\n", mfdcr(DCRN_PLB6MCIF_BEARH));
	printk("BEARL: 0x%08x\n", mfdcr(DCRN_PLB6MCIF_BEARL));

	printk("\nPLB4 Arbiter:\n");
	printk("P0ESRH 0x%08x\n", mfdcr(DCRN_PLB4_P0ESRH));
	printk("P0ESRL 0x%08x\n", mfdcr(DCRN_PLB4_P0ESRL));
	printk("P0EARH 0x%08x\n", mfdcr(DCRN_PLB4_P0EARH));
	printk("P0EARH 0x%08x\n", mfdcr(DCRN_PLB4_P0EARH));
	printk("P1ESRH 0x%08x\n", mfdcr(DCRN_PLB4_P1ESRH));
	printk("P1ESRL 0x%08x\n", mfdcr(DCRN_PLB4_P1ESRL));
	printk("P1EARH 0x%08x\n", mfdcr(DCRN_PLB4_P1EARH));
	printk("P1EARH 0x%08x\n", mfdcr(DCRN_PLB4_P1EARH));

	show_plbopb_regs(DCRN_PLB4OPB0_BASE, 0);
	show_plbopb_regs(DCRN_PLB4OPB1_BASE, 1);
	show_plbopb_regs(DCRN_PLB4OPB2_BASE, 2);
	show_plbopb_regs(DCRN_PLB4OPB3_BASE, 3);

	printk("\nPLB4-to-AHB Bridge:\n");
	printk("ESR:   0x%08x\n", mfdcr(DCRN_PLB4AHB_ESR));
	printk("SEUAR: 0x%08x\n", mfdcr(DCRN_PLB4AHB_SEUAR));
	printk("SELAR: 0x%08x\n", mfdcr(DCRN_PLB4AHB_SELAR));

	printk("\nAHB-to-PLB4 Bridge:\n");
	printk("\nESR: 0x%08x\n", mfdcr(DCRN_AHBPLB4_ESR));
	printk("\nEAR: 0x%08x\n", mfdcr(DCRN_AHBPLB4_EAR));
}

static irqreturn_t bus_error_irq(int irq, void *data)
{
	struct device_node *node = data;
	const char *name = node->name;

	printk("Bus Error: %s\n", name);

	show_bus_regs();
	show_regs(get_irq_regs());

	//pra_write(PRA_CALLOUT, PRA_HWSWERR);
	panic("Bus Error");

	return IRQ_HANDLED;
}

static irqreturn_t cmu_error_irq(int irq, void *data)
{
	printk("CMU Error\n");
	printk("FIR0: 0x%08x\n", mfcmu(CMUN_FIR0));

	/*
	 * TODO: The CONFFIR0-3 registers are broken for dd1, but we might want
	 * to print them here if we get a dd2.
	 */

	//pra_write(PRA_CALLOUT, PRA_CMU);
	panic("CMU Error");

	return IRQ_HANDLED;
}

static irqreturn_t conf_error_irq(int irq, void *data)
{
	printk("Configuration Logic Error\n");
	printk("CONF_FIR: 0x%08x\n", mfdcr(DCRN_CONF_FIR_RWC));
	printk("RPERR0:   0x%08x\n", mfdcr(DCRN_CONF_RPERR0));
	printk("RPERR1:   0x%08x\n", mfdcr(DCRN_CONF_RPERR1));

	//pra_write(PRA_CALLOUT, PRA_CONF_LOGIC);
	panic("Configuration Logic Error");

	return IRQ_HANDLED;
}

void __iomem *spad_cfg_addr;
void __iomem *gpio_addr;

static irqreturn_t opbd_error_irq(int irq, void *data)
{
	printk("OPBD Error\n");
	printk("SPAD FIR: 0x%08x\n", readl_be(spad_cfg_addr + 0x20));
	printk("GPIO FIR: 0x%08x\n", readl_be(gpio_addr + 0x18));

	//pra_write(PRA_CALLOUT, PRA_HWSWERR);
	panic("OPBD Error");

	return IRQ_HANDLED;
}

static void show_mc_regs(void)
{
	unsigned int base = DCRN_DDR34_BASE;
	printk("MCSTAT:            0x%08x\n", mfdcr(base+DCRN_DDR34_MCSTAT));
	printk("MCOPT1:            0x%08x\n", mfdcr(base+DCRN_DDR34_MCOPT1));
	printk("MCOPT2:            0x%08x\n", mfdcr(base+DCRN_DDR34_MCOPT2));
	printk("PHYSTAT:           0x%08x\n", mfdcr(base+DCRN_DDR34_PHYSTAT));
	printk("CFGR0:             0x%08x\n", mfdcr(base+DCRN_DDR34_CFGR0));
	printk("CFGR1:             0x%08x\n", mfdcr(base+DCRN_DDR34_CFGR1));
	printk("CFGR2:             0x%08x\n", mfdcr(base+DCRN_DDR34_CFGR2));
	printk("CFGR3:             0x%08x\n", mfdcr(base+DCRN_DDR34_CFGR3));
	printk("SCRUB_CNTL:        0x%08x\n",
		mfdcr(base+DCRN_DDR34_SCRUB_CNTL));
	printk("ECCERR_PORT0:      0x%08x\n",
		mfdcr(base+DCRN_DDR34_ECCERR_PORT0));
	printk("ECCERR_ADDR_PORT0: 0x%08x\n",
		mfdcr(base+DCRN_DDR34_ECCERR_ADDR_PORT0));
	printk("ECCERR_CNT_PORT0:  0x%08x\n",
		mfdcr(base+DCRN_DDR34_ECCERR_COUNT_PORT0));
	printk("ECC_CHECK_PORT0:   0x%08x\n",
		mfdcr(base+DCRN_DDR34_ECC_CHECK_PORT0));

	/* Core Wrapper */
	base = DCRN_CW_BASE;
	printk("MCER0:            0x%08x\n", mfdcr(base+DCRN_CW_MCER0));
	printk("MCER1:            0x%08x\n", mfdcr(base+DCRN_CW_MCER1));

	/* PLB6-MCIF bridge */
	printk("BESR:             0x%08x\n", mfdcr(DCRN_PLB6MCIF_BESR0));
	printk("BEARL:            0x%08x\n", mfdcr(DCRN_PLB6MCIF_BEARL));
	printk("BEARH:            0x%08x\n", mfdcr(DCRN_PLB6MCIF_BEARH));
}

static irqreturn_t mc_ue_irq(int irq, void *data)
{
	printk("DDR: Uncorrectable Error\n");

	show_mc_regs();

	//pra_write(PRA_CALLOUT, PRA_ECCERR);
	panic("DDR: Uncorrectable Error\n");

	return IRQ_HANDLED;
}

void WatchdogHandler(struct pt_regs *regs)
{
	mtspr(SPRN_TCR, mfspr(SPRN_TCR) & ~TCR_WIE);
	show_regs(regs);
	panic("Received Watchdog Critical Interrupt");
}

int fsp_machine_check_exception(struct pt_regs *regs)
{
	u32 mcsr = mfspr(SPRN_MCSR);

	printk("Machine Check: MCSR=0x%x\n", mcsr);

	if (mcsr & MCSR_TLBP) {
		printk("UTLB Parity Error\n");
		//pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_TLB);
	}
	else if (mcsr & MCSR_ICP) {
		printk("I-Cache Asynchronous Error\n");
		//pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_ICACHE);
	}
	else if (mcsr & MCSR_DCSP) {
		printk("D-Cache Error\n");
		//pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_DCACHE);
	}
	else if (mcsr & PPC47x_MCSR_GPR) {
		printk("GPR Parity Error\n");
		//pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_GPR);
	}
	else if (mcsr & PPC47x_MCSR_FPR) {
		printk("FPR Parity Error\n");
		//pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_FPR);
	}
	else if (mcsr & MCSR_L2) {
		u32 l2mck = mfl2(L2MCK);

		printk("L1/L2 Cache Error\n");
		//pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_L1_L2);

		show_l2_regs();
	}
	else if (mcsr & MCSR_DCR) {
		printk("DCR Timeout\n");
		//pra_write(PRA_CALLOUT, PRA_HWSWERR);
	}

	show_regs(regs);

	/* Clear MCSR in case we get another machine check during kexec */
	mtspr(SPRN_MCSRC, mcsr);

	panic("Machine Check\n");
}

static int fsp_of_probe(struct platform_device *ofdev)
{
	int rc;
	unsigned int irq;
	struct device_node *node = ofdev->dev.of_node;
	irq_handler_t handler = ofdev->dev.driver->of_match_table->data;

	 /* Ignore error interrupts if we're in the capture kernel */
	/* if (is_kdump_kernel()) FIXME - Error IRQs setup causing kernel panic on MCP 8 */
		return 0;

	irq = irq_of_parse_and_map(node, 0);
	if (irq == NO_IRQ)
		panic("Device tree node %s is missing a valid interrupt",
		      node->name);

	rc = request_irq(irq, handler, IRQF_DISABLED, node->name, node);

	if (rc)
		panic("fsp_of_probe: request_irq failed: node=%s rc=%d",
		      node->full_name, rc);

	return 0;
}

static struct of_device_id fsp_of_match_table[] = {
	{
		.compatible	= "ibm,bus-error-irq",
		.data		= bus_error_irq,
	},
	{
		.compatible	= "ibm,cmu-error-irq",
		.data		= cmu_error_irq,
	},
	{
		.compatible	= "ibm,conf-error-irq",
		.data		= conf_error_irq,
	},
	{
		.compatible	= "ibm,opbd-error-irq",
		.data		= opbd_error_irq,
	},
	{
		.compatible	= "ibm,mc-ue-irq",
		.data		= mc_ue_irq,
	},
	{
		.compatible	= "ibm,reset-warning-irq",
		.data		= reset_warning_irq,
	},
	{},
};

static struct platform_driver fsp_of_driver = {
	.driver = {
		.name		= "fsp",
		.of_match_table 	= fsp_of_match_table,
	},
	.probe		= fsp_of_probe,
};

static int __init fsp_handlers_init(void)
{
	u32 ccr2;

	/*
	 * L2 error reporting initialization
	 *
	 * The goal here is to configure parity errors and definite HW errors
	 * as machine checks, handled by fsp_machine_check_exception().  Any
	 * other errors that could have been caused by SW or other bus errors
	 * are configured as critical interrupts, handled by bus_error_irq().
	 */

	/* L2 machine checks */
	mtl2(L2PLBMCKEN0, 0xffffffff);
	mtl2(L2PLBMCKEN1, 0x0000ffff);
	mtl2(L2ARRMCKEN0, 0xffffffff);
	mtl2(L2ARRMCKEN1, 0xffffffff);
	mtl2(L2ARRMCKEN2, 0xfffff000);
	mtl2(L2CPUMCKEN, 0xffffffff);
	mtl2(L2RACMCKEN0, 0xffffffff);
	mtl2(L2WACMCKEN0, 0xffffffff);
	mtl2(L2WACMCKEN1, 0xffffffff);
	mtl2(L2WACMCKEN2, 0xffffffff);
	mtl2(L2WDFMCKEN, 0xffffffff);

	/* L2 interrupts */
	mtl2(L2PLBINTEN1, 0xffff0000);

	/*
	 * At a global level, enable all L2 machine checks and interrupts
	 * reported by the L2 subsystems, except for the external machine check
	 * input (UIC0.1) which we don't plan on using.  Instead, those will be
	 * coming in on UIC0 as critical interrupts.
	 */
	mtl2(L2MCKEN, 0x000007ff);
	mtl2(L2INTEN, 0x000007ff);

	/* Enable the DCR timeout machine check */
	ccr2 = mfspr(SPRN_CCR2);
	ccr2 |= 0x00000400;
	mtspr(SPRN_CCR2, ccr2);

	/* Enable FSP-2 configuration logic parity errors */
	mtdcr(DCRN_CONF_EIR_RS, 0x80000000);

	/* Enable SPAD errors through the SPAD EEIR register */
	/* TODO: get the base address from the device tree and use defines for
	 * the register accesses */
	spad_cfg_addr = ioremap(0x10b0027f00ULL, 128);
	writel_be(0xfe000000, spad_cfg_addr + 0x30);

	/* Enable GPIO errors through the GPIO EIR register */
	/* TODO: get GPIO driver to do this instead? */
	gpio_addr = ioremap(0x10b0027f80ULL, 64);
	writel_be(0x0000c100, gpio_addr + 0x20);

	return platform_driver_register(&fsp_of_driver);
}

arch_initcall(fsp_handlers_init);
