/*
 * Author: Giuseppe Cavallaro <peppe.cavall...@st.com>
 *
 * include/linux/mmc/arsan_plat.h
 *
 * platform data for the Arasan MMC/SD/SDI HC driver
 *
 * Copyright (C) 2010  STMicroelectronics Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */

#ifndef __ARASAN_PLAT_H__
#define __ARASAN_PLAT_H__

struct arasan_platform_data {
       unsigned int need_poll;
#ifdef CONFIG_STM_DRIVERS
       struct stm_pad_config *pad_config;
#endif
};

/* ARASAN Resource configuration */
#ifdef CONFIG_STM_DRIVERS
#include <linux/stm/platform.h>
#include <linux/stm/pad.h>
static inline int arasan_claim_resource(struct platform_device *pdev)
{
       int ret = 0;
       struct arasan_platform_data *plat_dat = pdev->dev.platform_data;

       /* Pad routing setup required on STM platforms */
       if (!devm_stm_pad_claim(&pdev->dev, plat_dat->pad_config,
                               dev_name(&pdev->dev))) {
               pr_err("%s: Failed to request pads!\n", __func__);
               ret = -ENODEV;
       }
       return ret;
}
#else
static inline int arasan_claim_resource(struct platform_device *pdev)
{
       return 0;
}
#endif
#endif
