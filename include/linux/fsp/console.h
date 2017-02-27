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
#ifndef _LINUX_FSP_CONSOLE_H_
#define _LINUX_FSP_CONSOLE_H_

#include <linux/tty.h>

struct fsptrace_ops {
	int (*trace_register)(const char *name, size_t size);
	int (*trace_unregister)(int td);
	int (*trace_binary)(int td, int level, unsigned int traceid,
			    unsigned int line, const char *address,
			    unsigned int size);
};

int register_fsptrace_callbacks(struct fsptrace_ops *);
int unregister_fsptrace_callbacks(struct fsptrace_ops *);

extern int silent_console;

void fsp_console_trace(struct tty_struct *tty, struct file *file,
		       const unsigned char *buf, size_t nr);


#endif
