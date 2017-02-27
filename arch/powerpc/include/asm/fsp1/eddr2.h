/*
 *   Copyright (c) International Business Machines Corp., 2005, 2012
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;	without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *   Author: Frank Haverkamp <haverkam@de.ibm.com>
 *
 ******************************************************************************
 *
 * eDDR2 Controller with Write Protection Support
 *
 *	MSB ------------------------ LSB
 *		    11.1111.1111.2222.2222.2233
 *	0123.4567.8901.2345.6789.0123.4567.8901
 *
 */
#ifndef _ASM_POWERPC_FSP1_EDDR2_H_
#define _ASM_POWERPC_FSP1_EDDR2_H_

#include <asm/reg.h>

#define EDDR2_BESRDC	0x00 /* PLB Bus Error Status   Clear R/W x0000_0000 */
#define EDDR2_BESRDS	0x01 /* PLB Bus Error Status	 Set		    */
#define EDDR2_BEARD	0x02 /* PLB Bus Error Address	     R/W x0000_0000 */
#define EDDR2_WPESRC	0x03 /* WProtect Array Err Syn Clear R/W x0000_0000 */
#define EDDR2_WPESRS	0x04 /* WProtect Array Err Syn	 Set		    */
#define EDDR2_WPEAR	0x05 /* Wrotect Array Error Addr     R/W x0000_0000 */
#define EDDR2_WMIRQC	0x06 /* PLB Bus Master Inter   Clear R/W x0000_0000 */
#define EDDR2_WMIRQS	0x07 /* PLB Bus Master Inter	 Set		    */
#define EDDR2_PLBOPT	0x08 /* PLB Slave Interface Options  R/W x0000_0000 */
#define EDDR2_PUABA	0x09 /* PLB Upper Address Base Addr  R/W x0000_0000 */
#define EDDR2_WPAIC	0x10 /* WProtect Array Interf Config R/W x0000_0000 */
#define EDDR2_WPIRQC	0x11 /* WProtection Interrupt  Clear R/W x0000_0000 */
#define EDDR2_WPIRQS	0x12 /* WProtection Interrupt	 Set		    */
#define EDDR2_WPAPE	0x13 /* WProtect Array Parity Enable R/W x0000_0000 */
#define EDDR2_MCSTAT	0x14 /* Controller Status	    Read x6000_0000 */
#define EDDR2_MCOPT1	0x20 /* Memory Controller Options 1  R/W x1102_0000 */
#define EDDR2_MCOPT2	0x21 /* Memory Controller Options 2  R/W x8400_0000 */
#define EDDR2_MODT0	0x22 /* On Die Term for Rank 0	     R/W x0000_0000 */
#define EDDR2_MODT1	0x23 /* On Die Term for Rank 1	     R/W x0000_0000 */
#define EDDR2_CODT	0x26 /* On Die Termination for Contr R/W x0000_0000 */
#define EDDR2_VVPR	0x27 /* Variable VRef Progr Reg	     R/W x0000_0000 */
#define EDDR2_OPARS	0x28 /* On Chip Driver Contr Setup   R/W x8000_0037 */
#define EDDR2_OPART	0x29 /* On Chip Driver Contr Trigger R/W x0000_0000 */
#define EDDR2_RTR	0x30 /* Refresh Timer Register	     R/W x0BE0_0000 */
#define EDDR2_MR0CF	0x40 /* Memory Rank 0 Configuration  R/W x0000_0000 */
#define EDDR2_MR0EA	0x41 /* Memory Rank 0 End Address   Read x0000_0000 */
#define EDDR2_MR1CF	0x44 /* Memory Rank 1 Configuration  R/W x0000_0000 */
#define EDDR2_MR1EA	0x45 /* Memory Rank 1 End Address   Read x0000_0000 */
#define EDDR2_TOM	0x46 /* Top Of Memory x46	 Read x0000_0000 IN */
#define EDDR2_INITPLR0	0x50 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR1	0x51 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR2	0x52 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR3	0x53 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR4	0x54 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR5	0x55 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR6	0x56 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR7	0x57 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR8	0x58 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR9	0x59 /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR10 0x5A /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR11 0x5B /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR12 0x5C /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR13 0x5D /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR14 0x5E /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_INITPLR15 0x5F /* Manual Init Control Reg	     R/W x0000_0000 */
#define EDDR2_RQDC	0x70 /* Read DQS Delay Control	     R/W x0000_0000 */
#define EDDR2_RFDC	0x74 /* Read Feedback Delay Control  R/W x0000_0000 */
#define EDDR2_RDCC	0x78 /* Read Data Capture Control    R/W x0000_0000 */
#define EDDR2_DLCR	0x7A /* Delay Line Calibration Reg   R/W autoinit   */
#define EDDR2_CLKTR	0x80 /* DDR Clock Timing Register    R/W x0000_0000 */
#define EDDR2_WRDTR	0x81 /* WData, DQS, DM ClkTiming Reg R/W x8600_0800 */
#define EDDR2_SDTR1	0x85 /* DDR SDRAM Timing Register 1  R/W x8020_1000 */
#define EDDR2_SDTR2	0x86 /* DDR SDRAM Timing Register 2  R/W x4210_4442 */
#define EDDR2_SDTR3	0x87 /* DDR SDRAM Timing Register 3  R/W x1010_0C10 */
#define EDDR2_MMODE	0x88 /* Memory Mode Register	     R/W x0000_0042 */
#define EDDR2_MEMODE	0x89 /* Memory Extended Mode Reg     R/W x0000_0400 */
#define EDDR2_ECCES	0x98 /* ECC Error Status	     R/W x0000_0000 */
#define EDDR2_ECCCEA	0x9D /* ECC Correctable Error Addr  Read x0000_0000 */
#define EDDR2_SIECR	0xA0 /* Scrub/Init Engine Config     R/W x0000_0000 */
#define EDDR2_SIESA	0xA1 /* Scrub/Init Engine Start Addr R/W x0000_0000 */
#define EDDR2_SIEEA	0xA2 /* Scrub/Init Engine End Addr   R/W x0000_0000 */
#define EDDR2_SEUEA	0xA3 /* Scrub Engine UE Address	    Read x0000_0000 */
#define EDDR2_SEUES	0xA4 /* Scrub Engine UE Status	     R/W x0000_0000 */
#define EDDR2_SECEA	0xA5 /* Scrub Engine CE Address	    Read x0000_0000 */
#define EDDR2_SECES	0xA6 /* Scrub Engine CE Status	     R/W x0000_0000 */
#define EDDR2_SEES0	0xA7 /* Scrub Engine Err Stat  Clear R/W x0000_0000 */
#define EDDR2_SEES1	0xA8 /* Scrub Engine Err Stat	 Set		    */
#define EDDR2_SFPM	0xA9 /* Scrub/Init Eng Fill Pat/Mask R/W x0000_0000 */
#define EDDR2_SIECA	0xAA /* Scrub/Init Engine Curr Addr Read x0000_0000 */
#define EDDR2_FCSR	0xB0 /* Feedback Calibration Status Read x0000_0000 */
#define EDDR2_RTSR	0xB1 /* Runtime Tracking Status	     R/W xFFFF_FF00 */
#define EDDR2_CID	0xF4 /* Core ID Register	    Read x0DD2_0016 */
#define EDDR2_RID	0xF8 /* Revision ID Register	    Read xFFFF_FFFF */

