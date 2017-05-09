/*
 *   Copyright (c) International Business Machines Corp., 2005, 2010
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
 *
 * **********************************************************************
 *
 * Safetynet driver header file.
 *
 * @author Gary Lowe <garylowe@us.ibm.com>
 *
 * @brief This file contains the header for the FSP-1 base Safetynet
 * register function and Safetynet driver.
 *
 * **********************************************************************
 */

#ifndef _LINUX_SND_H
#define _LINUX_SND_H

//#warning snd.h is deprecated.

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define SND_MAGIC_NUMBER     (0x00D6B7EDUL)
#define SND_SAFETY_NET_PAGES	3 /* main table */
#define SND_INTERRUPT_PAGES	4 /* interrupt table */
#if CONFIG_MODULES
#define SND_MODULE_PAGES	1 /* list of modules */
#else
#define SND_MODULE_PAGES	0 /* no module support */
#endif
#define SND_SAFETY_NET_SIZE	(SND_SAFETY_NET_PAGES * PAGE_SIZE)
#define SND_INTERRUPT_SIZE	(SND_INTERRUPT_PAGES * PAGE_SIZE)
#define SND_MODULE_SIZE		(SND_MODULE_PAGES * PAGE_SIZE)
#define SND_MAINAREA_SIZE	(SND_SAFETY_NET_SIZE+SND_INTERRUPT_SIZE+SND_MODULE_SIZE)
#define SND_MAX_ENTRIES		((SND_SAFETY_NET_SIZE - sizeof(trace_safety_net_table)) / sizeof(trace_safety_net_entry))
#define SND_MAX_TRACEBUFS	(SND_MAX_ENTRIES - 5)	/* 5 = 3 LTT buffers (irq, proc, trc), printk, and insmod */

//#define SND_PROC_SIZE            PAGE_SIZE   /* size of process table     */
#define SND_SAFETY_NET_NAME_SIZE 16 /* size of name in SAFETY_NET table   */
#define SND_INTR_CTRL_NAME_SIZE  28 /* max size of interrupt ctrl name    */
#define SND_INTR_OWNER_NAME_SIZE 28 /* max size of interrupt owner name   */
#define SND_MODULE_NAME_SIZE     SND_SAFETY_NET_NAME_SIZE /* max size of module name */
#define SND_FSP_TRACE_NAME_SIZE  16 /* max size of fsp-trace comp name    */
#define SND_PROC_NAME_SIZE       6  /* max size of process names          */
#define SND_PROC_NUM_PID         5  /* number of pid's to record          */

#define SND_SAFETY_NET_VERSION   2
#define SND_INTERRUPT_VERSION    1
#define SND_MODULE_VERSION       2
#define SND_PROC_VERSION         1
#define SND_FSP_TRACE_VERSION    2

#define SND_CURRENT              0
#define SND_SAVED                1

#define SND_GET_TABLE_HEAD		SND_MAGIC_NUMBER + 64
#define SND_GET_TABLE_BUFFER		SND_MAGIC_NUMBER + 65
#define SND_FREE_SAVED_TABLE		SND_MAGIC_NUMBER + 66

/* ioctl buffer read commands need a buffer name, a pointer and a size.
 * trace_safety_net_entry is useed for this: it has all the fields (name, virt, size).
 * additionally the field "real" is used to select current (0) or saved (1) snd.
 */

/* define entry format in SAFETY_NET table */
typedef struct _trace_safety_net_entry
{
	char name[SND_SAFETY_NET_NAME_SIZE];
	uint32_t virt;
	uint32_t real;
	uint32_t size;
} trace_safety_net_entry;

/* define entry format for interrupt table */
typedef struct _trace_interrupt_entry
{
	uint32_t num;
	char handler[SND_INTR_CTRL_NAME_SIZE];
	char owner[SND_INTR_OWNER_NAME_SIZE];
} trace_interrupt_entry;

/* define entry format for module entry */
typedef struct _trace_safety_net_entry trace_module_entry;

