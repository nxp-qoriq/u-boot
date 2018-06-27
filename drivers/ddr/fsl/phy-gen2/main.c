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
#include <asm/arch/clock.h>

#if defined(CONFIG_ARCH_LX2160A_PXP) || defined(CONFIG_DDR_FIXED_SETTINGS)
#include "include/io.h"
#endif


#ifdef CONFIG_DDR_FIXED_SETTINGS
#include "include/csr.h"
#include "include/imem.h"
#include "include/dmem.h"
#ifndef CONFIG_ARCH_LX2160A_PXP
#include "include/imem_2d.h"
#include "include/dmem_2d.h"
#endif
#include "include/pie.h"
#include "include/overrides.h"

enum image_types {
	CSR,
	IMEM,
	DMEM,
#ifndef CONFIG_ARCH_LX2160A_PXP
	IMEM_2D,
	DMEM_2D,
#endif
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
#ifndef CONFIG_ARCH_LX2160A_PXP
	const struct imem_2d *imem_image_2d = NULL;
	const struct dmem_2d *dmem_image_2d = NULL;
#endif
	const struct pie *pie_image = NULL;
	const struct overrides *overrides_image = NULL;

	debug("Load Image-%d..\n", image_type);
	switch (image_type) {
	case CSR:
		csr_image = phy_csr;
		size = ARRAY_SIZE(phy_csr);
		if (csr_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, csr_image[i].addr,
					       csr_image[i].data);
		}
		break;
	case IMEM:
		imem_image = image_imem;
		size = ARRAY_SIZE(image_imem);
		if (imem_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, imem_image[i].addr,
					       imem_image[i].data);
		}
		break;
	case DMEM:
		dmem_image = image_dmem;
		size = ARRAY_SIZE(image_dmem);
		if (dmem_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, dmem_image[i].addr,
					       dmem_image[i].data);
		}
		break;
#ifndef CONFIG_ARCH_LX2160A_PXP
	case IMEM_2D:
		imem_image_2d = image_imem_2d;
		size = ARRAY_SIZE(image_imem_2d);
		if (imem_image_2d) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, imem_image_2d[i].addr,
					       imem_image_2d[i].data);
		}
		break;
	case DMEM_2D:
		dmem_image_2d = image_dmem_2d;
		size = ARRAY_SIZE(image_dmem_2d);
		if (dmem_image_2d) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, dmem_image_2d[i].addr,
					       dmem_image_2d[i].data);
		}
		break;
#endif
	case PIE:
		pie_image = image_udimm;
		size = ARRAY_SIZE(image_udimm);
		if (pie_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, pie_image[i].addr,
					       pie_image[i].data);
		}
		break;
	case OVERRIDES:
		overrides_image = image_overrides;
		size = ARRAY_SIZE(image_overrides);
		if (overrides_image) {
			for (i = 0; i < size; i++)
				phy_io_write16(ctrl_num, overrides_image[i].addr,
					       overrides_image[i].data);
		}
		break;

	default:
		printf("Unsupported Image type\n");
		break;
	}
	debug("Load Image-%d..Done\n", image_type);
}

