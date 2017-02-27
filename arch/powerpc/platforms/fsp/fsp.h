/*
 *   Copyright (c) International Business Machines Corp., 2012
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
#ifndef _PLATFORMS_FSP_FSP_H_
#define _PLATFORMS_FSP_FSP_H_

#include <linux/interrupt.h>

extern void fsp_restart(char *);
extern void fsp_panic(char *);
extern void fsp_power_off(void);
extern void fsp_halt(void);

extern int fsp_machine_check_exception(struct pt_regs *);

#if defined(CONFIG_FSP1)

extern void show_eddr2_regs(void);

#elif defined(CONFIG_FSP2)

extern irqreturn_t reset_warning_irq(int irq, void *data);

#endif

#endif
