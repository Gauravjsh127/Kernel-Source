/*******************************************************************************
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
 *****************************************************************************/
#ifndef _ASM_POWERPC_FSP_DCR_H_
#define _ASM_POWERPC_FSP_DCR_H_
#ifdef __KERNEL__

#define DCRN_CCL_ADDR		0x006	/* Clock configuration addr */
#define DCRN_CCL_DATA		0x007	/* Clock configuration data */
#define DCRN_DDR_REG_ADDR	0x008
#define DCRN_DDR_REG_DATA 	0x009
#define DCRN_DDR_WPA_ADDR	0x00A
#define DCRN_DDR_WPA_DATA	0x00B
#define DCRN_CMU_ADDR		0x00C	/* Chip management unic addr */
#define DCRN_CMU_DATA		0x00D	/* Chip management unic data */
#define DCRN_EBC_ADDR		0x00E
#define DCRN_EBC_DATA		0x00F


/* PLB Arbiter */
#define DCRN_PLB_PCBI		0x010	/* PLB Crossbar ID/Rev Register */
#define DCRN_PLB_P0ACR		0x011	/* PLB0 Arbiter Control Register */
#define DCRN_PLB_P0ESRL		0x012	/* PLB0 Error Status Register Low */
#define DCRN_PLB_P0ESRH		0x013	/* PLB0 Error Status Register High */
#define DCRN_PLB_P0EARL		0x014	/* PLB0 Error Address Register Low */
#define DCRN_PLB_P0EARH		0x015	/* PLB0 Error Address Register High */
#define DCRN_PLB_P0ESRLS	0x016	/* PLB0 Error Status Register Low Set*/
#define DCRN_PLB_P0ESRHS	0x017	/* PLB0 Error Status Register High */
#define DCRN_PLB_PCBC		0x018	/* PLB Crossbar Control Register */
#define DCRN_PLB_P1ACR		0x019	/* PLB1 Arbiter Control Register */
#define DCRN_PLB_P1ESRL		0x01A	/* PLB1 Error Status Register Low */
#define DCRN_PLB_P1ESRH		0x01B	/* PLB1 Error Status Register High */
#define DCRN_PLB_P1EARL		0x01C	/* PLB1 Error Address Register Low */
#define DCRN_PLB_P1EARH		0x01D	/* PLB1 Error Address Register High */
#define DCRN_PLB_P1ESRLS	0x01E	/* PLB1 Error Status Register Low Set*/
#define DCRN_PLB_P1ESRHS	0x01F	/*PLB1 Error Status Register High Set*/

/* PLB/OPB bridge 0, 1, 2, 3 */
#define DCRN_PLB0_GESR0		0x020	/* Error status 0: Master Dev 0,1,2,3*/
#define DCRN_PLB0_GESRS0	0x021	/* Error Status Register Set 0 */
#define DCRN_PLB0_GEAR		0x022	/* Error Address Register */
#define DCRN_PLB0_GEARU		0x023	/* Error Upper Address Register */
#define DCRN_PLB0_GESR1		0x024	/* Error Status 1; Master Dev 4,5,6,7*/
#define DCRN_PLB0_GESRS1	0x025	/* Error Status Register Set 1 */
#define DCRN_PLB0_GCONFG	0x026	/* Error Configuration Register */
#define DCRN_PLB0_GCONFGS	0x027	/* Error Configuration Register Set */
#define DCRN_PLB0_GLATENCY	0x028	/* Burst Latency Timer */
#define DCRN_PLB0_GLATENCYS	0x029	/* Burst Latency Timer Set */
#define DCRN_PLB0_GREV		0x02A	/* Revision ID */

#define DCRN_PLB1_GESR0		0x030	/* Error status 0: Master Dev 0,1,2,3*/
#define DCRN_PLB1_GESRS0	0x031	/* Error Status Register Set 0 */
#define DCRN_PLB1_GEAR		0x032	/* Error Address Register */
#define DCRN_PLB1_GEARU		0x033	/* Error Upper Address Register */
#define DCRN_PLB1_GESR1		0x034	/* Error Status 1; Master Dev 4,5,6,7*/
#define DCRN_PLB1_GESRS1	0x035	/* Error Status Register Set 1 */
#define DCRN_PLB1_GCONFG	0x036	/* Error Configuration Register */
#define DCRN_PLB1_GCONFGS	0x037	/* Error Configuration Register Set */
#define DCRN_PLB1_GLATENCY	0x038	/* Burst Latency Timer */
#define DCRN_PLB1_GLATENCYS	0x039	/* Burst Latency Timer Set */
#define DCRN_PLB1_GREV		0x03A	/* Revision ID */

