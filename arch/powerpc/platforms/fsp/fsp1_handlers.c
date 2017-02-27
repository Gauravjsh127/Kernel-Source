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
#include <asm/reg.h>
#include <asm/dcr.h>

#include <linux/fsp/pra.h>
#include "fsp.h"

static irqreturn_t buddy_reset_irq(int irq, void *dontcare)
{
	show_regs(get_irq_regs());
	panic("Buddy initiated reset" );
	return IRQ_HANDLED;

}

static void show_plb_arbiter_regs(int plbarb_nr)
{
	unsigned long pcbi, pacr, pesrl, pesrh, pearl, pearh;

	pcbi = mfdcr(DCRN_PLB_PCBI+plbarb_nr*8);
	pacr = mfdcr(DCRN_PLB_P0ACR+plbarb_nr*8);
	pesrl= mfdcr(DCRN_PLB_P0ESRL+plbarb_nr*8);
	pesrh= mfdcr(DCRN_PLB_P0ESRH+plbarb_nr*8);
	pearl= mfdcr(DCRN_PLB_P0EARL+plbarb_nr*8);
	pearh= mfdcr(DCRN_PLB_P0EARH+plbarb_nr*8);
	printk("PLB_P%dESRL %lx\n", plbarb_nr, pesrl);
	printk("PLB_P%dESRH %lx\n", plbarb_nr, pesrh);
	printk("PLB_P%dEARL %lx\n", plbarb_nr, pearl);
	printk("PLB_P%dEARH %lx\n", plbarb_nr, pearh);
	printk("PLB_P%dACR %lx\n", plbarb_nr, pacr);
	printk("PLB_PCBI/PLB_PCBC %lx\n", pcbi);
}

static void show_plbopb_regs(int plbopb_nr)
{
	unsigned long gesr0, gesr1, gear, gearu, gconfg, glatency, grev;

	gesr0 = mfdcr(DCRN_PLB0_GESR0+plbopb_nr*16);
	gesr1 = mfdcr(DCRN_PLB0_GESR1+plbopb_nr*16);
	gear = mfdcr(DCRN_PLB0_GEAR+plbopb_nr*16);
	gearu = mfdcr(DCRN_PLB0_GEARU+plbopb_nr*16);
	gconfg = mfdcr(DCRN_PLB0_GCONFG+plbopb_nr*16);
	glatency = mfdcr(DCRN_PLB0_GLATENCY+plbopb_nr*16);
	grev = mfdcr(DCRN_PLB0_GREV+plbopb_nr*16);

	printk("PLBOPB%d GESR0 %08lx\n", plbopb_nr, gesr0);
	printk("PLBOPB%d GESR1 %08lx\n", plbopb_nr, gesr1);
	printk("PLBOPB%d GEAR %08lx\n", plbopb_nr, gear);
	printk("PLBOPB%d GEARU %08lx\n", plbopb_nr, gearu);
	printk("PLBOPB%d GCONFIG %08lx\n", plbopb_nr, gconfg);
	printk("PLBOPB%d GLATENCY %08lx\n", plbopb_nr, glatency);
	printk("PLBOPB%d GREV %08lx\n", plbopb_nr, grev);
}

static irqreturn_t bus_error_irq(int irq, void *data)
{
	struct device_node *node = data;
	const char *name = node->name;

	printk("Bus Error: %s\n", name);

	show_plb_arbiter_regs(0);
	show_plb_arbiter_regs(1);
	show_plbopb_regs(0);
	show_plbopb_regs(1);
	show_plbopb_regs(2);
	show_plbopb_regs(3);
	show_regs(get_irq_regs());

	pra_write(PRA_CALLOUT, PRA_HWSWERR);
	panic("Bus Error: %s\n", name);
	return IRQ_HANDLED;
}

#define	EBC_BEAR 0x20
#define	EBC_BESR 0x21
#define	EBC_CFG 0x23
static irqreturn_t ebc_error_irq(int irq, void *data)
{
	struct device_node *node = data;
	unsigned int ebc_addr, ebc_data, ebc_besr, ebc_bear, ebc_cfg;

	show_regs(get_irq_regs());

	ebc_addr = dcr_resource_start(node, 0);
	if (!ebc_addr)
		panic("ebc_error_irq: dcr_resource_start failed");

	ebc_data = ebc_addr + 1;

	mtdcr(ebc_addr, EBC_BESR);
	ebc_besr = mfdcr(ebc_data);
	mtdcr(ebc_addr, EBC_BEAR);
	ebc_bear = mfdcr(ebc_data);
	mtdcr(ebc_addr, EBC_CFG);
	ebc_cfg = mfdcr(ebc_data);

	printk("EBC_BESR: %08x\n", ebc_besr);
	printk("EBC_BEAR: %08x\n", ebc_bear);
	printk("EBC_CFG: %08x\n", ebc_cfg);

	pra_write(PRA_CALLOUT, PRA_HWSWERR);
	panic("EBC error\n");

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

	if (mcsr & MCSR_ICP) {
                printk("I-Cache Parity Error\n");
		pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_ICACHE);
        }
	else if (mcsr & MCSR_TLBP) {
                printk("TLB Parity Error\n");
		pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_TLB);
        }
	else if (mcsr & MCSR_DCSP) {
                printk("D-Cache Search Parity Error\n");
                pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_DCACHE);
        }
	else if (mcsr & MCSR_DCFP) {
                printk("D-Cache Flush Parity Error\n");
                pra_write(PRA_CALLOUT, PRA_TLBCACHEERR | PRA_CALLOUT_DCACHE);
        }
	else if (mcsr & MCSR_IB) {
		printk("Instruction PLB Error\n");
		pra_write(PRA_CALLOUT, PRA_HWSWERR);
	}
	else if (mcsr & MCSR_DRB) {
		printk("Data Read PLB Error\n");
		pra_write(PRA_CALLOUT, PRA_HWSWERR);
	}
	else if (mcsr & MCSR_DWB) {
		printk("Data Write PLB Error\n");
		pra_write(PRA_CALLOUT, PRA_HWSWERR);
	}

	show_plb_arbiter_regs(0);
	show_plb_arbiter_regs(1);
	show_plbopb_regs(0);
	show_plbopb_regs(1);
	show_plbopb_regs(2);
	show_plbopb_regs(3);
	show_eddr2_regs();
	show_regs(regs);

        panic("Machine Check\n");
}

static int __devinit fsp_of_probe(struct of_device *ofdev,
				  const struct of_device_id *match)
{
	int rc;
	unsigned int irq;
	struct device_node *node = ofdev->node;
	irq_handler_t handler = match->data;

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

static struct of_device_id __devinitdata fsp_of_match_table[] = {
	{
		.compatible	= "ibm,buddy-reset-irq",
		.data		= buddy_reset_irq,
	},
	{
		.compatible	= "ibm,bus-error-irq",
		.data		= bus_error_irq,
	},
	{
		.compatible	= "ibm,ebc",
		.data		= ebc_error_irq,
	},
	{},
};

static struct of_platform_driver fsp_of_driver = {
	.name		= "fsp",
	.match_table 	= fsp_of_match_table,
	.probe		= fsp_of_probe,
};

static int __init fsp_handlers_init(void)
{
	return of_register_platform_driver(&fsp_of_driver);
}

arch_initcall(fsp_handlers_init);
