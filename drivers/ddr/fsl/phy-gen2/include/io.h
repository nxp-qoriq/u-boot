/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#ifndef _IO_H_
#define _IO_H_

#include "init.h"

#define DDR_PHY1_ADDR	0x1400000
#define DDR_PHY2_ADDR	0x1600000
uint32_t phy_io_addr(const unsigned int ctrl_num, uint32_t addr);
void phy_io_write16(const unsigned int ctrl_num, uint32_t addr, uint16_t data);
void phy_io_write16_debug(const unsigned int ctrl_num, uint32_t addr,
			  uint16_t data);
uint16_t phy_io_read16(const unsigned int ctrl_num, uint32_t addr);

#endif
