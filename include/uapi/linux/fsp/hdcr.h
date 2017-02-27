/*
 *   Copyright (c) International Business Machines Corp., 2010, 2011
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
#ifndef _LINUX_FSP_HDCR_H_
#define _LINUX_FSP_HDCR_H_

/* DRCR access functions */
void drcr_clear(u32 clear_bits);
u32 drcr_read(void);

/* DISR access functions */
u32 disr_read(void);
void disr_set(u32 set_bits);
void disr_clear_and_set(u32 clear_bits, u32 set_bits, u32 *old_value);

/* DISR bits */
#define DISR_STAT_ACK                   0x00000001
/* Reserved, was Interface Error        0x00000002 */
/* unused                               0x00000004 */
#define DISR_CRIT_OP                    0x00000008
#define DISR_REBOOT_INPROG              0x00000010
#define DISR_RR_INPROG                  0x00000020
#define DISR_DEBUG                      0x00000040
#define DISR_STAT_SYNC                  0x00000080
#define DISR_FLASH_TERM                 0x00000100
#define DISR_RR_COMPLT                  0x00000200
#define DISR_RUNTIME_TERM               0x00000400
/* unused                               0x00000800 */
/* unused                               0x00001000 */
/* unused                               0x00002000 */
/* Reserved, was Chip Reset Requested   0x00004000 */
#define DISR_RSVD                       0xFFFF8000


#endif
