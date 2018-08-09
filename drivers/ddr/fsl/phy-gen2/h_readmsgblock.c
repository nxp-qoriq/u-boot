/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#include <common.h>
#include "include/io.h"
#include "include/init.h"

void h_readmsgblock(const unsigned int ctrl_num)
{
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 0);

	/* FIXME: add h_readMsgBlock() here */

	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 1);
}
