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

#define TIMEOUTDEFAULT 200

static uint32_t get_mail(const unsigned int ctrl_num, bool stream)
{
	int timeout;
	uint32_t mail = 0;

	timeout = TIMEOUTDEFAULT;
	while (--timeout &&
	       (phy_io_read16(ctrl_num, t_apbonly | csr_uct_shadow_regs)
		& uct_write_prot_shadow_mask)) {
		mdelay(10);
	}
	if (!timeout) {
		printf("Error: timeout getting mail in %s\n", __func__);
		return 0xFFFF;
	}

	mail = phy_io_read16(ctrl_num, t_apbonly |
			     csr_uct_write_only_shadow);
	if (stream) {
		mail |= phy_io_read16(ctrl_num, t_apbonly |
				      csr_uct_dat_write_only_shadow) << 16;
	}

	/* Ack */
	phy_io_write16(ctrl_num, t_apbonly | csr_dct_write_prot, 0);

	timeout = TIMEOUTDEFAULT;
	while (--timeout &&
	       !(phy_io_read16(ctrl_num, t_apbonly | csr_uct_shadow_regs)
		 & uct_write_prot_shadow_mask)) {
		mdelay(1);
	}
	if (!timeout)
		printf("Error: timeout ack mail in %s\n", __func__);

	/* completed */
	phy_io_write16(ctrl_num, t_apbonly | csr_dct_write_prot, 1);

	return mail;
}

static void decode_stream_message(const unsigned int ctrl_num)
{
	uint32_t index;
	uint32_t args[12];
	int i, j;

	index = get_mail(ctrl_num, 1);
	if ((index & 0xffff) > 12)	/* up to 12 args so far */
		printf("Program error in %s\n", __func__);
	for (i = 0; i < (index & 0xffff) && i < 12; i++)
		args[i] = get_mail(ctrl_num, 1);
	debug("0x%08x:\t", index);
	for (j = 0; j < i; j++)
		debug("0x%x\t", args[j]);
	debug("\n");
}

static int wait_fw_done(const unsigned int ctrl_num, int train2d)
{
	uint32_t mail = 0;

	while (mail == 0x0) {
		mail = get_mail(ctrl_num, 0);
		switch (mail) {
		case 0x7:
			debug("%s Training completed\n", train2d ? "2D" : "1D");
			break;
		case 0xff:
			debug("%s Training failure\n", train2d ? "2D" : "1D");
			break;
		case 0x0:
			debug("End of initialization\n");
			mail = 0;
			break;
		case 0x1:
			debug("End of fine write leveling\n");
			mail = 0;
			break;
		case 0x2:
			debug("End of read enable training\n");
			mail = 0;
			break;
		case 0x3:
			debug("End of read delay center optimization\n");
			mail = 0;
			break;
		case 0x4:
			debug("End of write delay center optimization\n");
			mail = 0;
			break;
		case 0x5:
			debug("End of 2D read delay/voltage center optimization\n");
			mail = 0;
			break;
		case 0x6:
			debug("End of 2D write delay /voltage center optimization\n");
			mail = 0;
			break;
		case 0x8:
			decode_stream_message(ctrl_num);
			mail = 0;
			break;
		case 0x9:
			debug("End of max read latency training\n");
			mail = 0;
			break;
		case 0xa:
			debug("End of read dq deskew training\n");
			mail = 0;
			break;
		case 0xc:
			debug("End of LRDIMM Specific training (DWL, MREP, MRD and MWD)\n");
			mail = 0;
			break;
		case 0xd:
			debug("End of CA training\n");
			mail = 0;
			break;
		case 0xfd:
			debug("End of MPR read delay center optimization\n");
			mail = 0;
			break;
		case 0xfe:
			debug("End of Write leveling coarse delay\n");
			mail = 0;
			break;
		default:
			mail = 0;
			break;
		}
	}

	if (mail == 0x7)
		return 0;
	else if (mail == 0xff)
		return -EIO;
	else if (mail == 0xffff)
		return -ETIME;

	debug("PHY_GEN2 FW: Unxpected mail = 0x%x\n", mail);

	return -EINVAL;
}

int g_exec_fw(const unsigned int ctrl_num, int train2d)
{
	int ret;

	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 0x1);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr,
		       csr_reset_to_micro_mask | csr_stall_to_micro_mask);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr,
		       csr_stall_to_micro_mask);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr, 0);

	ret = wait_fw_done(ctrl_num, train2d);
	if (ret == -ETIME)
		printf("Timed out while waiting for firmware execution\n");
	else if (ret == -EIO)
		printf("Training failed\n");
	else if (ret)
		printf("Unknown error\n");

	phy_io_write16(ctrl_num, t_apbonly | csr_micro_reset_addr,
		       csr_stall_to_micro_mask);

	return ret;
}
