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
#ifdef CONFIG_ARCH_LX2160A_PXP
#include "include/io.h"
#endif

/* #define DEBUG */

#ifdef DEBUG
#include "include/csr.h"
#include "include/imem.h"
#include "include/dmem.h"
#include "include/pie.h"
#include "include/overrides.h"

enum image_types {
	CSR,
	IMEM,
	DMEM,
	PIE,
	OVERRIDES
};

void load_image(const unsigned int ctrl_num, enum image_types image_type)
{
	int i;
	int size;

	const struct csr *csr_image = NULL;
	const struct imem *imem_image = NULL;
	const struct dmem *dmem_image = NULL;
	const struct pie *pie_image = NULL;
	const struct overrides *overrides_image = NULL;

	printf("Load Image-%d..\n", image_type);
	switch (image_type) {
	case CSR:
		csr_image = phy_csr;
		size = ARRAY_SIZE(phy_csr);
		if (csr_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, csr_image[i].addr, csr_image[i].data);
		}
		break;
	case IMEM:
		imem_image = image_imem;
		size = ARRAY_SIZE(image_imem);
		if (imem_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, imem_image[i].addr, imem_image[i].data);
		}
		break;
	case DMEM:
		dmem_image = image_dmem;
		size = ARRAY_SIZE(image_dmem);
		if (dmem_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, dmem_image[i].addr, dmem_image[i].data);
		}
		break;
	case PIE:
		pie_image = image_udimm;
		size = ARRAY_SIZE(image_udimm);
		if (pie_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, pie_image[i].addr, pie_image[i].data);
		}
		break;
	case OVERRIDES:
		overrides_image = image_overrides;
		size = ARRAY_SIZE(image_overrides);
		if (overrides_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, overrides_image[i].addr, overrides_image[i].data);
		}
		break;

	default:
		printf("Unsupported Image type\n");
		break;
	}
	printf("Load Image-%d..Done\n", image_type);
}

unsigned int compute_phy_config_regs(const unsigned int ctrl_num,
		const memctl_options_t *popts,
		fsl_ddr_cfg_regs_t *ddr)
{
	printf("DDR PHY Debug Path\n");

#ifdef CONFIG_FSL_PHY_GEN2_PHY_A2017_11
	printf("PHY_GEN2 FW Version: A2017.11\n");
#else
	printf("PHY_GEN2 FW Version: A2017.05\n");
#endif

	/* configure phy register */
	load_image(ctrl_num, CSR);

	/* Initialise firmware mailbox */
	phy_io_write16(ctrl_num, 0x0d0031, 0x1);
	phy_io_write16(ctrl_num, 0x0c0033, 0x1);

	/* Load IMEM image */
	load_image(ctrl_num, IMEM);

	/* Load DMEM image into memory */
	load_image(ctrl_num, DMEM);

	/* Execute training firmware and Wait for training to complete */
	g_exec_fw(ctrl_num);

	/* Load Phy Init Engine image */
	phy_io_write16(ctrl_num, 0x0d0000, 0x0);
	load_image(ctrl_num, PIE);
	phy_io_write16(ctrl_num, 0x0d0000, 0x1);

#ifdef CONFIG_ARCH_LX2160A_PXP
	phy_io_write16(ctrl_num, 0x0d0000, 0);
	load_image(ctrl_num, OVERRIDES);
	phy_io_write16(ctrl_num, 0x0d0000, 1);
#endif
	printf("DDR PHY Debug Path.. Done\n");
	return 0;

}
#else
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

#ifdef CONFIG_ARCH_LX2160A_PXP
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 0);
	prog_tx_odt_drv_stren(ctrl_num, input);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 1);
#endif

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
#endif
