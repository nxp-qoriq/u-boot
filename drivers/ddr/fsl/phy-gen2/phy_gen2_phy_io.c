/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#include <common.h>
#include <asm/io.h>
#include "include/io.h"

#define MAP_PHY_ADDR(pstate, n, instance, offset, c) \
		((((pstate * n) + instance + c) << 12) + offset)

uint32_t map_phy_addr_space(uint32_t addr)
{
	/* 23 bit addressing */
	int pstate =     (addr & 0x700000) >> 20; /* bit 22:20 */
	int block_type = (addr & 0x0f0000) >> 16; /* bit 19:16 */
	int instance =   (addr & 0x00f000) >> 12; /* bit 15:12 */
	int offset =     (addr & 0x000fff);       /* bit 11:0 */

	debug("addr = 0x%0x\n", addr);
	debug("pstate = %d, block_type = 0x%x, instance = %d, offset 0x%x\n",
	      pstate, block_type, instance, offset);

	switch (block_type) {
	case 0x0: /* 0x0 : ANIB */
		return MAP_PHY_ADDR(pstate, 12, instance, offset, 0);
	case 0x1: /* 0x1 : DBYTE */
		return MAP_PHY_ADDR(pstate, 10, instance, offset, 0x30);
	case 0x2: /* 0x2 : MASTER */
		return MAP_PHY_ADDR(pstate, 1, instance, offset, 0x58);
	case 0x4: /* 0x4 : ACSM */
		return MAP_PHY_ADDR(pstate, 1, instance, offset, 0x5c);
	case 0x5: /* 0x5 : μCTL Memory */
		return MAP_PHY_ADDR(pstate, 0, instance, offset, 0x60);
	case 0x7: /* 0x7 : PPGC */
		return MAP_PHY_ADDR(pstate, 0, 0, offset, 0x68);
	case 0x9: /* 0x9 : INITENG */
		return MAP_PHY_ADDR(pstate, 1, instance, offset, 0x69);
	case 0xc: /* 0xC : DRTUB */
		return MAP_PHY_ADDR(pstate, 0, 0, offset, 0x6d);
	case 0xd: /* 0xD : APB Only */
		return MAP_PHY_ADDR(pstate, 0, 0, offset, 0x6e);
	default:
		printf("ERR: Invalid block_type = 0x%x\n", block_type);
		return 0;
	}
}

uint32_t phy_io_addr(const unsigned int ctrl_num, uint32_t addr)
{
	uint32_t ddr_phy_addr = 0x0;
	switch (ctrl_num) {
	case 0:
		ddr_phy_addr = DDR_PHY1_ADDR;
		break;
	case 1:
		ddr_phy_addr = DDR_PHY2_ADDR;
		break;
	default:
		printf("ERR: Invalid controller_number = %u\n", ctrl_num);
		return ddr_phy_addr;
	}
	return ddr_phy_addr + (map_phy_addr_space(addr) << 2);
}

void phy_io_write16(const unsigned int ctrl_num, uint32_t addr, uint16_t data)
{
	out_le16((u16 *)(uintptr_t) phy_io_addr(ctrl_num, addr), data);
	debug("0x%x (0x%x) -> 0x%x\n", addr, phy_io_addr(ctrl_num, addr),
	      data);
}

void phy_io_write16_debug(const unsigned int ctrl_num, uint32_t addr,
			  uint16_t data)
{
	out_le16((u16 *)(uintptr_t) phy_io_addr(ctrl_num, addr), data);
	printf("0x%x (0x%x) -> 0x%x\n", addr, phy_io_addr(ctrl_num, addr),
	       data);
}

uint16_t phy_io_read16(const unsigned int ctrl_num, uint32_t addr)
{
	u16 reg = in_le16((u16 *)(uintptr_t) phy_io_addr(ctrl_num, addr));

	debug("R: 0x%x (0x%x) -> 0x%x\n", addr, phy_io_addr(ctrl_num, addr),
	       reg);
	return reg;
}
