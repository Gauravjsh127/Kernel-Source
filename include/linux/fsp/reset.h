/*
 * Copyright (c) International Business Machines Corp., 2012, 2013
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _FSP_RESET_H_
#define _FSP_RESET_H_

#define DBSR_IDE        0x80000000      /* Imprecise Debug Event */
#define DBSR_UDE        0x40000000      /* Unconditional Debug Event */
#define DBSR_MRR        0x30000000      /* MRR Mask */
#define DBSR_MRR_SYSTEM 0x30000000      /* MRR System */
#define DBSR_MRR_CHIP   0x20000000      /* MRR Chip */
#define DBSR_MRR_CORE   0x10000000      /* MRR Core */
#define DBSR_MRR_NONE   0x00000000      /* MRR None */
#define DBSR_ICMP       0x08000000      /* Instruction Compl Debug Event */
#define DBSR_BRT        0x04000000      /* Branch Taken Debug Event */


/**
 * Specified reset originators.
 */
typedef enum {
	RST_ORIG_UNKNOWN = 0,
	RST_ORIG_CORE,
	RST_ORIG_ASIC,
	RST_ORIG_POWER,
	RST_ORIG_EXTERNAL,
} rst_origin_t;


/**
 * Specified reset types.
 */
typedef enum {
	RST_TYPE_UNKNOWN = 0,
	RST_TYPE_CORE,
	RST_TYPE_CHIP,
	RST_TYPE_SYSTEM,
} rst_type_t;


/**
 * Specified reset reasons.
 */
typedef enum {
	RST_REASON_UNKNOWN,
	RST_REASON_CORE_WATCHDOG,
	RST_REASON_RESTART,
	RST_REASON_PANIC,
	RST_REASON_HALT,
	RST_REASON_POWER_OFF,
	RST_REASON_POWER_CYCLE,
	RST_REASON_PINHOLE,
	RST_REASON_PARPORT,		/* No longer used */
	RST_REASON_BOOTCODE,
	RST_REASON_HOST_INITIATED,
	RST_REASON_BOOT_WATCHDOG,
	RST_REASON_UNIT_CHECK_TIMER,	/* No longer used */
	RST_REASON_EXT_TOOL,
	RST_REASON_PABEND,
	RST_REASON_CHASSIS,
	RST_REASON_PCIE,
} rst_reason_t;

extern struct atomic_notifier_head pre_kdump_notifier_list;

#endif /* _FSP_RESET_H_ */
