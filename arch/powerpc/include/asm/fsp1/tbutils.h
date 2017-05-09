/* *****************************************************************************
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
 * *****************************************************************************
 * TB utils - Time base utilities for powerpc timebase
 *
 * @file tbutils.h
 *
 * @author Dean Sanner <dsanner@us.ibm.com>
 * @author Corey Hyllested <chylles@us.ibm.com>
 *
 * @brief This file contains some inline utility functions to capture and
 * and manipulate the powerpc timebase provided on the 40x and 4xx powerpc
 * processors.
 * ****************************************************************************/

#ifndef _ASM_POWERPC_TBUTILS_H_
#define _ASM_POWERPC_TBUTILS_H_

//#warning tbutils.h is deprecated.  Please use ktime_get_ts() or get_tb() instead!

#include <linux/errno.h>
#include <asm/reg.h>
#include <asm/time.h>

/**
 *  @struct  tbtime_t
 * This structure contains the processor timebase.  Used for very granular time measurements
 * The 405/440 timebase is a 64bit value so a structure is used to represent the full value.
*/
typedef union {
	uint64_t tb;		/* full 64 bit portion of timebase*/
	struct {
		uint32_t high;		/* High portion of the time base */
		uint32_t low;		/* Low portion of the time base */
	}half;
} tbtime_t;



/*=======================================================*/
/*=               Time base functions                    */
/*=======================================================*/

inline static int tbtime_get(tbtime_t * tb)
{
	int rc = -EINVAL;

	if (tb) {
		uint32_t tmp = 0;

		do {
			tmp = mfspr(SPRN_TBRU);
			tb->half.low = mfspr(SPRN_TBRL);
			tb->half.high = mfspr(SPRN_TBRU);
		} while (tmp != tb->half.high);

		rc = 0;
	}

	return rc;
}

static inline void tbtime_to_timeval(struct timeval *timev, uint64_t ticks)
{
        uint64_t time;

	/* convert ticks to nanoseconds */
	time = ticks * (1000000000 / (ppc_tb_freq - 1));

	/* convert time to microseconds */
	do_div(time, NSEC_PER_USEC);

	/* save seconds, microseconds in timeval. */
	timev->tv_usec = do_div(time, USEC_PER_SEC);
	timev->tv_sec = (uint32_t) time;
}

#endif /* _ASM_POWERPC_TBUTILS_H_ */
