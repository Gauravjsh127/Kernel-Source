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
 */

#include <linux/module.h>
#include <asm/reg.h>
#include <asm/dcr.h>

#include <linux/fsp/pra.h>

#define PRA_START		0x3300
#define PRA_SIZE		(832 * 4)
#define PRA_CURRENT_ADDR	PRA_START
#define PRA_PREVIOUS_ADDR	(PRA_START + (PRA_SIZE/2))

static u32 pra_cache[PRA_NUMENTRIES];

static u32 __pra_read(bool current, unsigned int key)
{
	u32 addr = (current ? PRA_CURRENT_ADDR : PRA_PREVIOUS_ADDR) + (key * 4);

	mtdcr(DCRN_DDR_WPA_ADDR,  addr);
	return mfdcr(DCRN_DDR_WPA_DATA);
}

static void __pra_write(unsigned int key, u32 data)
{
	u32 addr = (current ? PRA_CURRENT_ADDR : PRA_PREVIOUS_ADDR) + (key * 4);

	mtdcr(DCRN_DDR_WPA_ADDR, addr);
	mtdcr(DCRN_DDR_WPA_DATA, data);
}

/* verify the PRA cache checksum and return 1 if valid */
static int pra_cache_validate(void)
{
	int i;
	unsigned int checksum = 0;

	/* the checksum is simply an XOR of all the other PRA registers */
	for (i = PRA_CHECKSUM+1; i < PRA_NUMENTRIES; i++)
		checksum ^= pra_cache[i];

	return (checksum == pra_cache[PRA_CHECKSUM]) ? 1 : 0;
}

/* flush the PRA cache to HW */
static void pra_cache_flush(void)
{
	int i;

	for (i = 0; i < PRA_NUMENTRIES; i++)
		__pra_write(i, pra_cache[i]);
}

/* read PRA value from cache, or prev PRA value from HW */
u32 arch_pra_read(bool current, unsigned int key)
{
	/* read previous value directly from HW */
	if (!current)
		return __pra_read(current, key);

	/* read current value from cache */
	return pra_cache[key];
}
EXPORT_SYMBOL(arch_pra_read);

/* write a value to the PRA cache and flush cache to HW */
void arch_pra_write(unsigned int key, u32 data)
{
	pra_cache[key] = data;
	pra_cache_flush();
}
EXPORT_SYMBOL(arch_pra_write);

void __init arch_pra_init(void)
{
	int i;

	/* populate the cache */
	for (i = 0; i < PRA_NUMENTRIES; i++)
		pra_cache[i] = __pra_read(true, i);

	/* make sure the cache checksum is valid */
	if (!pra_cache_validate()) {
		printk("Current PRA is invalid - clearing...\n");
		memset(pra_cache, 0, PRA_NUMENTRIES * 4);
		pra_cache_flush();
	}
}
EXPORT_SYMBOL(arch_pra_init);
