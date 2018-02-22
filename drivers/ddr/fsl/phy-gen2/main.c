/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <fsl_ddr_sdram.h>
#include <fsl_errata.h>
#include <fsl_ddr.h>
#include <fsl_immap.h>
#include "include/init.h"
#include "include/dimm.h"
#include "include/phy_gen2_fit.h"

unsigned int compute_phy_config_regs(const unsigned int ctrl_num,
		const memctl_options_t *popts,
		fsl_ddr_cfg_regs_t *ddr)
{
	int ret;
	struct dimm *dimm;
	struct input *input;
	void *msg_1d, *msg_2d;
	size_t len;

	dimm = calloc(1, sizeof(struct dimm));
	if (!dimm)
		return -ENOMEM;

	dimm->dramtype = DDR4;

	/* FIXME: Add condition for LRDIMM */
	dimm->dimmtype = UDIMM;

	dimm->primary_sdram_width = 64;
	dimm->n_ranks = 1;
	dimm->data_width = 64;
	/* FIXME: change frequency dymanically */
	input = phy_gen2_init_input(ctrl_num, dimm, 1600);
	if (!input)
		return -ENOMEM;

	ret = phy_gen2_msg_init(ctrl_num, &msg_1d, &msg_2d, input, &len);
	if (ret)
		return ret;

	ret = c_init_phy_config(ctrl_num, input, msg_1d);
	if (ret) {
		printf("Error in step C\n");
		return ret;
	}

	ret = phy_gen2_dimm_train(ctrl_num, input, 0); /* train 1D first */
	if (ret)
		return ret;

	/* FIXME: How to determine training firmware is done? */
	g_exec_fw(ctrl_num);
	/* FIXME: add 2D training parameter later */
	h_readmsgblock(ctrl_num);

	if (input->basic.train2d) {		/* 2D training starts here */
		ret = phy_gen2_dimm_train(ctrl_num, input, 0); /* train 1D first */
			if (ret)
				return ret;
		g_exec_fw(ctrl_num);
		/* FIXME: add 2D training parameter later */
		h_readmsgblock(ctrl_num);
	}


	i_load_pie(ctrl_num, input, msg_1d);

	printf("DDR4(%d) %s with %d-rank %d-bit bus (x%d)\n", ctrl_num,
	       dimm->dimmtype == RDIMM ? "RDIMM" : dimm->dimmtype == LRDIMM ?
	       "LRDIMM" : "UDIMM", dimm->n_ranks, dimm->primary_sdram_width,
	       dimm->data_width);

	if (msg_1d)
		free(msg_1d);
	if (msg_2d)
		free(msg_2d);
	if (input)
		free(input);
	if (dimm)
		free(dimm);
	return 0;
}