int compute_phy_config_regs(const unsigned int ctrl_num,
		const memctl_options_t *popts,
		const dimm_params_t *dimm_param,
		fsl_ddr_cfg_regs_t *ddr)
{
	debug("DDR PHY Debug Path\n");

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
	g_exec_fw(ctrl_num, 0);

#ifndef CONFIG_ARCH_LX2160A_PXP
	phy_io_write16(ctrl_num, 0x0d0000, 0x0);

	/* Load 2D IMEM image */
	load_image(ctrl_num, IMEM_2D);

	 /* Load 2D DMEM image into memory */
	load_image(ctrl_num, DMEM_2D);

	/* Execute training firmware and Wait for training to complete */
	g_exec_fw(ctrl_num, 1);
#endif

	/* Load Phy Init Engine image */
	phy_io_write16(ctrl_num, 0x0d0000, 0x0);
	load_image(ctrl_num, PIE);
	phy_io_write16(ctrl_num, 0x0d0000, 0x1);

#if defined(CONFIG_ARCH_LX2160A_PXP) || defined(CONFIG_DDR_FIXED_SETTINGS)
	phy_io_write16(ctrl_num, 0x0d0000, 0);
	phy_io_write16(ctrl_num, 0x0c0080, 3);
	load_image(ctrl_num, OVERRIDES);
	phy_io_write16(ctrl_num, 0x0c0080, 3);
	phy_io_write16(ctrl_num, 0x0d0000, 1);
#endif
	debug("DDR PHY Debug Path.. Done\n");
	return 0;
}
#else
static void parase_odt(const unsigned int val,
		       const bool read,
		       const int i,
		       const unsigned int cs_d0,
		       const unsigned int cs_d1,
		       unsigned int *odt)
{
	int shift = read ? 4 : 0;
	int j;

	if (i < 0 || i > 3) {
		printf("Error: invalid chip-select value\n");
	}
	switch (val) {
	case FSL_DDR_ODT_CS:
		odt[i] |= (1 << i) << shift;
		break;
	case FSL_DDR_ODT_ALL_OTHER_CS:
		for (j = 0; j < CONFIG_CHIP_SELECTS_PER_CTRL; j++) {
			if (i == j)
				continue;
			if (!((cs_d0 | cs_d1) & (1 << j)))
				continue;
			odt[j] |= (1 << i) << shift;
		}
		break;
	case FSL_DDR_ODT_CS_AND_OTHER_DIMM:
		odt[i] |= (1 << i) << 4;
		/* fall through */
	case FSL_DDR_ODT_OTHER_DIMM:
		for (j = 0; j < CONFIG_CHIP_SELECTS_PER_CTRL; j++) {
			if (((cs_d0 & (1 << i)) && (cs_d1 & (1 << j))) ||
			    ((cs_d1 & (1 << i)) && (cs_d0 & (1 << j))))
				odt[j] |= (1 << i) << shift;
		}
		break;
	case FSL_DDR_ODT_ALL:
		for (j = 0; j < CONFIG_CHIP_SELECTS_PER_CTRL; j++) {
			if (!((cs_d0 | cs_d1) & (1 << j)))
				continue;
			odt[j] |= (1 << i) << shift;
		}
		break;
	case FSL_DDR_ODT_SAME_DIMM:
		for (j = 0; j < CONFIG_CHIP_SELECTS_PER_CTRL; j++) {
			if (((cs_d0 & (1 << i)) && (cs_d0 & (1 << j))) ||
			    ((cs_d1 & (1 << i)) && (cs_d1 & (1 << j))))
				odt[j] |= (1 << i) << shift;
		}
		break;
	case FSL_DDR_ODT_OTHER_CS_ONSAMEDIMM:
		for (j = 0; j < CONFIG_CHIP_SELECTS_PER_CTRL; j++) {
			if (i == j)
				continue;
			if (((cs_d0 & (1 << i)) && (cs_d0 & (1 << j))) ||
			    ((cs_d1 & (1 << i)) && (cs_d1 & (1 << j))))
				odt[j] |= (1 << i) << shift;
		}
		break;
	case FSL_DDR_ODT_NEVER:
		break;
	default:
		break;
	}
}