#define DCRN_PLB2_GESR0		0x040	/* Error status 0: Master Dev 0,1,2,3*/
#define DCRN_PLB2_GESRS0	0x041	/* Error Status Register Set 0 */
#define DCRN_PLB2_GEAR		0x042	/* Error Address Register */
#define DCRN_PLB2_GEARU		0x043	/* Error Upper Address Register */
#define DCRN_PLB2_GESR1		0x044	/* Error Status 1; Master Dev 4,5,6,7*/
#define DCRN_PLB2_GESRS1	0x045	/* Error Status Register Set 1 */
#define DCRN_PLB2_GCONFG	0x046	/* Error Configuration Register */
#define DCRN_PLB2_GCONFGS	0x047	/* Error Configuration Register Set */
#define DCRN_PLB2_GLATENCY	0x048	/* Burst Latency Timer */
#define DCRN_PLB2_GLATENCYS	0x049	/* Burst Latency Timer Set */
#define DCRN_PLB2_GREV		0x04A	/* Revision ID */

#define DCRN_PLB3_GESR0		0x050	/* Error status 0: Master Dev 0,1,2,3*/
#define DCRN_PLB3_GESRS0	0x051	/* Error Status Register Set 0 */
#define DCRN_PLB3_GEAR		0x052	/* Error Address Register */
#define DCRN_PLB3_GEARU		0x053	/* Error Upper Address Register */
#define DCRN_PLB3_GESR1		0x054	/* Error Status 1; Master Dev 4,5,6,7*/
#define DCRN_PLB3_GESRS1	0x055	/* Error Status Register Set 1 */
#define DCRN_PLB3_GCONFG	0x056	/* Error Configuration Register */
#define DCRN_PLB3_GCONFGS	0x057	/* Error Configuration Register Set */
#define DCRN_PLB3_GLATENCY	0x058	/* Burst Latency Timer */
#define DCRN_PLB3_GLATENCYS	0x059	/* Burst Latency Timer Set */
#define DCRN_PLB3_GREV		0x05A	/* Revision ID */

/* RTC */
#define DCRN_RTC_SECONDS	0x60
#define DCRN_RTC_SEC_ALRM	0x61
#define DCRN_RTC_MINUTES	0x62
#define DCRN_RTC_MIN_ALRM	0x63
#define DCRN_RTC_HOURS		0x64
#define DCRN_RTC_HOURS_ALRM	0x65
#define DCRN_RTC_DAY_OF_WEEK	0x66
#define DCRN_RTC_DAY_OF_MONTH	0x67
#define DCRN_RTC_MONTH		0x68
#define DCRN_RTC_YEAR		0x69
#define DCRN_RTC_REG_A		0x6A
#define DCRN_RTC_REG_B		0x6B
#define DCRN_RTC_REG_C		0x6C
#define DCRN_RTC_REG_D		0x6D
#define DCRN_RTC_CENTURY	0x6E
#define DCRN_RTC_WRAPPER_CNTL	0x70

/* CMU addresses of RTC64 registers */
#define CMUN_RTC64_HI		0x0B
#define CMUN_RTC64_LO		0x0C
#define CMUN_RTC64_ALRM_CNTL	0x0D
#define CMUN_RTC64A0_HI		0x0E
#define CMUN_RTC64A0_LO		0x0F
#define CMUN_RTC64A1_HI		0x10
#define CMUN_RTC64A1_LO		0x11
#define CMUN_RTC64A2_HI		0x12
#define CMUN_RTC64A2_LO		0x13

/* CMU addresses of URCR registers */
#define CMUN_CRCS		0x00 /* Chip Reset Control Status */
#define CMUN_URCR0_RS		0x01
#define CMUN_URCR0_C		0x02
#define CMUN_URCR1_RS		0x03
#define CMUN_URCR1_C		0x04

/* addresses of PSI URCR registers */
#define URCR1_PSI0_MASTER_RESET	0x08000000
#define URCR1_PSI1_MASTER_RESET	0x04000000
#define URCR1_PSI2_MASTER_RESET	0x02000000
#define URCR1_PSI3_MASTER_RESET	0x01000000

/* CMU address for USB configuration */
#define CMUN_USBADRB		0x25

