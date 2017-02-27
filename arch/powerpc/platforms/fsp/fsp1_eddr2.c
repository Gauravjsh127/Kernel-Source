/*
 *   Copyright (c) International Business Machines Corp., 2005, 2011
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/of_platform.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/reg.h>
#include <asm/dcr.h>

#include <linux/fsp/pra.h>

#define SCRUB_RATE 32	/* 1 scrub every 32 auto-refresh cycles */
#define SCRUB_BUFSZ 10

static unsigned long eddr2_addr_dcr, eddr2_data_dcr;
static unsigned long scrub_ce_addr[SCRUB_BUFSZ];
static int scrub_ce_count;
#define SCRUB_INDEX(count) ((count - 1) % SCRUB_BUFSZ)

static void mtddr(unsigned long reg, unsigned long val)
{
	BUG_ON(!eddr2_addr_dcr || !eddr2_data_dcr);

	mtdcr(eddr2_addr_dcr, reg);
	mtdcr(eddr2_data_dcr, val);
}

static unsigned long mfddr(unsigned long reg)
{
	BUG_ON(!eddr2_addr_dcr || !eddr2_data_dcr);

	mtdcr(eddr2_addr_dcr, reg);
	return mfdcr(eddr2_data_dcr);
}

void show_eddr2_regs(void)
{
	printk("EDDR2_BESRDC %08lx\n", mfddr(EDDR2_BESRDC));
	printk("EDDR2_BEARD %08lx\n", mfddr(EDDR2_BEARD));
	printk("EDDR2_WPESRC %08lx\n", mfddr(EDDR2_WPESRC));
	printk("EDDR2_WPEAR %08lx\n", mfddr(EDDR2_WPEAR));
	printk("EDDR2_WMIRQC %08lx\n", mfddr(EDDR2_WMIRQC));
	printk("EDDR2_PLBOPT %08lx\n", mfddr(EDDR2_PLBOPT));
	printk("EDDR2_PUABA %08lx\n", mfddr(EDDR2_PUABA));
	printk("EDDR2_WPAIC %08lx\n", mfddr(EDDR2_WPAIC));
	printk("EDDR2_WPIRQC %08lx\n", mfddr(EDDR2_WPIRQC));
	printk("EDDR2_WPAPE %08lx\n", mfddr(EDDR2_WPAPE));
	printk("EDDR2_MCSTAT %08lx\n", mfddr(EDDR2_MCSTAT));
	printk("EDDR2_MCOPT1 %08lx\n", mfddr(EDDR2_MCOPT1));
	printk("EDDR2_MCOPT2 %08lx\n", mfddr(EDDR2_MCOPT2));
	printk("EDDR2_MODT0 %08lx\n", mfddr(EDDR2_MODT0));
	printk("EDDR2_MODT1 %08lx\n", mfddr(EDDR2_MODT1));
	printk("EDDR2_CODT %08lx\n", mfddr(EDDR2_CODT));
	printk("EDDR2_VVPR %08lx\n", mfddr(EDDR2_VVPR));
	printk("EDDR2_OPARS %08lx\n", mfddr(EDDR2_OPARS));
	/* don't even read register OPART, according to the edd2 spec
	   this triggers 'Adjust Set and Drive/Sense Sequence Start'
	   printk("EDDR2_OPART %08lx\n", mfddr(EDDR2_OPART)); */
	printk("EDDR2_RTR %08lx\n", mfddr(EDDR2_RTR));
	printk("EDDR2_MR0CF %08lx\n", mfddr(EDDR2_MR0CF));
	printk("EDDR2_MR0EA %08lx\n", mfddr(EDDR2_MR0EA));
	printk("EDDR2_MR1CF %08lx\n", mfddr(EDDR2_MR1CF));
	printk("EDDR2_MR1EA %08lx\n", mfddr(EDDR2_MR1EA));
	printk("EDDR2_TOM %08lx\n", mfddr(EDDR2_TOM));
	printk("EDDR2_INITPLR0 %08lx\n", mfddr(EDDR2_INITPLR0));
	printk("EDDR2_INITPLR1 %08lx\n", mfddr(EDDR2_INITPLR1));
	printk("EDDR2_INITPLR2 %08lx\n", mfddr(EDDR2_INITPLR2));
	printk("EDDR2_INITPLR3 %08lx\n", mfddr(EDDR2_INITPLR3));
	printk("EDDR2_INITPLR4 %08lx\n", mfddr(EDDR2_INITPLR4));
	printk("EDDR2_INITPLR5 %08lx\n", mfddr(EDDR2_INITPLR5));
	printk("EDDR2_INITPLR6 %08lx\n", mfddr(EDDR2_INITPLR6));
	printk("EDDR2_INITPLR7 %08lx\n", mfddr(EDDR2_INITPLR7));
	printk("EDDR2_INITPLR8 %08lx\n", mfddr(EDDR2_INITPLR8));
	printk("EDDR2_INITPLR9 %08lx\n", mfddr(EDDR2_INITPLR9));
	printk("EDDR2_INITPLR10 %08lx\n", mfddr(EDDR2_INITPLR10));
	printk("EDDR2_INITPLR11 %08lx\n", mfddr(EDDR2_INITPLR11));
	printk("EDDR2_INITPLR12 %08lx\n", mfddr(EDDR2_INITPLR12));
	printk("EDDR2_INITPLR13 %08lx\n", mfddr(EDDR2_INITPLR13));
	printk("EDDR2_INITPLR14 %08lx\n", mfddr(EDDR2_INITPLR14));
	printk("EDDR2_INITPLR15 %08lx\n", mfddr(EDDR2_INITPLR15));
	printk("EDDR2_RQDC %08lx\n", mfddr(EDDR2_RQDC));
	printk("EDDR2_RFDC %08lx\n", mfddr(EDDR2_RFDC));
	printk("EDDR2_RDCC %08lx\n", mfddr(EDDR2_RDCC));
	printk("EDDR2_DLCR %08lx\n", mfddr(EDDR2_DLCR));
	printk("EDDR2_CLKTR %08lx\n", mfddr(EDDR2_CLKTR));
	printk("EDDR2_WRDTR %08lx\n", mfddr(EDDR2_WRDTR));
	printk("EDDR2_SDTR1 %08lx\n", mfddr(EDDR2_SDTR1));
	printk("EDDR2_SDTR2 %08lx\n", mfddr(EDDR2_SDTR2));
	printk("EDDR2_SDTR3 %08lx\n", mfddr(EDDR2_SDTR3));
	printk("EDDR2_MMODE %08lx\n", mfddr(EDDR2_MMODE));
	printk("EDDR2_MEMODE %08lx\n", mfddr(EDDR2_MEMODE));
	printk("EDDR2_ECCES %08lx\n", mfddr(EDDR2_ECCES));
	printk("EDDR2_ECCCEA %08lx\n", mfddr(EDDR2_ECCCEA));
	printk("EDDR2_SIECR %08lx\n", mfddr(EDDR2_SIECR));
	printk("EDDR2_SIESA %08lx\n", mfddr(EDDR2_SIESA));
	printk("EDDR2_SIEEA %08lx\n", mfddr(EDDR2_SIEEA));
	printk("EDDR2_SEUEA %08lx\n", mfddr(EDDR2_SEUEA));
	printk("EDDR2_SEUES %08lx\n", mfddr(EDDR2_SEUES));
	printk("EDDR2_SECEA %08lx\n", mfddr(EDDR2_SECEA));
	printk("EDDR2_SECES %08lx\n", mfddr(EDDR2_SECES));
	printk("EDDR2_SEES0 %08lx\n", mfddr(EDDR2_SEES0));
	printk("EDDR2_SEES1 %08lx\n", mfddr(EDDR2_SEES1));
	printk("EDDR2_SFPM %08lx\n", mfddr(EDDR2_SFPM));
	printk("EDDR2_SIECA %08lx\n", mfddr(EDDR2_SIECA));
	printk("EDDR2_FCSR %08lx\n", mfddr(EDDR2_FCSR));
	printk("EDDR2_RTSR %08lx\n", mfddr(EDDR2_RTSR));
	printk("EDDR2_CID %08lx\n", mfddr(EDDR2_CID));
	printk("EDDR2_RID %08lx\n", mfddr(EDDR2_RID));
}

