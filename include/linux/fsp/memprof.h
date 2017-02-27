/*
 *   Copyright (c) International Business Machines Corp., 2007,2012
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

#ifndef _MEMPROF_H
#define _MEMPROF_H

/* support for profiling memory allocator */
#include <asm/atomic.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>


/* backtrace hash support */
#define BTDEPTH		4

#define KMALLOC		1
#define KFREE		2


extern uint32_t slab_traceon;

typedef struct btinfo_struct {
	struct list_head list;		/* hash list */
	unsigned long cachep;		/* cachep | wrapped */
#define WRAP_MASK 0x1			/* cachep low bit set indicates that
					   nhits has wrapped */
	atomic_t nhits;			/* number of bt hits */
	unsigned long bt[BTDEPTH];	/* backtrace */
} btinfo_t;

btinfo_t *btinfo_lookup_insert(unsigned int, unsigned long, unsigned long,
			       unsigned long *);

/* instrumentation for kmalloc */
#define GET_BT(backtrace)						\
{									\
	uint cnt = 0;							\
	unsigned long *prev_sp = (unsigned long *)(current->stack + 1); \
	unsigned long *stack_top = 					\
		(unsigned long *)current->stack + THREAD_SIZE;		\
	unsigned long *sp;						\
	asm("mr %0,1" : "=r" (sp));					\
	__get_user(sp, (unsigned long **)sp);				\
	while ((cnt < BTDEPTH) && (sp > prev_sp) && (sp < stack_top)) {	\
		if (__get_user(backtrace[cnt], &sp[1]))			\
			break;						\
		if (__get_user(sp, (unsigned long **)sp))		\
			break;						\
		cnt++;							\
	}								\
}


#define KMALLOC_TRACE(flag, size, ptr)					\
{									\
	if (slab_traceon) {						\
		unsigned long backtrace[BTDEPTH] = {0};			\
		GET_BT(backtrace);					\
		(void)btinfo_lookup_insert(flag, size, (unsigned long)ptr, \
					   backtrace);			\
	}								\
}


#endif