/* CMU address of boot watchdog status and cntl */
#define CMUN_BWTC		0x05

/* CRCS bit fields */
#define CRCS_STAT_MASK		0xF0000000
#define CRCS_STAT_POR		0x10000000
#define CRCS_STAT_ALTERNATE_POR 0x00000000 /* See bug 36786 */
#define CRCS_STAT_PHR		0x20000000
#define CRCS_STAT_CRCS_SYS	0x40000000 /* Write to bit 24 of this reg */
#define CRCS_STAT_DBCR_SYS	0x50000000 /* Check TSR */
#define CRCS_STAT_HOST_SYS	0x60000000
#define CRCS_STAT_CRCS_CHIP	0x80000000 /* Write to bit 25 of this reg */
#define CRCS_STAT_DBCR_CHIP	0x90000000 /* Check TSR */
#define CRCS_STAT_HOST_CHIP	0xA0000000
#define CRCS_STAT_PSI_CHIP	0xB0000000
#define CRCS_STAT_CRCS_CORE	0xC0000000 /* Write to bit 26 of this reg */
#define CRCS_STAT_DBCR_CORE	0xD0000000 /* Check TSR */
#define CRCS_STAT_HOST_CORE	0xE0000000
#define CRCS_STAT_SELF_CORE	0x40000000
#define CRCS_STAT_SELF_CHIP	0x50000000
#define CRCS_WATCHE		0x08000000
#define CRCS_CORE		0x04000000 /* Reset PPC440 core */
#define CRCS_CHIP		0x02000000 /* Chip Reset */
#define CRCS_SYS		0x01000000 /* System Reset */
#define CRCS_WRCR		0x00800000 /* Watchdog reset on core reset */
#define CRCS_PLOCK		0x00000002 /* PLL Locked */

/* CMU_BWTC bit fields */
#define BWTC_CONFIG0		0x80000000 /* BWD enable pin */
#define BWTC_CONFIG1		0x40000000 /* BWD mode pin 0:ADDR 1:CS */
#define BWTC_CONFIG4		0x20000000 /* Boot Fash size */
#define BWTC_CONFIG3		0x10000000 /* Boot Fash size */
#define BWTC_CONFIG2		0x08000000 /* Boot Fash size */
#define BWTC_NANDBOOT		0x04000000 /* NANDBOOT */
#define BWTC_CONFIG1LT		0x00008000 /* Latched version of BWD mode */
#define BWTC_CONFIG4LT		0x00004000 /* Latched CONFIG4 */
#define BWTC_CONFIG3LT		0x00002000 /* Latched CONFIG3 */
#define BWTC_CONFIG2LT		0x00001000 /* Latched CONFIG2 */
#define BWTC_WDT_MASK		0x00000700 /* WDT Mask */
#define BWTC_WDT_500US		0x00000000 /* 500 usec */
#define BWTC_WDT_1MS		0x00000100 /* 1 msec   */
#define BWTC_WDT_250MS		0x00000200 /* 250 msec */
#define BWTC_WDT_500MS		0x00000300 /* 500 msec */
#define BWTC_WDT_1SEC		0x00000400 /* 1 sec    */
#define BWTC_WDT_8SEC		0x00000500 /* 8 sec    */
#define BWTC_WDT_16SEC		0x00000600 /* 16 sec   */
#define BWTC_WDT_32SEC		0x00000700 /* 32 sec   */
#define BWTC_BANK_MSB		0x00000008 /* Dual Boot Bank Sel bit 2 (R) */
#define BWTC_MASK		0x00000004 /* Mask dual boot bank select */
#define BWTC_BANK		0x00000002 /* Dual Boot Bank Sel (R) */
#define BWTC_BANK_LSB		0x00000002 /* Dual Boot Bank Sel (R) */
#define BWTC_WDTE		0x00000001 /* Watchdog Enable (RC)   */

/* MCMAL0 */
/* MCMAL1 */

/* FSI0 DMA */
/* FSI1 DMA */
/* FSI2 DMA */
/* FSI3 DMA */
/* PSI DMA */
/*Define the various engine base addresses*/
#define DMA_FSI_ENG0	0x180
#define DMA_FSI_ENG1	0x1C0
#define DMA_FSI_ENG2	0x200
#define DMA_FSI_ENG3	0x240
#define DMA_PSI_ENG	0x280

