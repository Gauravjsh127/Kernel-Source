/*
 * Copyright (c) International Business Machines Corp., 2006,2012
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
#include <linux/kernel.h>
#include <asm/reg.h>
#include <asm/dcr.h>
#include <asm/fsp/reg.h>

#if defined(CONFIG_FSP1)

#define NUM_TLB_WAYS	1
#define NUM_TLB_INDEXES 64

struct tlb_0 {
	unsigned epn		: 22;
	unsigned v		: 1;
	unsigned ts		: 1;
	unsigned size		: 4;
	unsigned tpar		: 4;
};

struct tlb_1 {
	unsigned rpn		: 22;
	unsigned par1		: 2;
	unsigned reserved	: 4;
	unsigned erpn		: 4;
};

struct tlb_2 {
	unsigned par2		: 2;
	unsigned reserved	: 14;
	unsigned u		: 4;
	unsigned w		: 1;
	unsigned i		: 1;
	unsigned m		: 1;
	unsigned g		: 1;
	unsigned e		: 1;
	unsigned reserved2	: 1;
	unsigned ux		: 1;
	unsigned uw		: 1;
	unsigned ur		: 1;
	unsigned sx		: 1;
	unsigned sw		: 1;
	unsigned sr		: 1;
};

#elif defined(CONFIG_FSP2)

#define NUM_TLB_WAYS	4
#define NUM_TLB_INDEXES	256

struct tlb_0 {
	unsigned epn		: 20;
	unsigned v		: 1;
	unsigned ts		: 1;
	unsigned dsiz		: 6;
	unsigned reserved	: 1;
	unsigned epnpar		: 1;
	unsigned dsizpar	: 1;
	unsigned tidpar		: 1;
};

struct tlb_1 {
	unsigned rpn		: 20;
	unsigned rpnpar		: 1;
	unsigned erpnpar	: 1;
	unsigned erpn		: 10;
};

struct tlb_2 {
	unsigned storpermpar	: 1;
	unsigned reserved	: 13;
	unsigned il1i		: 1;
	unsigned il1d		: 1;
	unsigned u		: 4;
	unsigned w		: 1;
	unsigned i		: 1;
	unsigned m		: 1;
	unsigned g		: 1;
	unsigned e		: 1;
	unsigned reserved2	: 1;
	unsigned ux		: 1;
	unsigned uw		: 1;
	unsigned ur		: 1;
	unsigned sx		: 1;
	unsigned sw		: 1;
	unsigned sr		: 1;
};

#endif /* CONFIG_FSP2 */

struct tlb {
	struct tlb_0 word0;
	struct tlb_1 word1;
	struct tlb_2 word2;
	u32 tid;
};

struct sprs {
	u32 ccr0;
	u32 ccr1;
	u32 ccr2;
	u32 cr;
	u32 csrr0;
	u32 csrr1;
	u32 ctr;
	u32 dbcr0;
	u32 dbcr1;
	u32 dbcr2;
	u32 dbdr;
	u32 dbsr;
	u32 dcesr;
	u32 dcripr;
	u32 dear;
	u32 dec;
	u32 decar;
	u32 esr;
	u32 icesr;
	u32 ivpr;
	u32 lr;
	u32 mcsr;
	u32 mcsrr0;
	u32 mcsrr1;
	u32 mmucr;
	u32 msr;
	u32 pid;
	u32 pvr;
	u32 rmpd;
	u32 sprg0;
	u32 sprg1;
	u32 sprg2;
	u32 sprg3;
	u32 sprg4;
	u32 sprg5;
	u32 sprg6;
	u32 sprg7;
	u32 sprg8;
	u32 srr0;
	u32 srr1;
	u32 tbl;
	u32 tbu;
	u32 tcr;
	u32 tsr;
	u32 usprg0;
	u32 xer;
};

struct uic {
	char *name;
	u32 status;
	u32 enable;
};

#define UIC_SR		0
#define UIC_ER		2