static void start_scrub_continuous_interval(void)
{
	unsigned long siecr;

	/* disable any scrub operation that might already be in progress */
	mtddr(EDDR2_SIECR, 0);

	/* initialize the engine: continuous, stop and lock on error,
	 * scrub every 32 auto-refresh cycles. */
	siecr = SCRUB_SIECR_MODE_INTERVAL_CONTINUOUS | SCRUB_SIECR_SUER |
		SCRUB_SIECR_SCER | SCRUB_SIECR_LUES | SCRUB_SIECR_LCES |
		SCRUB_RATE;

	mtddr(EDDR2_SIESA, 0);			/* start addr */
	mtddr(EDDR2_SIEEA, mfddr(EDDR2_TOM));	/* end addr */
	mtddr(EDDR2_SIECR, siecr);
}

static irqreturn_t ecc_ue_handler(int irq, void *ptr, struct pt_regs *regs)
{
	printk("ECC UE handler: Uncorrectable memory error found!\n");
	printk("Error status: 0x%08lx\n", mfddr(EDDR2_ECCES));

	pra_write(PRA_CALLOUT, PRA_ECCERR);
	show_eddr2_regs();
	panic("Uncorrectable memory error!");
}

static irqreturn_t ecc_ce_handler(int irq, void *ptr, struct pt_regs *regs)
{
	unsigned long error_addr = mfddr(EDDR2_ECCCEA);

	printk("ECC CE handler: Correctable memory error found!\n");
	printk("Error address : 0x%08lx\n", error_addr);
	printk("Error status: 0x%08lx\n", mfddr(EDDR2_ECCES));

	/* clear all error bits */
	mtddr(EDDR2_ECCES, 0xffff7c00);

	/* stop existing scrub */
	mtddr(EDDR2_SIECR, 0);

	/*
	 * Configure an immediate scrub to correct the data.  This will result
	 * in the scrub CE handler getting called, which will increase the
	 * scrub count.  Then the Operation Completed (OC) handler will be
	 * called, which will restart scrubbing.
	 */
	mtddr(EDDR2_SIESA, error_addr);
	mtddr(EDDR2_SIECR, SCRUB_SIECR_MODE_IMMEDIATE_SINGLE |
			   SCRUB_SIECR_SUER | SCRUB_SIECR_SCER |
			   SCRUB_SIECR_LUES | SCRUB_SIECR_LCES);

	return IRQ_HANDLED;
}