/******************************************************************************
 *
 * Bus Error Syndrome Register (BESR)
 *
 *****************************************************************************/

#define BESR_MxID	      0xF0000000 /* PLB MasterID 0-C */
#define BESR_MxET	      0x0E000000 /* Indicat. of Err Detect. and Type */
#define	  BESR_MxET_NOERR     0x00000000 /* 000 No Error */
#define	  BESR_MxET_ECC	      0x02000000 /* 001 ECC Error */
#define	  BESR_MxET_PROT      0x04000000 /* 010 Protect Error (Write only) */
#define BESR_MxRW	      0x01000000 /* Operation Causing Err 0: W, 1: R */
#define BESR_RES	      0x00FFFFFF /* Reserved */

/******************************************************************************
 *
 * Write Protect Array Error Syndrome Register (WPESR)
 *
 *****************************************************************************/

#define WPESR_MxID	      0xF0000000 /* PLB MasterID 0-C */
#define WPESR_MxET	      0x0C000000 /* Indicat. of Err Detect. and Type */
#define	  WPESR_MxET_NOERR    0x00000000 /* 00 No Error */
#define	  WPESR_MxET_WROK     0x04000000 /* 01 Parity Err PLB2Mem Write ok */
#define	  WPESR_MxET_WRNOK    0x08000000 /* 10 Parity Err PLB2Mem Write nok */
#define	  WPESR_MxET_PARITY   0x0C000000 /* 11 Parity Err Detected */
#define WPESR_MxRW	      0x02000000 /* Operation Causing Err
					  * 0 - Write (PLB to Memory)
					  * 1 - Read (DCR to WPAI) */
