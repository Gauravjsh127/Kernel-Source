/*
 * Copyright (c) International Business Machines Corp., 2006, 2012
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

#ifdef CONFIG_FSP1
#include <asm/fsp1/eddr2.h>
#include <asm/fsp1/dcr.h>
#endif

#ifdef CONFIG_FSP2
#include <asm/fsp2/dcr.h>
#endif

/* FSP PVR values. */
#define PVR_FSP1_PASS0  0x114218d0
#define PVR_FSP1_PASS20 0x114218d1
#define PVR_FSP1_PASS30 0x114218d2

#define PVR_FSP2	0x7ff520c0

/* These SPRs are missing from reg_booke.h */
#define SPRN_RMPD	0x339	/* Real Mode Page Description Register */
#define SPRN_MCSRC	0x33C	/* MCSR Clear */
#define SPRN_DCESR	0x352	/* Data Cache Exception Syndrome Register */
#define SPRN_ICESR	0x353	/* I-Cache Exception Syndrome Register */
#define SPRN_CCR2	0x379	/* Core Configuration Register 2 */
#define SPRN_DCRIPR	0x37B	/* DCR Immediate Prefix Register */
#define SPRN_DBDR	0x3F3	/* Debug Data Register */

#define SPRN_DCDBTRH    0x39D   /* Data Cache Debug Tag Register High */
#define SPRN_DCDBTRL    0x39C   /* Data Cache Debug Tag Register Low */
#define SPRN_ICDBDR0    0x3D3   /* Instruction Cache Debug Data Register 0 */
#define SPRN_ICDBDR1    0x3D4   /* Instruction Cache Debug Data Register 1 */
#define SPRN_ICDBTRH    0x39F   /* Instruction Cache Debug Tag Register High */
#define SPRN_ICDBTRL    0x39E   /* Instruction Cache Debug Tag Register Low */
#define SPRN_IOCCR      0x35C   /* Instruction Opcode Compare Control Register */
#define SPRN_IOCR1      0x35D   /* Instruction Opcode Compare Register 1 */
#define SPRN_IOCR2      0x35E   /* Instruction Opcode Compare Register 2 */
#define SPRN_PMUCC0     0x35A   /* PMU Core Control Register */
#define SPRN_PWM        0x376   /* Pulse Width Margin Register */
#define SPRN_RSTCFG     0x39B   /* Reset Configuration Register */
#define SPRN_USPGR0     0x100   /* User SPR General 0 */

/* These MCSR bits are missing from reg_booke.h */
#define MCSR_L2		0x00200000	/* Error reported through L2 */
#define MCSR_DCR	0x00100000	/* DCR timeout */