/*Define the "base" DMA DCR regs, OR these into the base to
 get the channel you desire  ie FSI_ENG0 Channel CR 0 is (DMA_FSI_ENG0 | DCRN_DMA_CR0) */
#define DCRN_DMA_CR0(base)	(base + 0x0)	/* Channel Control Register 0*/
#define DCRN_DMA_CTC0(base)	(base + 0x1)	/* Count and Control Register 0 */
#define DCRN_DMA_SAH0(base)	(base + 0x2)	/* Source Address High Register 0*/
#define DCRN_DMA_SAL0(base)	(base + 0x3)	/* Source Address Low Register 0*/
#define DCRN_DMA_DAH0(base)	(base + 0x4)	/* Dest Addr High Register 0 */
#define DCRN_DMA_DAL0(base)	(base + 0x5)	/* Dest Addr Low Register 0 */
#define DCRN_DMA_SGH0(base)	(base + 0x6)	/* S/G Desc Addr High Register 0 */
#define DCRN_DMA_SGL0(base)	(base + 0x7)	/* S/G Desc Addr Low Register 0 */
#define DCRN_DMA_CR1(base)	(base + 0x8)	/* Channel Control Register 1*/
#define DCRN_DMA_CTC1(base)	(base + 0x9)	/* Count and Control Register 1 */
#define DCRN_DMA_SAH1(base)	(base + 0xA)	/* Source Address High Register 1*/
#define DCRN_DMA_SAL1(base)	(base + 0xB)	/* Source Address Low Register 1*/
#define DCRN_DMA_DAH1(base)	(base + 0xC)	/* Dest Addr High Register 1 */
#define DCRN_DMA_DAL1(base)	(base + 0xD)	/* Dest Addr Low Register 1 */
#define DCRN_DMA_SGH1(base)	(base + 0xE)	/* S/G Desc Addr High Register 1 */
#define DCRN_DMA_SGL1(base)	(base + 0xF)	/* S/G Desc Addr Low Register 1 */
#define DCRN_DMA_CR2(base)	(base + 0x10)	/* Channel Control Register 2*/
#define DCRN_DMA_CTC2(base)	(base + 0x11)	/* Count and Control Register 2 */
#define DCRN_DMA_SAH2(base)	(base + 0x12)	/* Source Address High Register 2*/
#define DCRN_DMA_SAL2(base)	(base + 0x13)	/* Source Address Low Register 2*/
#define DCRN_DMA_DAH2(base)	(base + 0x14)	/* Dest Addr High Register 2 */
#define DCRN_DMA_DAL2(base)	(base + 0x15)	/* Dest Addr Low Register 2 */
#define DCRN_DMA_SGH2(base)	(base + 0x16)	/* S/G Desc Addr High Register 2 */
#define DCRN_DMA_SGL2(base)	(base + 0x17)	/* S/G Desc Addr Low Register 2 */
#define DCRN_DMA_CR3(base)	(base + 0x18)	/* Channel Control Register 3*/
#define DCRN_DMA_CTC3(base)	(base + 0x19)	/* Count and Control Register 3 */
#define DCRN_DMA_SAH3(base)	(base + 0x1A)	/* Source Address High Register 3*/
#define DCRN_DMA_SAL3(base)	(base + 0x1B)	/* Source Address Low Register 3*/
#define DCRN_DMA_DAH3(base)	(base + 0x1C)	/* Dest Addr High Register 3 */
#define DCRN_DMA_DAL3(base)	(base + 0x1D)	/* Dest Addr Low Register 3 */
#define DCRN_DMA_SGH3(base)	(base + 0x1E)	/* S/G Desc Addr High Register 3 */
#define DCRN_DMA_SGL3(base)	(base + 0x1F)	/* S/G Desc Addr Low Register 3 */
#define DCRN_DMA_SR(base)	(base + 0x20)	/* Status Register */
						/* Reserved 1A1..1A2 */
#define DCRN_DMA_SGC(base)	(base + 0x23)	/* S/G Command Register */
#define DCRN_DMA_PMP(base)	(base + 0x24)	/* PLB Master Priority Register */

/* uic */

