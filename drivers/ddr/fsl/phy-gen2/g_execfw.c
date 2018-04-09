/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#include <common.h>
#include "include/dimm.h"
#include "include/io.h"
#include "include/init.h"

/* FIXME: Replace with timer */
#ifdef CONFIG_FSL_PHY_GEN2_PHY_A2017_11
#define TIMEOUTDEFAULT 100
#else
#define TIMEOUTDEFAULT 10
#endif

static int wait_fw_done(const unsigned int ctrl_num)
{
	int timeout;
	uint16_t mail = 0;

	while (mail == 0x0) {
		timeout = TIMEOUTDEFAULT;
		while (--timeout && (phy_io_read16(ctrl_num, t_apbonly |
		       csr_uct_shadow_regs) & uct_write_prot_shadow_mask)) {
		}
		if (!timeout)
			return -ETIME;

		mail = phy_io_read16(ctrl_num, t_apbonly |
				     csr_uct_write_only_shadow);

		printf("PHY_GEN2 FW: mail = 0x%x\n", mail);

		if (!(mail == 0x07 || mail == 0xff))
			mail = 0x0;

		phy_io_write16(ctrl_num, t_apbonly |
			       csr_dct_write_prot, 0);	/* Ack */

		timeout = TIMEOUTDEFAULT;
		while (--timeout && !(phy_io_read16(ctrl_num, t_apbonly |
		       csr_uct_shadow_regs) & uct_write_prot_shadow_mask)) {
		}
		if (!timeout)
			return -ETIME;
		phy_io_write16(ctrl_num, t_apbonly |
			       csr_dct_write_prot, 1);	/* completed */
	}

	if (mail == 0x7)
		return 0;
	else if (mail == 0xff)
		return -EIO;

	debug("PHY_GEN2 FW: Unxpected mail = 0x%x\n", mail);

	return -EINVAL;
}

void g_exec_fw(const unsigned int ctrl_num)
{
	int ret;

	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 0x1);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr,
		       csr_reset_to_micro_mask | csr_stall_to_micro_mask);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr,
		       csr_stall_to_micro_mask);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr, 0);

	ret = wait_fw_done(ctrl_num);
	if (ret == -ETIME)
		printf("Timed out while waiting for firmware execution\n");
	else if (ret == 0xff)
		printf("Training failed\n");
	else if (ret)
		printf("Unknown error\n");

	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr,
		       csr_stall_to_micro_mask);
}