#define WPESR_RES	      0x01FFFFFF /* Reserved */

/******************************************************************************
 *
 * PLB Master Write Interrupt (WMIRQ)
 *
 *****************************************************************************/

#define WMIRQ_M0_IRQ	      0x80000000 /* Master 0 Write Irq 1 - WriteErr */
#define WMIRQ_M1_IRQ	      0x40000000 /* Master 1 Write Irq 1 - WriteErr */
#define WMIRQ_M2_IRQ	      0x20000000 /* Master 2 Write Irq 1 - WriteErr */
#define WMIRQ_M3_IRQ	      0x10000000 /* Master 3 Write Irq 1 - WriteErr */
#define WMIRQ_M4_IRQ	      0x08000000 /* Master 4 Write Irq 1 - WriteErr */
#define WMIRQ_M5_IRQ	      0x04000000 /* Master 5 Write Irq 1 - WriteErr */
#define WMIRQ_M6_IRQ	      0x02000000 /* Master 6 Write Irq 1 - WriteErr */
#define WMIRQ_M7_IRQ	      0x01000000 /* Master 7 Write Irq 1 - WriteErr */
#define WMIRQ_M8_IRQ	      0x00800000 /* Master 8 Write Irq 1 - WriteErr */
#define WMIRQ_M9_IRQ	      0x00400000 /* Master 9 Write Irq 1 - WriteErr */
#define WMIRQ_M10_IRQ	      0x00200000 /* Master 10 Write Irq 1 - WriteErr */
#define WMIRQ_M11_IRQ	      0x00100000 /* Master 11 Write Irq 1 - WriteErr */
#define WMIRQ_RES	      0x000FFFFF /* Reserved */

/******************************************************************************
 *
 * PLB Slave Interface Options (PLBOPT)
 *
 *****************************************************************************/

#define PLBOPT_WR_RE	      0x80000000 /* PLB Slave Write Rearbitrate En. */
#define PLBOPT_RD_RE	      0x40000000 /* PLB Slave Read Rearbitrate En. */
#define PLBOPT_RARW	      0x20000000 /* PLB Slave Read Around Write En. */
#define PLBOPT_RES	      0x1FFFFFFF /* Reserved */

/******************************************************************************
 *
 * PLB Upper Address Base Address Register (PUABA)
 *
 *****************************************************************************/

#define PUABA_PUABA	      0x0000000F /* Must be set to 0x0 */
#define PUABA_RES	      0xFFFFFFF0 /* Reserved */

/******************************************************************************
 *
 * Write Protect Array Interface Configuration Register (WPAIC)
 *
 *****************************************************************************/

#define WPAIC_EN	      0x80000000 /* Write Protect Interface Enable */
#define WPAIC_PER	      0x40000000 /* Parity Error Response:
					  * 0 - PLB to Mem Write Allowed
					  * 1 - PLB to Mem Write NOT Allowed */
#define WPAIC_RES	      0x3FFFFFFF /* Reserved */

/******************************************************************************
 *
 * Write Protection Interrupt Request (WPIRQ)
 *
 *****************************************************************************/

#define WPIRQ_PR_IRQ	      0x80000000 /* WProt Array Parity Err Intr */
#define WPIRQ_PT_IRQ	      0x40000000 /* WProt Array Protect Err Intr */
#define WPIRQ_RES	      0x30000000 /* Reserved */

/******************************************************************************
 *
 * Write Protection Array Parity Enable Register (WPAPE)
 *
 *****************************************************************************/

#define WPAPE_nWPGS	      0x80000000 /* WPA Write Data Parity Gen/Storage*/
#define WPAPE_nRPC	      0x40000000 /* WPA Read Data Parity Checking */
#define WPAPE_RES	      0x3FFFFFFF /* Reserved */

/******************************************************************************
 *
 * Memory Controller Status (MCSTAT)
 *
 *****************************************************************************/