#define DCRN_UIC0_BASE		0x2C0
#define DCRN_UIC1_BASE		0x2C8	/* uic1_0 */
#define DCRN_UIC2_BASE		0x350	/* uic1_1 */
#define DCRN_UIC3_BASE		0x358	/* uic1_2 */
#define DCRN_UIC4_BASE		0x360	/* uic1_3 */
#define DCRN_UIC5_BASE		0x2D0	/* uic2_0 */
#define DCRN_UIC6_BASE		0x2D8	/* uic2_1 */
#define DCRN_UIC7_BASE		0x2E0	/* uic2_2 */
#define DCRN_UIC8_BASE		0x2E8	/* uic2_3 */
#define DCRN_UIC9_BASE		0x2F0	/* uic2_4 */
#define DCRN_UIC10_BASE		0x2F8	/* uic2_5 */
#define DCRN_UIC11_BASE		0x300	/* uic2_6 */
#define DCRN_UIC12_BASE		0x308	/* uic2_7 */
#define DCRN_UIC13_BASE		0x310	/* uic2_8 */
#define DCRN_UIC14_BASE		0x318	/* uic2_9 */
#define DCRN_UIC15_BASE		0x320	/* uic2_10 */
#define DCRN_UIC16_BASE		0x328	/* uic2_11 */
#define DCRN_UIC17_BASE		0x330	/* uic2_12 */
#define DCRN_UIC18_BASE		0x338	/* uic2_13 */
#define DCRN_UIC19_BASE		0x340	/* uic2_14 */
#define DCRN_UIC20_BASE		0x348	/* uic2_15 */

#define DCRN_UIC0_SR		(DCRN_UIC0_BASE + 0x0 )
#define DCRN_UIC0_SRS		(DCRN_UIC0_BASE + 0x1 )
#define DCRN_UIC0_ER		(DCRN_UIC0_BASE + 0x2 )
#define DCRN_UIC0_CR		(DCRN_UIC0_BASE + 0x3 )
#define DCRN_UIC0_PR		(DCRN_UIC0_BASE + 0x4 )
#define DCRN_UIC0_TR		(DCRN_UIC0_BASE + 0x5 )
#define DCRN_UIC0_MSR		(DCRN_UIC0_BASE + 0x6 )
#define DCRN_UIC0_VR0		(DCRN_UIC0_BASE + 0x7 )

#define DCRN_UIC1_SR		(DCRN_UIC1_BASE + 0x0 )
#define DCRN_UIC1_SRS		(DCRN_UIC1_BASE + 0x1 )
#define DCRN_UIC1_ER		(DCRN_UIC1_BASE + 0x2 )
#define DCRN_UIC1_CR		(DCRN_UIC1_BASE + 0x3 )
#define DCRN_UIC1_PR		(DCRN_UIC1_BASE + 0x4 )
#define DCRN_UIC1_TR		(DCRN_UIC1_BASE + 0x5 )
#define DCRN_UIC1_MSR		(DCRN_UIC1_BASE + 0x6 )
#define DCRN_UIC1_VR0		(DCRN_UIC1_BASE + 0x7 )

#define DCRN_UIC2_SR		(DCRN_UIC2_BASE + 0x0 )
#define DCRN_UIC2_SRS		(DCRN_UIC2_BASE + 0x1 )
#define DCRN_UIC2_ER		(DCRN_UIC2_BASE + 0x2 )
#define DCRN_UIC2_CR		(DCRN_UIC2_BASE + 0x3 )
#define DCRN_UIC2_PR		(DCRN_UIC2_BASE + 0x4 )
#define DCRN_UIC2_TR		(DCRN_UIC2_BASE + 0x5 )
#define DCRN_UIC2_MSR		(DCRN_UIC2_BASE + 0x6 )
#define DCRN_UIC2_VR0		(DCRN_UIC2_BASE + 0x7 )

#define DCRN_UIC3_SR		(DCRN_UIC3_BASE + 0x0 )
#define DCRN_UIC3_SRS		(DCRN_UIC3_BASE + 0x1 )
#define DCRN_UIC3_ER		(DCRN_UIC3_BASE + 0x2 )
#define DCRN_UIC3_CR		(DCRN_UIC3_BASE + 0x3 )
#define DCRN_UIC3_PR		(DCRN_UIC3_BASE + 0x4 )
#define DCRN_UIC3_TR		(DCRN_UIC3_BASE + 0x5 )
#define DCRN_UIC3_MSR		(DCRN_UIC3_BASE + 0x6 )
#define DCRN_UIC3_VR0		(DCRN_UIC3_BASE + 0x7 )