#define DCRN_UIC0	0x2c0
#define DCRN_UIC0_1	0x380
#define DCRN_UIC0_2	0x388
#define DCRN_UIC1_0	0x2c8
#define DCRN_UIC1_1	0x350
#define DCRN_UIC1_2	0x358
#define DCRN_UIC1_3	0x360
#define DCRN_UIC1_4	0x368
#define DCRN_UIC1_5	0x370
#define DCRN_UIC2_0	0x2d0
#define DCRN_UIC2_1	0x2d8
#define DCRN_UIC2_2	0x2e0
#define DCRN_UIC2_3	0x2e8
#define DCRN_UIC2_4	0x2f0
#define DCRN_UIC2_5	0x2f8
#define DCRN_UIC2_6	0x300
#define DCRN_UIC2_7	0x308
#define DCRN_UIC2_8	0x310
#define DCRN_UIC2_9	0x318
#define DCRN_UIC2_10	0x320
#define DCRN_UIC2_11	0x328
#define DCRN_UIC2_12	0x330
#define DCRN_UIC2_13	0x338
#define DCRN_UIC2_14	0x340
#define DCRN_UIC2_15	0x348

struct uic_table {
	char *name;
	u32 dcr;
};

struct uic_table uic_table[] = {
	{ "0",		DCRN_UIC0},
#ifdef CONFIG_FSP2
	{ "0.1",	DCRN_UIC0_1},
	{ "0.2",	DCRN_UIC0_2},
#endif
	{ "1.0",	DCRN_UIC1_0},
	{ "1.1",	DCRN_UIC1_1},
	{ "1.2",	DCRN_UIC1_2},
	{ "1.3",	DCRN_UIC1_3},
#ifdef CONFIG_FSP2
	{ "1.4",	DCRN_UIC1_4},
	{ "1.5",	DCRN_UIC1_5},
#endif
	{ "2.0",	DCRN_UIC2_0},
	{ "2.1",	DCRN_UIC2_1},
	{ "2.2",	DCRN_UIC2_2},
	{ "2.3",	DCRN_UIC2_3},
	{ "2.4",	DCRN_UIC2_4},
	{ "2.5",	DCRN_UIC2_5},
	{ "2.6",	DCRN_UIC2_6},
	{ "2.7",	DCRN_UIC2_7},
	{ "2.8",	DCRN_UIC2_8},
	{ "2.9",	DCRN_UIC2_9},
	{ "2.10",	DCRN_UIC2_10},
	{ "2.11",	DCRN_UIC2_11},
	{ "2.12",	DCRN_UIC2_12},
	{ "2.13",	DCRN_UIC2_13},
	{ "2.14",	DCRN_UIC2_14},
	{ "2.15",	DCRN_UIC2_15},
};
#define NUM_UICS ARRAY_SIZE(uic_table)

struct crash_regs {
	struct sprs sprs;
	struct tlb tlbs[NUM_TLB_WAYS][NUM_TLB_INDEXES];
	struct uic uics[NUM_UICS];
};

struct crash_regs crash_regs;

