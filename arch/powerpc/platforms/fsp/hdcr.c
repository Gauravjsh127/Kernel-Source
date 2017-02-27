/*
 *   Copyright (c) International Business Machines Corp., 2010, 2012
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
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <linux/crash_dump.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/reg.h>
#include <asm/dcr.h>

#include <linux/of_irq.h>
#include <linux/of_address.h>

#include <asm/fsp/reg.h>
#include <asm/fsp2/dcr.h>

#include <linux/fsp/reboot.h>
#include <linux/fsp/pra.h>
#include <linux/fsp/hdcr.h>


#define IRQF_DISABLED 0
extern void flush_dcache_all(void);

u32 *drcr_read_addr_ptr;
u32 *drcr_clear_addr_ptr;


void drcr_clear(u32 clear_bits)
{
	*(drcr_clear_addr_ptr) = clear_bits;
}
EXPORT_SYMBOL(drcr_clear);

u32 drcr_read(void)
{
	return *(drcr_read_addr_ptr);
}
EXPORT_SYMBOL(drcr_read);


u32 *disr_read_addr_ptr;
u32 *disr_clear_addr_ptr;

/*
 * This function 'shadows' the DISR register to some unused UIC bits, so a peer
 * FSP can read them via buddy JTAG.  This functionality and the bit
 * definitions are defined in the FSP-1 Reboot and Reset/Reload High Level
 * Design Document, owned by Paul Movall.
 */

#ifdef CONFIG_FSP1

#define DCRN_UIC_DISR_BASE	0x360 /* UIC 1.3 */
#define UIC_DISR_SHADOW_MASK	0xEC000000

#elif defined(CONFIG_FSP2)

/*
 * TODO: This needs architectural definition.  I just randomly picked some
 * free UIC bits for now.
 */
#define DCRN_UIC_DISR_BASE	0x370 /* UIC 1.5 */
#define UIC_DISR_SHADOW_MASK	0x000000EC

#endif

#define DCRN_UIC_DISR_SR	(DCRN_UIC_DISR_BASE + 0)
#define DCRN_UIC_DISR_SRS	(DCRN_UIC_DISR_BASE + 1)
#define DCRN_UIC_DISR_ER	(DCRN_UIC_DISR_BASE + 2)
#define DCRN_UIC_DISR_PR	(DCRN_UIC_DISR_BASE + 4)    /* we store DISR here */

static void shadow_disr_to_uic15(u32 disr)
{
	u32 uic15_er;
	u32 uic15_pl;
	u32 uic15_sr_sdwbits;
	unsigned long flags;

	local_irq_save(flags);

	/* make sure the shadow bits aren't irq enabled */
	uic15_er = ~UIC_DISR_SHADOW_MASK & mfdcr(DCRN_UIC_DISR_ER);
	mtdcr(DCRN_UIC_DISR_ER, uic15_er);

	/* clear the UIC shadow bits */
	mtdcr(DCRN_UIC_DISR_SR, UIC_DISR_SHADOW_MASK);

	/* set the shadow bits as defined in Paul M's reset spec */
#if defined(CONFIG_FSP1)
	uic15_sr_sdwbits =
		((disr << 31) & 0x80000000) |
		((disr << 23) & 0x40000000) |
		((disr << 24) & 0x20000000) |
		((disr << 21) & 0x08000000) |
		((disr << 18) & 0x04000000);
#elif defined(CONFIG_FSP2)
	uic15_sr_sdwbits =
		((disr << 7) & 0x00000080) |
		((disr >> 1) & 0x00000040) |
		((disr     ) & 0x00000020) |
		((disr >> 3) & 0x00000008) |
		((disr >> 6) & 0x00000004);
#endif

	/* we only use the Polrity Register as setting in the status is giving us trouble*/
	/* read Polarity Register and clear the shadow bits*/
	uic15_pl=mfdcr(DCRN_UIC_DISR_PR) & ~UIC_DISR_SHADOW_MASK;
	/* now add the disr part */
	uic15_pl=uic15_pl | uic15_sr_sdwbits;

	mtdcr(DCRN_UIC_DISR_PR, uic15_pl);
	
	printk("shadow_disr_to_uic15: 0x%08lx to uic15_pr\n",uic15_pl);

	local_irq_restore(flags);
}

void disr_set(u32 set_bits)
{
	*(disr_read_addr_ptr) = set_bits;
	shadow_disr_to_uic15(*disr_read_addr_ptr);
}

void disr_clear(u32 clear_bits)
{
	*(disr_clear_addr_ptr) = clear_bits;
	shadow_disr_to_uic15(*disr_read_addr_ptr);
}

u32 disr_read(void)
{
	return *(disr_read_addr_ptr);
}
EXPORT_SYMBOL(disr_read);

void disr_clear_and_set(u32 clear_bits, u32 set_bits, u32 *old_value)
{
	u32 temp;
	unsigned long flags;

	local_irq_save(flags);

	if (old_value) {
		temp = disr_read();
		*old_value = temp;
	}

	disr_set(set_bits);
	disr_clear(clear_bits);

	local_irq_restore(flags);
}
EXPORT_SYMBOL(disr_clear_and_set);