int compute_phy_config_regs(const unsigned int ctrl_num,
			    const memctl_options_t *popts,
			    const dimm_params_t *dimm_param,
			    fsl_ddr_cfg_regs_t *ddr)
{
	int ret = 0;
	struct dimm *dimm;
	struct input *input;
	void *msg_1d, *msg_2d;
	size_t len;
	int i;
	unsigned int odt_rd, odt_wr;

	dimm = calloc(1, sizeof(struct dimm));
	if (!dimm) {
		printf("%s %d: Could not allocate memory\n", __func__, __LINE__);
		return -ENOMEM;
	}

	if (dimm_param) {
		dimm->dramtype = DDR4;
		/* FIXME: Add condition for LRDIMM */
		dimm->dimmtype = dimm_param->registered_dimm ? RDIMM : UDIMM;
		dimm->primary_sdram_width = dimm_param->primary_sdram_width;
		dimm->ec_sdram_width = dimm_param->ec_sdram_width;
		dimm->n_ranks = dimm_param->n_ranks;
		dimm->data_width = dimm_param->device_width;
		dimm->mirror = dimm_param->mirrored_dimm;
		dimm->mr[0] = ddr->ddr_sdram_mode & 0xffff;
		dimm->mr[1] = ddr->ddr_sdram_mode >> 16;
		dimm->mr[2] = ddr->ddr_sdram_mode_2 >> 16;
		dimm->mr[3] = ddr->ddr_sdram_mode_2 & 0xffff;
		dimm->mr[4] = ddr->ddr_sdram_mode_9 >> 16;
		dimm->mr[5] = ddr->ddr_sdram_mode_9 & 0xffff;
		dimm->mr[6] = ddr->ddr_sdram_mode_10 >> 16;
		debug("ddr_sdram_mode = 0x%x\n", ddr->ddr_sdram_mode);
		debug("ddr_sdram_mode_2 = 0x%x\n", ddr->ddr_sdram_mode_2);
		debug("ddr_sdram_mode_9 = 0x%x\n", ddr->ddr_sdram_mode_9);
		debug("ddr_sdram_mode_10 = 0x%x\n", ddr->ddr_sdram_mode_10);
		dimm->cs_d0 = popts->cs_d0;
		dimm->cs_d1 = popts->cs_d1;
		for (i = 0; i < CONFIG_CHIP_SELECTS_PER_CTRL; i++) {
			if (!(ddr->cs[i].config & SDRAM_CS_CONFIG_EN))
				continue;
			odt_rd = (ddr->cs[i].config >> 20) & 0x7;
			odt_wr = (ddr->cs[i].config >> 16) & 0x7;
			parase_odt(odt_rd, true, i, dimm->cs_d0, dimm->cs_d1,
				   dimm->odt);
			parase_odt(odt_wr, false, i, dimm->cs_d0, dimm->cs_d1,
				   dimm->odt);
		}

		/* Do not set sdram_cfg[RD_EN] or sdram_cfg2[RCW_EN] for RDIMM */
		if (dimm_param->registered_dimm) {
			ddr->ddr_sdram_cfg &= ~(1 << 28);
			ddr->ddr_sdram_cfg_2 &= ~(1 << 2);
			dimm->rcw[0] = (ddr->ddr_sdram_rcw_1 >> 28) & 0xf;
			dimm->rcw[1] = (ddr->ddr_sdram_rcw_1 >> 24) & 0xf;
			dimm->rcw[2] = (ddr->ddr_sdram_rcw_1 >> 20) & 0xf;
			dimm->rcw[3] = (ddr->ddr_sdram_rcw_1 >> 16) & 0xf;
			dimm->rcw[4] = (ddr->ddr_sdram_rcw_1 >> 12) & 0xf;
			dimm->rcw[5] = (ddr->ddr_sdram_rcw_1 >> 8) & 0xf;
			dimm->rcw[6] = (ddr->ddr_sdram_rcw_1 >> 4) & 0xf;
			dimm->rcw[7] = (ddr->ddr_sdram_rcw_1 >> 0) & 0xf;
			dimm->rcw[8] = (ddr->ddr_sdram_rcw_2 >> 28) & 0xf;
			dimm->rcw[9] = (ddr->ddr_sdram_rcw_2 >> 24) & 0xf;
			dimm->rcw[10] = (ddr->ddr_sdram_rcw_2 >> 20) & 0xf;
			dimm->rcw[11] = (ddr->ddr_sdram_rcw_2 >> 16) & 0xf;
			dimm->rcw[12] = (ddr->ddr_sdram_rcw_2 >> 12) & 0xf;
			dimm->rcw[13] = (ddr->ddr_sdram_rcw_2 >> 8) & 0xf;
			dimm->rcw[14] = (ddr->ddr_sdram_rcw_2 >> 4) & 0xf;
			dimm->rcw[15] = (ddr->ddr_sdram_rcw_2 >> 0) & 0xf;
			dimm->rcw3x = (ddr->ddr_sdram_rcw_3 >> 8) & 0xff;
		}
	} else {
		printf("Error: empty DIMM parameters.\n");
		ret = -EINVAL;
		goto err_dimm;
	}
	debug("Initializing input data structure\n");
	input = phy_gen2_init_input(ctrl_num, dimm,
				(get_ddr_freq(ctrl_num) + 1000000) / 2000000);
	if (!input) {
		printf("%s %d: Could not allocate memory\n", __func__, __LINE__);
		ret = -ENOMEM;
		goto err_dimm;
	}

	debug("Initializing message block\n");
	ret = phy_gen2_msg_init(ctrl_num, &msg_1d, &msg_2d, input, &len);
	if (ret) {
		printf("%s %d: Error init msg\n", __func__, __LINE__);
		goto err_input;
	}

	debug("Initialize PHY config\n");
	ret = c_init_phy_config(ctrl_num, input, msg_1d);
	if (ret) {
		printf("Error in step C\n");
		goto err_msg_block;
	}

	debug("Load firmware\n");
	ret = phy_gen2_dimm_train(ctrl_num, input, 0, msg_1d, len); /* train 1D first */
	if (ret) {
		printf("%s %d: Error training\n", __func__, __LINE__);
		goto err_msg_block;
	}

	/* FIXME: How to determine training firmware is done? */
	debug("Execute firmware\n");
	ret = g_exec_fw(ctrl_num, 0);
	/* FIXME: add 2D training parameter later */
	debug("Read message block\n");
	h_readmsgblock(ctrl_num);

	if (!ret && input->basic.train2d) {		/* 2D training starts here */
		debug("Load 2D firmware\n");
		ret = phy_gen2_dimm_train(ctrl_num, input, 1, msg_2d, len); /* train 2D */
			if (ret) {
				printf("%s %d: Error training\n", __func__, __LINE__);
				goto err_msg_block;
			}
		debug("Execute 2D firmware\n");
		ret = g_exec_fw(ctrl_num, 1);
		/* FIXME: add 2D training parameter later */
		debug("Read 2D message block\n");
		h_readmsgblock(ctrl_num);
	}

	if (!ret) {
		debug("Load PIE\n");
		i_load_pie(ctrl_num, input, msg_1d);

		printf("DDR4(%d) %s with %d-rank %d-bit bus (x%d)\n", ctrl_num,
		       dimm->dimmtype == RDIMM ? "RDIMM" :
		       dimm->dimmtype == LRDIMM ? "LRDIMM" : "UDIMM",
		       dimm->n_ranks, dimm->primary_sdram_width,
		       dimm->data_width);
	}

#ifdef CONFIG_ARCH_LX2160A_PXP
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 0);
	prog_tx_odt_drv_stren(ctrl_num, input);
	phy_io_write16(ctrl_num, t_apbonly | csr_micro_cont_mux_sel_addr, 1);
#endif

err_msg_block:
	if (input->basic.train2d)
		free(msg_2d);
	free(msg_1d);
err_input:
	free(input);
err_dimm:
	free(dimm);

	return ret;
}
#endif