static irqreturn_t scrub_ue_handler(int irq, void *ptr, struct pt_regs *regs)
{
	printk("Scrub UE handler\n");
	printk("SEES SEUES SEUEA: 0x%08lx 0x%08lx 0x%08lx\n",
		mfddr(EDDR2_SEES0), mfddr(EDDR2_SEUES), mfddr(EDDR2_SEUEA));

	pra_write(PRA_CALLOUT, PRA_ECCERR);
	show_eddr2_regs();
	panic("Uncorrectable memory error!");
}

static irqreturn_t scrub_ce_handler(int irq, void *ptr, struct pt_regs *regs)
{
	scrub_ce_count++;
	scrub_ce_addr[SCRUB_INDEX(scrub_ce_count)] = mfddr(EDDR2_SECEA);

	printk("Scrub CE handler\n");
	printk("scrub_ce_count SEES SECES SECEA: %d 0x%08lx 0x%08lx 0x%08lx\n",
		scrub_ce_count, mfddr(EDDR2_SEES0), mfddr(EDDR2_SECES),
		mfddr(EDDR2_SECEA));

	/* clear CE & SIES */
	mtddr(EDDR2_SEES0, SEES_CEI | SEES_SIES_MASK);

	/*
	 * Now scrub_oc_handler will be called, which will restart the
	 * scrubbing.
	 */

	return IRQ_HANDLED;
}