#define DCRN_UIC4_SR		(DCRN_UIC4_BASE + 0x0 )
#define DCRN_UIC4_SRS		(DCRN_UIC4_BASE + 0x1 )
#define DCRN_UIC4_ER		(DCRN_UIC4_BASE + 0x2 )
#define DCRN_UIC4_CR		(DCRN_UIC4_BASE + 0x3 )
#define DCRN_UIC4_PR		(DCRN_UIC4_BASE + 0x4 )
#define DCRN_UIC4_TR		(DCRN_UIC4_BASE + 0x5 )
#define DCRN_UIC4_MSR		(DCRN_UIC4_BASE + 0x6 )
#define DCRN_UIC4_VR0		(DCRN_UIC4_BASE + 0x7 )

#define DCRN_UIC5_SR		(DCRN_UIC5_BASE + 0x0 )
#define DCRN_UIC5_SRS		(DCRN_UIC5_BASE + 0x1 )
#define DCRN_UIC5_ER		(DCRN_UIC5_BASE + 0x2 )
#define DCRN_UIC5_CR		(DCRN_UIC5_BASE + 0x3 )
#define DCRN_UIC5_PR		(DCRN_UIC5_BASE + 0x4 )
#define DCRN_UIC5_TR		(DCRN_UIC5_BASE + 0x5 )
#define DCRN_UIC5_MSR		(DCRN_UIC5_BASE + 0x6 )
#define DCRN_UIC5_VR0		(DCRN_UIC5_BASE + 0x7 )

#define DCRN_UIC6_SR		(DCRN_UIC6_BASE + 0x0 )
#define DCRN_UIC6_SRS		(DCRN_UIC6_BASE + 0x1 )
#define DCRN_UIC6_ER		(DCRN_UIC6_BASE + 0x2 )
#define DCRN_UIC6_CR		(DCRN_UIC6_BASE + 0x3 )
#define DCRN_UIC6_PR		(DCRN_UIC6_BASE + 0x4 )
#define DCRN_UIC6_TR		(DCRN_UIC6_BASE + 0x5 )
#define DCRN_UIC6_MSR		(DCRN_UIC6_BASE + 0x6 )
#define DCRN_UIC6_VR0		(DCRN_UIC6_BASE + 0x7 )

#define DCRN_UIC7_SR		(DCRN_UIC7_BASE + 0x0 )
#define DCRN_UIC7_SRS		(DCRN_UIC7_BASE + 0x1 )
#define DCRN_UIC7_ER		(DCRN_UIC7_BASE + 0x2 )
#define DCRN_UIC7_CR		(DCRN_UIC7_BASE + 0x3 )
#define DCRN_UIC7_PR		(DCRN_UIC7_BASE + 0x4 )
#define DCRN_UIC7_TR		(DCRN_UIC7_BASE + 0x5 )
#define DCRN_UIC7_MSR		(DCRN_UIC7_BASE + 0x6 )
#define DCRN_UIC7_VR0		(DCRN_UIC7_BASE + 0x7 )

#define DCRN_UIC8_SR		(DCRN_UIC8_BASE + 0x0 )
#define DCRN_UIC8_SRS		(DCRN_UIC8_BASE + 0x1 )
#define DCRN_UIC8_ER		(DCRN_UIC8_BASE + 0x2 )
#define DCRN_UIC8_CR		(DCRN_UIC8_BASE + 0x3 )
#define DCRN_UIC8_PR		(DCRN_UIC8_BASE + 0x4 )
#define DCRN_UIC8_TR		(DCRN_UIC8_BASE + 0x5 )
#define DCRN_UIC8_MSR		(DCRN_UIC8_BASE + 0x6 )
#define DCRN_UIC8_VR0		(DCRN_UIC8_BASE + 0x7 )

#define DCRN_UIC9_SR		(DCRN_UIC9_BASE + 0x0 )
#define DCRN_UIC9_SRS		(DCRN_UIC9_BASE + 0x1 )
#define DCRN_UIC9_ER		(DCRN_UIC9_BASE + 0x2 )
#define DCRN_UIC9_CR		(DCRN_UIC9_BASE + 0x3 )
#define DCRN_UIC9_PR		(DCRN_UIC9_BASE + 0x4 )
#define DCRN_UIC9_TR		(DCRN_UIC9_BASE + 0x5 )
#define DCRN_UIC9_MSR		(DCRN_UIC9_BASE + 0x6 )
#define DCRN_UIC9_VR0		(DCRN_UIC9_BASE + 0x7 )

