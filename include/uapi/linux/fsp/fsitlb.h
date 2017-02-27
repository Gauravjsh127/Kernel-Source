/*
 * Copyright (c) International Business Machines Corp., 2011
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
#ifndef _LINUX_FSP_FSITLB_H_
#define _LINUX_FSP_FSITLB_H_

#warning fsitlb.h is deprecated

#ifdef CONFIG_FSP_FSITLB
#ifndef CONFIG_FSP1
#error only FSP-1 is supported
#endif
extern void fsitlb_write_enable(unsigned long address);
extern void fsitlb_write_disable(unsigned long address);
extern void fsitlb_read_enable(unsigned long address);
extern void fsitlb_read_disable(unsigned long address);
extern void fsitlb_readwrite_enable(unsigned long address);
extern void fsitlb_readwrite_disable(unsigned long address);
#else
#define fsitlb_write_enable(a)
#define fsitlb_write_disable(a)
#define fsitlb_read_enable(a)
#define fsitlb_read_disable(a)
#define fsitlb_readwrite_enable(a)
#define fsitlb_readwrite_disable(a)
#endif

#endif