#define MCSTAT_MIC	      0x80000000 /* Memory Init Complete */
#define MCSTAT_SRMS	      0x40000000 /* Self Refresh Mode Status */
#define MCSTAT_IDLE	      0x20000000 /* Core Idle Status */
#define MCSTAT_RES	      0x1FFFFFFF /* Reserved */

/******************************************************************************
 *
 * Memory Controller Options 1 (MCOPT1)
 *
 *****************************************************************************/

#define MCOPT1_MCHK	      0x30000000 /* Memory Data error checking */
#define	  MCOPT1_MCHK_NOGEN   0x00000000 /* 00 - ECC checking, correction,
					  *	 error rep. (no generation) */
#define	  MCOPT1_MCHK_NORPT   0x10000000 /* 01 - ECC checking, correction,
					  *	 no error reporting */
#define	  MCOPT1_MCHK_ECC     0x20000000 /* 10 - ECC generation only */
#define	  MCOPT1_MCHK_ECCCR   0x30000000 /* 01 - ECC checking, correction,
					  *	 error reporting */
#define MCOPT1_RDEN	      0x08000000 /* Registered DIMM Enable */
#define MCOPT1_PMUM	      0x06000000 /* Page Management Unit Mode */
#define	  MCOPT1_PMUM_CLOSE   0x00000000 /* Close after read/write */
#define	  MCOPT1_PMUM_AUTO    0x02000000 /* Auto-Close */
#define	  MCOPT1_PMUM_OPEN    0x04000000 /* Open */
#define MCOPT1_WDTH	      0x01000000 /* DDR DRAM Interf Width 1: 16bit */
#define MCOPT1_BCNT	      0x00020000 /* Memory Device Bank Count
					  * 0 - 4 Bank Devices
					  * 1 - 8 Bank Devices */
#define MCOPT1_DDR2	      0x00010000 /* DDR2 Select */
#define MCOPT1_QDEP	      0x00030000 /* Depth of Cmd Queue for Pg LAhead */
#define	  MCOPT1_QDEP_4CMD    0x00020000 /* 10 - 4 commands deep */
#define MCOPT1_DCOO	      0x00002000 /* Disable All Out of Order ops. */
#define MCOPT1_DREF	      0x00001000 /* Enable Deferred Refresh */
#define MCOPT1_RES	      0xC0CCCFFF /* Reserved */

/******************************************************************************
 *
 * Memory Controller Options 2 (MCOPT2)
 *
 *****************************************************************************/

#define MCOPT2_SREN	      0x80000000 /* Self Refresh Entry/Exit Control */
#define MCOPT2_IPTR	      0x20000000 /* Initialization Trigger Reg */
#define MCOPT2_XSRP	      0x10000000 /* Exit Self Refresh Prevent */
#define MCOPT2_DCEN	      0x08000000 /* SDRAM Controller Enable */
#define MCOPT2_ISIE	      0x04000000 /* Initialization Seq Interruptable */
#define MCOPT2_RES	      0x43FFFFFF /* Reserved */

#define MAX_WPA_ADDRESS	      0x3FFC /* Highest address for the WPA */

#define SCRUB_SIECR_MODE_MASK			0xE0000000
#define SCRUB_SIECR_MODE_INTERVAL		0x20000000
#define SCRUB_SIECR_MODE_FILL_MEMORY		0x40000000
#define SCRUB_SIECR_MODE_IMMEDIATE_FAST		0x60000000
#define SCRUB_SIECR_MODE_IMMEDIATE_SINGLE	0x80000000
#define SCRUB_SIECR_MODE_INTERVAL_CONTINUOUS	0xA0000000
#define SCRUB_SIECR_SUER			0x10000000
#define SCRUB_SIECR_SCER			0x08000000
#define SCRUB_SIECR_LUES			0x04000000
#define SCRUB_SIECR_LCES			0x02000000
#define SCRUB_SIECR_IFSX			0x01000000
#define SCRUB_SIECR_SRIT_MASK			0x00003FFF


/*
 * Scrub Engine Error Status Register (SEES)
 */
#define SEES_UEI		0x80000000 /* UE Status */
#define SEES_CEI		0x40000000 /* CE Status */
#define SEES_SIES_MASK		0x30000000 /* SIE Status */
#define SEES_SIEOC		0x08000000 /* SIE Operation Complete */

#endif /* _ASM_POWERPC_FSP1_EDDR2_H_ */