#define DCRN_UIC10_SR		(DCRN_UIC10_BASE + 0x0 )
#define DCRN_UIC10_SRS		(DCRN_UIC10_BASE + 0x1 )
#define DCRN_UIC10_ER		(DCRN_UIC10_BASE + 0x2 )
#define DCRN_UIC10_CR		(DCRN_UIC10_BASE + 0x3 )
#define DCRN_UIC10_PR		(DCRN_UIC10_BASE + 0x4 )
#define DCRN_UIC10_TR		(DCRN_UIC10_BASE + 0x5 )
#define DCRN_UIC10_MSR		(DCRN_UIC10_BASE + 0x6 )
#define DCRN_UIC10_VR0		(DCRN_UIC10_BASE + 0x7 )

#define DCRN_UIC11_SR		(DCRN_UIC11_BASE + 0x0 )
#define DCRN_UIC11_SRS		(DCRN_UIC11_BASE + 0x1 )
#define DCRN_UIC11_ER		(DCRN_UIC11_BASE + 0x2 )
#define DCRN_UIC11_CR		(DCRN_UIC11_BASE + 0x3 )
#define DCRN_UIC11_PR		(DCRN_UIC11_BASE + 0x4 )
#define DCRN_UIC11_TR		(DCRN_UIC11_BASE + 0x5 )
#define DCRN_UIC11_MSR		(DCRN_UIC11_BASE + 0x6 )
#define DCRN_UIC11_VR0		(DCRN_UIC11_BASE + 0x7 )

#define DCRN_UIC12_SR		(DCRN_UIC12_BASE + 0x0 )
#define DCRN_UIC12_SRS		(DCRN_UIC12_BASE + 0x1 )
#define DCRN_UIC12_ER		(DCRN_UIC12_BASE + 0x2 )
#define DCRN_UIC12_CR		(DCRN_UIC12_BASE + 0x3 )
#define DCRN_UIC12_PR		(DCRN_UIC12_BASE + 0x4 )
#define DCRN_UIC12_TR		(DCRN_UIC12_BASE + 0x5 )
#define DCRN_UIC12_MSR		(DCRN_UIC12_BASE + 0x6 )
#define DCRN_UIC12_VR0		(DCRN_UIC12_BASE + 0x7 )

#define DCRN_UIC13_SR		(DCRN_UIC13_BASE + 0x0 )
#define DCRN_UIC13_SRS		(DCRN_UIC13_BASE + 0x1 )
#define DCRN_UIC13_ER		(DCRN_UIC13_BASE + 0x2 )
#define DCRN_UIC13_CR		(DCRN_UIC13_BASE + 0x3 )
#define DCRN_UIC13_PR		(DCRN_UIC13_BASE + 0x4 )
#define DCRN_UIC13_TR		(DCRN_UIC13_BASE + 0x5 )
#define DCRN_UIC13_MSR		(DCRN_UIC13_BASE + 0x6 )
#define DCRN_UIC13_VR0		(DCRN_UIC13_BASE + 0x7 )

#define DCRN_UIC14_SR		(DCRN_UIC14_BASE + 0x0 )
#define DCRN_UIC14_SRS		(DCRN_UIC14_BASE + 0x1 )
#define DCRN_UIC14_ER		(DCRN_UIC14_BASE + 0x2 )
#define DCRN_UIC14_CR		(DCRN_UIC14_BASE + 0x3 )
#define DCRN_UIC14_PR		(DCRN_UIC14_BASE + 0x4 )
#define DCRN_UIC14_TR		(DCRN_UIC14_BASE + 0x5 )
#define DCRN_UIC14_MSR		(DCRN_UIC14_BASE + 0x6 )
#define DCRN_UIC14_VR0		(DCRN_UIC14_BASE + 0x7 )

#define DCRN_UIC15_SR		(DCRN_UIC15_BASE + 0x0 )
#define DCRN_UIC15_SRS		(DCRN_UIC15_BASE + 0x1 )
#define DCRN_UIC15_ER		(DCRN_UIC15_BASE + 0x2 )
#define DCRN_UIC15_CR		(DCRN_UIC15_BASE + 0x3 )
#define DCRN_UIC15_PR		(DCRN_UIC15_BASE + 0x4 )
#define DCRN_UIC15_TR		(DCRN_UIC15_BASE + 0x5 )
#define DCRN_UIC15_MSR		(DCRN_UIC15_BASE + 0x6 )
#define DCRN_UIC15_VR0		(DCRN_UIC15_BASE + 0x7 )

