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
#include <linux/fsp/pra.h>
#include <linux/mtd/mtd.h>
#include <linux/err.h>
#include <asm/io.h>

#define PRA_START		0x10b0025e80ULL
#define PRA_SIZE		384
#define PRA_PREVIOUS_OFFSET	(PRA_SIZE/2) /* 0x10b0025f40 */
#define MTD_OFFSET		0x1e80

static void __iomem *pra_base_addr;

static inline u32 pra_offset(bool current, unsigned int key)
{
	return (current ? 0 : PRA_PREVIOUS_OFFSET) + (key * 4);
}

static inline void __iomem *pra_key_to_address(bool current, unsigned int key)
{
	return pra_base_addr + pra_offset(current, key);
}

u32 arch_pra_read(bool current, unsigned int key)
{
	u32 val = 0;
#ifdef CONFIG_MTD
	struct mtd_info *mtd = NULL;
	size_t bytes_read = 0;
	int32_t rc;

	mtd = get_mtd_device_nm("sram");
	if (IS_ERR(mtd))
		goto iomem_read;

	rc = mtd_read(mtd, MTD_OFFSET + pra_offset(current, key),
			sizeof(u32), &bytes_read, (uint8_t *)&val);
	put_mtd_device(mtd);
	if (!rc && bytes_read == sizeof(u32))
		goto exit;
#endif
iomem_read:
	val = readl_be(pra_key_to_address(current, key));
exit:
	return val;
}
EXPORT_SYMBOL(arch_pra_read);

void arch_pra_write(unsigned int key, u32 data)
{
#ifdef CONFIG_MTD
	struct mtd_info *mtd = NULL;
	size_t bytes_written = 0;
	int32_t rc;

	mtd = get_mtd_device_nm("sram");
	if (IS_ERR(mtd))
		goto iomem_write;

	rc = mtd_write(mtd, MTD_OFFSET + pra_offset(true, key),
			sizeof(u32), &bytes_written, (uint8_t *)&data);
	put_mtd_device(mtd);
	if (rc || bytes_written != sizeof(u32))
		goto iomem_write;
	else
		return;
#endif
iomem_write:
	writel_be(data, pra_key_to_address(true, key));
}
EXPORT_SYMBOL(arch_pra_write);

void __init arch_pra_init(void)
{
	pra_base_addr = ioremap(PRA_START, PRA_SIZE);
}
EXPORT_SYMBOL(arch_pra_init);
