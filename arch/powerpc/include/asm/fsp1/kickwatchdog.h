/*
 * Copyright (c) International Business Machines Corp., 2006, 2010
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

#ifndef __FSP1_KICKWATCHDOG_H__
#define __FSP1_KICKWATCHDOG_H__

#include <asm/processor.h>

#ifdef CONFIG_FSP1
#define kick_watchdog()					\
	do {						\
		mtspr(SPRN_TSR, TSR_ENW | TSR_WIS);	\
	} while (0)
	//use this instead.  drivers/watchdog/booge_wdt.c
	//static void booke_wdt_ping(void)
#else
#define kick_watchdog() do { } while (0)
#endif

#endif
