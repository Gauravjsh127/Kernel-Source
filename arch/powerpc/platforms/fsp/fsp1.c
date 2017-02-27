/* FSP1 board specific routines
 *
 * Mike Kobler <mkobler@linux.vnet.ibm.com>
 * Josh Poimboeuf <jpoimboe@us.ibm.com>
 * Copyright 2010 IBM Corporation.
 *
 * Matt Porter <mporter@kernel.crashing.org>
 * Copyright 2002-2005 MontaVista Software Inc.
 *
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 * Copyright (c) 2003-2005 Zultys Technologies
 *
 * Rewritten and ported to the merged powerpc tree:
 * Copyright 2007 David Gibson <dwg@au1.ibm.com>, IBM Corporation.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/rtc.h>

#include <asm/machdep.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/time.h>
#include <asm/uic.h>
#include <asm/pci-bridge.h>
#include <asm/ppc4xx.h>
#include <asm/fsp1/rtc.h>

#include "fsp.h"

static __initdata struct of_device_id fsp1_of_bus[] = {
	{ .compatible = "ibm,plb4", },
	{ .compatible = "ibm,opb", },
	{ .compatible = "ibm,ebc", },
	{},
};

static int __init fsp1_device_probe(void)
{
	of_platform_bus_probe(NULL, fsp1_of_bus, NULL);
	//of_instantiate_rtc();

	return 0;
}
machine_device_initcall(fsp1, fsp1_device_probe);

/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init fsp1_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (!of_flat_dt_is_compatible(root, "ibm,fsp1"))
		return 0;

	return 1;
}


extern long ibmfsp1_time_init(void);

define_machine(fsp1) {
	.name			= "FSP-1",
	.probe			= fsp1_probe,
	.progress		= udbg_progress,
	.init_IRQ		= uic_init_tree,
	.get_irq		= uic_get_irq,
	.restart		= fsp_restart,
	.panic			= fsp_panic,
	.power_off		= fsp_power_off,
	.halt			= fsp_halt,
	.machine_check_exception= fsp_machine_check_exception,
	.calibrate_decr		= generic_calibrate_decr,
	.time_init		= ibmfsp1_time_init,
	.set_rtc_time		= ibmfsp1_set_rtc_time,
	.get_rtc_time		= ibmfsp1_get_rtc_time,
};