#define DRCR_RESERVED 	   0x0000FF00
#define DRCR_VALID_BIT     0x00008000
#define DRCR_TERM1_BIT	   0x00004000
#define DRCR_TERM2_BIT     0x00000100
#define DRCR_TERM_MASK1    (DRCR_VALID_BIT | DRCR_TERM1_BIT)
#define DRCR_TERM_MASK2    (DRCR_VALID_BIT | DRCR_TERM2_BIT)
#define DRCR_TERM_BITMASK  (DRCR_VALID_BIT | DRCR_TERM1_BIT | DRCR_TERM2_BIT)

static int drcr_is_irq_valid(void)
{
	u32 drcr_reg = drcr_read();
	u32 term_bits = drcr_reg & DRCR_TERM_BITMASK;

	/* Check for valid type */
	if ((term_bits != DRCR_TERM_MASK1) &&
	    (term_bits != DRCR_TERM_MASK2)) {
		/* Invalid bit combo, clear and return */
		drcr_clear(DRCR_TERM1_BIT | DRCR_TERM2_BIT);
		if(!(drcr_reg & ~DRCR_TERM_BITMASK & DRCR_RESERVED)) {
			drcr_clear(DRCR_VALID_BIT);
		}
		return 0;
	}

	return 1;
}

/*
 * Handle the warning from the host that they are about to reset the FSP.
 */
static irqreturn_t drcr_irq_handler(int irq, void *dontcare)
{
	int i;
	char *buf = "host initiated reset";

	if(!drcr_is_irq_valid()) return IRQ_NONE;

	printk("Received host initiated reset request\n");
	show_regs(get_irq_regs());

	mtspr(SPRN_TSR, TSR_ENW | TSR_WIS); /* clear watchdog */

	//pra_write(PRA_SFT_RST_FLAVOR, LINUX_REBOOT_CMD_HOSTINIT);
	//flush_dcache_all();

	/* Do not clear the DRCR register.  This signals to the host that it is
	 * safe to proceed with the reset.  We want to defer that until after
	 * the capture kernel has had a chance to run. */
	/* drcr_clear(0x0000ff00); */

	mtspr(SPRN_TSR, TSR_ENW | TSR_WIS); /* clear watchdog */

	panic("DRCR: Host Initiated Reset");
}

static int fsp_hdcr_of_probe(struct platform_device *ofdev)
{
	int rc;
	struct resource res;
	struct device_node *node = ofdev->dev.of_node;
	unsigned int size;
	unsigned int irq;
	struct irq_desc *desc;
	static int drcr_dev_id; /* unused, but required by request_irq when
				   combining IRQF_DISABLED and IRQF_SHARED */
				   
	
	if (of_address_to_resource(node, 0, &res))
		panic("hdcr_init: of_address_to_resource failure");

	size = res.end - res.start + 1;
	if (size != 16)
		panic("hdcr_init: hdcr size of %u not supported", size);

	drcr_read_addr_ptr = ioremap(res.start, size);

	if (!drcr_read_addr_ptr)
		panic("hdcr_init: of_iomap failure");

	drcr_clear_addr_ptr = drcr_read_addr_ptr + 1;
	disr_read_addr_ptr = drcr_read_addr_ptr + 2;
	disr_clear_addr_ptr = drcr_read_addr_ptr + 3;

	shadow_disr_to_uic15(disr_read());
	
	irq = irq_of_parse_and_map(node, 0);
	if (irq == NO_IRQ)
		panic("Device tree node %s is missing a valid interrupt",
		      node->name);

	/* Clear out spurious DRCR interrupts prior to enablement (capture
	 * kernel only).
	 *
	 * UIC initialization within the uic_init_one function causes all
	 * interrupts to be 'level' type interrupts for a period of time until
	 * further parsing of the device tree changes some interrupts into
	 * 'edge' type interrupts.
	 *
	 * The DRCR write interrupt is an 'edge' type interrupt.  Since we now
	 * defer clearing the DRCR register until after the capture kernel has
	 * collected the kdump, this causes a spurious interrupt to be
	 * recorded during the initialization of the capture kernel when the
	 * interrupt line toggles briefly from an edge to a level interrupt.
	 *
	 * Failure to clear this causes the DRCR IRQ handler to fire and a
	 * subsequent panic.
	 */
	if(is_kdump_kernel()) {
		desc = irq_to_desc(irq);
		if(desc && desc->irq_data.chip && desc->irq_data.chip->irq_ack)
			desc->irq_data.chip->irq_ack(&desc->irq_data);
	}

	rc = request_irq(irq, drcr_irq_handler, IRQF_SHARED | IRQF_DISABLED,
			  "DRCR host-initiated reset", &drcr_dev_id);
	if (rc)
		panic("Error registering DRCR irq handler");

	return 0;
}

static struct of_device_id fsp_hdcr_match_table[] = {
	{ .compatible	= "ibm,hdcr", },
	{},
};

static struct platform_driver fsp_hdcr_of_driver = {
	.driver = {
		.name		= "hdcr",
		.of_match_table	= fsp_hdcr_match_table,
		},
	.probe		= fsp_hdcr_of_probe,
};

static int __init fsp_hdcr_init(void)
{
	return platform_driver_register(&fsp_hdcr_of_driver);
}
arch_initcall(fsp_hdcr_init);
