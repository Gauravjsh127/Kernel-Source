#ifndef __CHKP_H__
#define __CHKP_H__
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

#include <asm/processor.h>

#define CHKP_KICKOFF		0x00000100 /* Not yet started the kernel */
#define CHKP_START		0x00000200 /* IAC have last two bits 0 */
#define CHKP_WMMUCR		0x00000300
#define CHKP_KBASE		0x00000400
#define CHKP_CONTEXTCHANGE0	0x00000500
#define CHKP_CONTEXTCHANGE1	0x00000600
#define CHKP_EXCEPTIONVECT	0x00000700

#define CHKP_EINIT		0x00000800 /* early_init */
#define CHKP_RELOC_OFFSET	0x00000900
#define CHKP_MEMSET_IO		0x00000A00
#define CHKP_IDENTIFY_CPU	0x00000B00
#define CHKP_EINIT_DONE		0x00000C00

#define CHKP_MINIT		0x00000D00 /* machine_init */
#define CHKP_PINIT		0x00000F00
#define CHKP_PINIT1		0x00001000
#define CHKP_MINIT_DONE		0x00001100

#define CHKP_MMU_INIT		0x00001200 /* MMU_init */
#define CHKP_MMUSETUP		0x00001300
#define CHKP_MMUINITHW		0x00001400
#define CHKP_MMUCONTEXT		0x00001500
#define CHKP_SWAPPER_INIT	0x00001600

#define CHKP_START_KERNEL	0x00002000
#define CHKP_SETUP_ARCH_B	0x00002100
#define CHKP_SETUP_ARCH_E	0x00002200

#define CHKP_SA_INITBMEM_C	0x00002280 /* setup_arch */
#define CHKP_SA_SETUP_C		0x00002300 /* setup_arch */
#define CHKP_SA_RETR_MAC_C	0x00002400 /* ..._setup_arch */
#define CHKP_SA_GETCLOCKS_C	0x00002500 /* ..._setup_arch */
#define CHKP_SA_SETUPHOSE_C	0x00002600 /* ..._setup_arch */
#define CHKP_SA_EARLYSER_C	0x00002700 /* ..._setup_arch */
#define CHKP_SA_PAGING_C	0x00002800 /* setup_arch */

#define CHKP_FREEBM_CORE_C	0x00002900 /* free_bootmem_core */
#define CHKP_FREEBM_C		0x00002A00 /* free_bootmem */

#define CHKP_BUILD_ZLIST_C	0x00002B00
#define CHKP_PALLOC_INIT_C	0x00002C00
#define CHKP_SORT_EXTTAB_C	0x00002D00
#define CHKP_TRAP_INIT_C	0x00002E00

#define CHKP_INIT_ROOTFS_B	0x00003200
#define CHKP_INIT_ROOTFS_E	0x00003200

#define CHKP_INITCALLS_B	0x00003600
#define CHKP_INITCALLS_E	0x00003700

#define CHKP_NAMESPACE_B	0x00003A00
#define CHKP_NAMESPACE_E	0x00003B00

#define CHKP_OPEN_CONSOLE_C	0x00004000 /* open("/dev/console") */
#define CHKP_RUN_INIT_C		0x00004100 /* run_init_process(...) */

#define CHKP_FSI1		0x00005000 /* FSI init done */
#define CHKP_FSI2		0x00005100 /* FSI init done */


#if defined(__ASSEMBLY__)

#if defined(CONFIG_ETXX)
#define CHECKPOINT(reg1, checkpoint)			\
		mftbl	reg1 ;				\
		mtspr	SPRN_DVC1, reg1 ;		\
		lis	reg1, checkpoint@h ;		\
		ori	reg1, reg1, checkpoint@l ;	\
		mtspr	SPRN_DAC1, reg1 ;
#else
#define CHECKPOINT(reg1, checkpoint)
#endif

#else /* not assembly */

#define CHKPT_PREFIX(chkpt) ((chkpt) | 0x20000000)

#include <linux/fsp/pra.h>

static void
checkpoint(unsigned long checkpoint)
{
#if defined(CONFIG_ETXX)
	unsigned long tbl = mfspr(SPRN_TBRL);
	mtspr(SPRN_DVC1, tbl);
	mtspr(SPRN_DAC1, CHKPT_PREFIX(checkpoint));
#elif defined(CONFIG_FSP)
	pra_write(PRA_CURR_CHKPT, CHKPT_PREFIX(checkpoint));
#endif
}

#endif /* defined(__ASSEMBLY__) */

#endif /* __CHKP_H__ */