#define DCRN_UIC16_SR		(DCRN_UIC16_BASE + 0x0 )
#define DCRN_UIC16_SRS		(DCRN_UIC16_BASE + 0x1 )
#define DCRN_UIC16_ER		(DCRN_UIC16_BASE + 0x2 )
#define DCRN_UIC16_CR		(DCRN_UIC16_BASE + 0x3 )
#define DCRN_UIC16_PR		(DCRN_UIC16_BASE + 0x4 )
#define DCRN_UIC16_TR		(DCRN_UIC16_BASE + 0x5 )
#define DCRN_UIC16_MSR		(DCRN_UIC16_BASE + 0x6 )
#define DCRN_UIC16_VR0		(DCRN_UIC16_BASE + 0x7 )

#define DCRN_UIC17_SR		(DCRN_UIC17_BASE + 0x0 )
#define DCRN_UIC17_SRS		(DCRN_UIC17_BASE + 0x1 )
#define DCRN_UIC17_ER		(DCRN_UIC17_BASE + 0x2 )
#define DCRN_UIC17_CR		(DCRN_UIC17_BASE + 0x3 )
#define DCRN_UIC17_PR		(DCRN_UIC17_BASE + 0x4 )
#define DCRN_UIC17_TR		(DCRN_UIC17_BASE + 0x5 )
#define DCRN_UIC17_MSR		(DCRN_UIC17_BASE + 0x6 )
#define DCRN_UIC17_VR0		(DCRN_UIC17_BASE + 0x7 )

#define DCRN_UIC18_SR		(DCRN_UIC18_BASE + 0x0 )
#define DCRN_UIC18_SRS		(DCRN_UIC18_BASE + 0x1 )
#define DCRN_UIC18_ER		(DCRN_UIC18_BASE + 0x2 )
#define DCRN_UIC18_CR		(DCRN_UIC18_BASE + 0x3 )
#define DCRN_UIC18_PR		(DCRN_UIC18_BASE + 0x4 )
#define DCRN_UIC18_TR		(DCRN_UIC18_BASE + 0x5 )
#define DCRN_UIC18_MSR		(DCRN_UIC18_BASE + 0x6 )
#define DCRN_UIC18_VR0		(DCRN_UIC18_BASE + 0x7 )

#define DCRN_UIC19_SR		(DCRN_UIC19_BASE + 0x0 )
#define DCRN_UIC19_SRS		(DCRN_UIC19_BASE + 0x1 )
#define DCRN_UIC19_ER		(DCRN_UIC19_BASE + 0x2 )
#define DCRN_UIC19_CR		(DCRN_UIC19_BASE + 0x3 )
#define DCRN_UIC19_PR		(DCRN_UIC19_BASE + 0x4 )
#define DCRN_UIC19_TR		(DCRN_UIC19_BASE + 0x5 )
#define DCRN_UIC19_MSR		(DCRN_UIC19_BASE + 0x6 )
#define DCRN_UIC19_VR0		(DCRN_UIC19_BASE + 0x7 )

#define DCRN_UIC20_SR		(DCRN_UIC20_BASE + 0x0 )
#define DCRN_UIC20_SRS		(DCRN_UIC20_BASE + 0x1 )
#define DCRN_UIC20_ER		(DCRN_UIC20_BASE + 0x2 )
#define DCRN_UIC20_CR		(DCRN_UIC20_BASE + 0x3 )
#define DCRN_UIC20_PR		(DCRN_UIC20_BASE + 0x4 )
#define DCRN_UIC20_TR		(DCRN_UIC20_BASE + 0x5 )
#define DCRN_UIC20_MSR		(DCRN_UIC20_BASE + 0x6 )
#define DCRN_UIC20_VR0		(DCRN_UIC20_BASE + 0x7 )



/* OPB/PLB bridge */
#define DCRN_OPLB_GCTRL 	0x3F8	/* Bridge Control Register */
#define DCRN_OPLB_GSTAT 	0x3F9	/* Bridge Status Register */
#define DCRN_OPLB_GEARL 	0x3FA	/* Bridge Error Addr Register Low */
#define DCRN_OPLB_GEARH 	0x3FB	/* Bridge Error Addr Register High */
#define DCRN_OPLB_GREVID 	0x3FC	/* Bridge Revision ID Register */
					/* Reserved 3FD..3FF */

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_FSP1_DCR_H_ */
