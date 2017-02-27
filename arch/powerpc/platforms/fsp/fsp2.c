/*
 * FSP-2 board specific routines
 *
 * Copyright 2010-2011, IBM Corporation.
 *
 * Based on earlier code:
 *    Matt Porter <mporter@kernel.crashing.org>
 *    Copyright 2002-2005 MontaVista Software Inc.
 *
 *    Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *    Copyright (c) 2003-2005 Zultys Technologies
 *
 *    Rewritten and ported to the merged powerpc tree:
 *    Copyright 2007 David Gibson <dwg@au1.ibm.com>, IBM Corporation.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/rtc.h>
#include <linux/mmc/arasan_plat.h>

#include <asm/machdep.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/time.h>
#include <asm/uic.h>
#include <asm/ppc4xx.h>

#include "fsp.h"

static __initdata struct of_device_id fsp2_of_bus[] = {
	{ .compatible = "ibm,plb4", },
	{ .compatible = "ibm,plb6", },
	{ .compatible = "ibm,opb", },
	{},
};

static int __init fsp2_device_probe(void)
{
	of_platform_bus_probe(NULL, fsp2_of_bus, NULL);

	return 0;
}
machine_device_initcall(fsp2, fsp2_device_probe);

static int arasan_of_probe(struct platform_device * of_dev)
{
	struct platform_device * arasan_device = NULL;
        struct resource res[2] = {0,};
	static struct arasan_platform_data arasan_pdata = {0,};
	int rc = 0;

	rc = of_address_to_resource(of_dev->dev.of_node, 0, res);
	if (rc) {
		dev_err(&of_dev->dev, "%s resource error!\n",
			of_dev->dev.of_node->full_name);
		rc = -ENXIO;
		goto error;
	}

	res[1].start = irq_of_parse_and_map(of_dev->dev.of_node, 0);
	if (res[1].start == NO_IRQ) {
		dev_err(&of_dev->dev, "%s resource error!\n",
			of_dev->dev.of_node->full_name);
		rc = -ENXIO;
		goto error;
	}

	res[1].name = "mmcirq";
	res[1].flags = IORESOURCE_IRQ;

        arasan_device = platform_device_alloc("arasan", 0);
	if (arasan_device == NULL) {
		rc = -ENOMEM;
		goto error;
	}

	rc = platform_device_add_resources(arasan_device, res, 2);
	if (rc) {
		goto error;
	}

	rc = platform_device_add_data(arasan_device,
				      &arasan_pdata, sizeof(arasan_pdata));
	if (rc) {
		goto error;
	}

	rc = platform_device_add(arasan_device);
	if (rc) {
		goto error;
	}

	if (rc) {
error:
		platform_device_put(arasan_device);
	}

	return rc;
}

static struct of_device_id arasan_match_table[] = {
	{ .compatible	= "arasan,emmc", },
	{},
};

static struct platform_driver arasan_of_driver = {
	.driver = {
		.name		= "arasan-mmc",
		.of_match_table	= arasan_match_table,
	},
	.probe		= arasan_of_probe,
};

static int __init arasan_device_init(void)
{
	return platform_driver_register(&arasan_of_driver);
}
machine_device_initcall(fsp2, arasan_device_init);

/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init fsp2_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (!of_flat_dt_is_compatible(root, "ibm,fsp2"))
		return 0;

	return 1;
}

define_machine(fsp2) {
	.name			= "FSP-2",
	.probe			= fsp2_probe,
	.progress		= udbg_progress,
	.init_IRQ		= uic_init_tree,
	.get_irq		= uic_get_irq,
	.restart		= fsp_restart,
	.panic			= fsp_panic,
//	.power_off		= fsp_power_off,
	.halt			= fsp_halt,
	.machine_check_exception= fsp_machine_check_exception,
	.calibrate_decr		= generic_calibrate_decr,
	//TODO: time_init, set_rtc_time, get_rtc_time, others?
};