static irqreturn_t scrub_oc_handler(int irq, void *ptr, struct pt_regs *regs)
{
	printk("Scrub operation completed\n");
	printk("SEES: 0x%08lx\n", mfddr(EDDR2_SEES0));

	/* clear OC bit */
	mtddr(EDDR2_SEES0, SEES_SIEOC);

	/* restart scrubbing */
	start_scrub_continuous_interval();

	return IRQ_HANDLED;
}

static int scrub_count_read(char *page, char **start,
			    off_t off, int count, int *eof, void *data)
{
	int len = 0;

	len = sprintf(page, "%d\n", scrub_ce_count);
	*eof = 1;
	return len;
}

static int scrub_addr_read(char *page, char **start,
			   off_t off, int count, int *eof, void *data)
{
	int len = 0;
	int i, j;

	for (i = 0, j = scrub_ce_count;(j > 0) && (i < SCRUB_BUFSZ); i++, j--)
		len += sprintf(page+len, "0x%08lx\n",
			       scrub_ce_addr[SCRUB_INDEX(j)]);

	*eof = 1;
	return len;
}

/* TODO: could this struct be useful in other code? */
struct of_irq_handler {
	char *name;
	void *handler;
};

static struct of_irq_handler irqs[] = {
	{
		.name		= "eDDR2 ECC Uncorrectable Error",
		.handler	= ecc_ue_handler,
	},
	{
		.name		= "eDDR2 ECC Correctable Error",
		.handler	= ecc_ce_handler,
	},
	{
		.name		= "eDDR2 Scrub ECC Uncorrectable Error",
		.handler	= scrub_ue_handler,
	},
	{
		.name		= "eDDR2 Scrub ECC Correctable Error",
		.handler	= scrub_ce_handler,
	},
	{
		.name		= "eDDR2 Scrub ECC Operation Complete",
		.handler	= scrub_oc_handler,
	},
	{},
};

static int __devinit fsp_eddr2_of_probe(struct of_device *ofdev,
					const struct of_device_id *match)
{
	unsigned int dcr_base, dcr_len, irq;
	int i, rc;

	dcr_base = dcr_resource_start(ofdev->node, 0);
	if (!dcr_base)
		panic("scrub: can't find DCR resource!");

	dcr_len = dcr_resource_len(ofdev->node, 0);
	if (dcr_len != 2)
		panic("scrub: bad DCR resource len %d!", dcr_len);

	/* setup global variables for mfddr() functions */
	eddr2_addr_dcr = dcr_base;
	eddr2_data_dcr = dcr_base + 1;

	for (i = 0; irqs[i].name; i++) {
		irq = irq_of_parse_and_map(ofdev->node, i);
		if (irq == NO_IRQ)
			panic("scrub: irq_of_parse_and_map failed: "
			      "idx=%d name='%s'", i, irqs[i].name);

		rc = request_irq(irq, irqs[i].handler, 0, irqs[i].name, NULL);
		if (rc)
			panic("scrub: request_irq failed: rc=%d irq=%d "
			      "idx=%d name='%s'", rc, irq, i, irqs[i].name);
	}

	return 0;
}

static struct of_device_id __devinitdata fsp_eddr2_match_table[] = {
	{ .compatible	= "ibm,eddr2", },
	{},
};

static struct of_platform_driver fsp_eddr2_of_driver = {
	.name		= "eddr2",
	.match_table	= fsp_eddr2_match_table,
	.probe		= fsp_eddr2_of_probe,
};

static int __init fsp_eddr2_init(void)
{
	struct proc_dir_entry *procdir;
	struct proc_dir_entry *procentry;
	int ret;

	procdir = proc_mkdir("scrub", NULL);
	if (!procdir)
		goto error;

	procentry = create_proc_read_entry("count", 644, procdir,
					   scrub_count_read, NULL);
	if (!procentry)
		goto error;

	procentry = create_proc_read_entry("addr", 644, procdir,
					   scrub_addr_read, NULL);
	if (!procentry)
		goto error;


	ret = of_register_platform_driver(&fsp_eddr2_of_driver);
	if (ret)
		goto error;

	start_scrub_continuous_interval();

	return 0;

error:
	panic("fsp_eddr2_init failed");

	return 0;
}

device_initcall(fsp_eddr2_init);