void crash_save_regs(void)
{
	int i, j;

	__asm__ __volatile__("mfcr %0" : "=r" (crash_regs.sprs.cr));

	crash_regs.sprs.ccr0	= mfspr(SPRN_CCR0);
	crash_regs.sprs.ccr1	= mfspr(SPRN_CCR1);
	crash_regs.sprs.csrr0	= mfspr(SPRN_CSRR0);
	crash_regs.sprs.csrr1	= mfspr(SPRN_CSRR1);
	crash_regs.sprs.ctr	= mfspr(SPRN_CTR);
	crash_regs.sprs.dbcr0	= mfspr(SPRN_DBCR0);
	crash_regs.sprs.dbcr1	= mfspr(SPRN_DBCR1);
	crash_regs.sprs.dbcr2	= mfspr(SPRN_DBCR2);
	crash_regs.sprs.dbdr	= mfspr(SPRN_DBDR);
	crash_regs.sprs.dbsr	= mfspr(SPRN_DBSR);
	crash_regs.sprs.dear	= mfspr(SPRN_DEAR);
	crash_regs.sprs.dec	= mfspr(SPRN_DEC);
	crash_regs.sprs.decar	= mfspr(SPRN_DECAR);
	crash_regs.sprs.esr	= mfspr(SPRN_ESR);
	crash_regs.sprs.ivpr	= mfspr(SPRN_IVPR);
	crash_regs.sprs.lr	= mfspr(SPRN_LR);
	crash_regs.sprs.mcsr	= mfspr(SPRN_MCSR);
	crash_regs.sprs.mcsrr0	= mfspr(SPRN_MCSRR0);
	crash_regs.sprs.mcsrr1	= mfspr(SPRN_MCSRR1);
	crash_regs.sprs.msr	= mfmsr();
	crash_regs.sprs.pid	= mfspr(SPRN_PID);
	crash_regs.sprs.pvr	= mfspr(SPRN_PVR);
	crash_regs.sprs.sprg0	= mfspr(SPRN_SPRG0);
	crash_regs.sprs.sprg1	= mfspr(SPRN_SPRG1);
	crash_regs.sprs.sprg2	= mfspr(SPRN_SPRG2);
	crash_regs.sprs.sprg3	= mfspr(SPRN_SPRG3);
	crash_regs.sprs.sprg4	= mfspr(SPRN_SPRG4);
	crash_regs.sprs.sprg5	= mfspr(SPRN_SPRG5);
	crash_regs.sprs.sprg6	= mfspr(SPRN_SPRG6);
	crash_regs.sprs.sprg7	= mfspr(SPRN_SPRG7);
	crash_regs.sprs.srr0	= mfspr(SPRN_SRR0);
	crash_regs.sprs.srr1	= mfspr(SPRN_SRR1);
	crash_regs.sprs.tbl	= mfspr(SPRN_TBRL);
	crash_regs.sprs.tbu	= mfspr(SPRN_TBRU);
	crash_regs.sprs.tcr	= mfspr(SPRN_TCR);
	crash_regs.sprs.tsr	= mfspr(SPRN_TSR);
	crash_regs.sprs.usprg0	= mfspr(SPRN_USPRG0);
	crash_regs.sprs.xer	= mfspr(SPRN_XER);
#ifdef CONFIG_FSP2
	crash_regs.sprs.ccr2	= mfspr(SPRN_CCR2);
	crash_regs.sprs.dcesr	= mfspr(SPRN_DCESR);
	crash_regs.sprs.dcripr	= mfspr(SPRN_DCRIPR);
	crash_regs.sprs.icesr	= mfspr(SPRN_ICESR);
	crash_regs.sprs.rmpd	= mfspr(SPRN_RMPD);
	crash_regs.sprs.sprg8	= mfspr(SPRN_SPRG8);
#endif


	for (i = 0; i < NUM_TLB_WAYS; i++) {

		u32 way = i << 30;

		for (j = 0; j < NUM_TLB_INDEXES; j++) {

			u32 word0, word1, word2, addr, mmucr;

			addr = way | j;

			__asm__ __volatile__("tlbre %0,%1,0" : "=r" (word0)
							     : "r" (addr));

			mmucr = mfspr(SPRN_MMUCR);

			__asm__ __volatile__("tlbre %0,%1,1" : "=r" (word1)
							     : "r" (addr));

			__asm__ __volatile__("tlbre %0,%1,2" : "=r" (word2)
							     : "r" (addr));

			crash_regs.tlbs[i][j].tid = mmucr & 0xfff;
			*(u32 *)&crash_regs.tlbs[i][j].word0 = word0;
			*(u32 *)&crash_regs.tlbs[i][j].word1 = word1;
			*(u32 *)&crash_regs.tlbs[i][j].word2 = word2;
		}
	}

	for (i = 0; i < NUM_UICS; i++) {
		crash_regs.uics[i].name = uic_table[i].name;
		crash_regs.uics[i].status = mfdcr(uic_table[i].dcr + UIC_SR);
		crash_regs.uics[i].enable = mfdcr(uic_table[i].dcr + UIC_ER);
	}
}