typedef struct _trace_proc_entry
{
	char name[SND_PROC_NAME_SIZE];
	uint16_t pid[SND_PROC_NUM_PID];
	uint16_t ppid[SND_PROC_NUM_PID];
	uint16_t  cntr;
} trace_proc_entry;

/* define entry format in fsp-trace entry */
typedef struct _trace_safety_net_entry trace_fsp_trace_entry;

/* head of table information */
typedef struct _trace_safety_net_table {
	uint32_t magic;
	uint32_t version;
	uint32_t used;
	uint32_t size;
	trace_safety_net_entry entry[0];
} trace_safety_net_table;

/* head of interrupt table */
typedef struct _trace_interrupt_table {
	uint32_t version;
	uint32_t used;
	trace_interrupt_entry entry[0];
} trace_interrupt_table;

/* head of module table */
typedef struct _trace_safety_net_table trace_module_table;

/* header for proc table */
typedef struct _trace_proc_table
{
	uint16_t version;
	uint16_t wrapped;
	uint32_t used;
	trace_proc_entry entry[0];
} trace_proc_table;

/* header for fsp trace table */
typedef struct _trace_fsp_trace_table
{
	uint32_t magic;
	uint32_t version;
	uint32_t used;
	uint32_t size;
	trace_fsp_trace_entry entry[0];
} trace_fsp_trace_table;

/* define names of FFDC items */
#define SND_LTT_TRACE  "snd_ltt_trace"   /* ltt trace buffer       */
#define SND_LTT_PROC   "snd_ltt_proc"    /* proc/interrupt info    */
#define SND_LTT_INTR   "snd_ltt_intr"    /* interrupt information  */
#define SND_PRINTK_BUF "snd_printk_buf"  /* printk buffer          */
#define SND_PRINTK_IDX "snd_printk_idx"  /* last entry in printk   */
#define SND_INSMOD_BUF "snd_insmod_buf"  /* location of insmod buf */
#define SND_FSP_TRACE  "snd_fsp_trace"   /* fsp-trace buffer table */

#define SND_FLAG_AVAIL   (0x80000000U)
#define SND_FLAG_SAVED   (0x40000000U)
#define SND_SIZE_SIZE(size)  ((size) & 0x00ffffff)

#ifdef __KERNEL__

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/module.h>

/* avoid ifdef's each time snd is used */
#ifdef CONFIG_FSP_SAFETYNET

/* interfaces for registering SafetyNet items. */
void snd_register(const char *, const void *, uint32_t);
void snd_register_module_info(const struct module *);
void snd_register_interrupt(const char *, uint32_t, const char *);

/* get snd table pointer and size */
int snd_get_table_pointer(trace_safety_net_table **paddr,
			  size_t *psize,
			  int whichone); /* SND_CURRENT or SND_SAVED */

/* free saved snd */
int snd_free_old_snd(void);

/* initialize snd. to be called from do_init_bootmem */
void snd_early_init(trace_safety_net_table *pdefault);

static inline unsigned long snd_pages_needed(void) {
	return SND_SAFETY_NET_PAGES + SND_INTERRUPT_PAGES;
}


/* register address in SafetyNet table */
#define SND_REGISTER(NAME, ADDR, SIZE)		\
	snd_register(NAME,ADDR,SIZE)
#define SND_REGISTER_INTERRUPT(NAME, IRQ, HANDLER)	\
	snd_register_interrupt(NAME, IRQ, HANDLER)

#ifdef CONFIG_MODULES
#define SND_REGISTER_MODULE(MOD)		\
	snd_register_module_info(MOD)
#else
/* shouldn't be used anywhere. should we BUG? */
#define SND_REGISTER_MODULE(MOD) do {} while(0)
#endif

#else

#define SND_REGISTER(NAME, ADDR, SIZE) do {} while(0)
#define SND_REGISTER_INTERRUPT(NAME, IRQ, IRQ_DESC) do {} while(0)
#define SND_REGISTER_MODULE(MOD) do {} while(0)
#define snd_pages_needed() (0)
#define snd_early_init(x) do {} while(0)

#endif

#endif /* KERNEL */

#endif
