/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <fsl_ddr_sdram.h>
#include <fsl_ddr_dimm_params.h>
#include <asm/arch/soc.h>
#include <asm/arch/clock.h>
#include <asm/io.h>
#include "ddr.h"

DECLARE_GLOBAL_DATA_PTR;

void fsl_ddr_board_options(memctl_options_t *popts,
			   dimm_params_t *pdimm,
			   unsigned int ctrl_num)
{
	const struct board_specific_parameters *pbsp, *pbsp_highest = NULL;
	ulong ddr_freq;

	if (ctrl_num > 1) {
		printf("Not supported controller number %d\n", ctrl_num);
		return;
	}
	if (!pdimm->n_ranks)
		return;

	pbsp = udimms[0];

	/* Get clk_adjust, wrlvl_start, wrlvl_ctl, according to the board ddr
	 * freqency and n_banks specified in board_specific_parameters table.
	 */
	ddr_freq = get_ddr_freq(0) / 1000000;
	while (pbsp->datarate_mhz_high) {
		if (pbsp->n_ranks == pdimm->n_ranks) {
			if (ddr_freq <= pbsp->datarate_mhz_high) {
				popts->clk_adjust = pbsp->clk_adjust;
				popts->wrlvl_start = pbsp->wrlvl_start;
				popts->wrlvl_ctl_2 = pbsp->wrlvl_ctl_2;
				popts->wrlvl_ctl_3 = pbsp->wrlvl_ctl_3;
				popts->cpo_override = pbsp->cpo_override;
				popts->write_data_delay =
					pbsp->write_data_delay;
				goto found;
			}
			pbsp_highest = pbsp;
		}
		pbsp++;
	}

	if (pbsp_highest) {
		printf("Error: board specific timing not found for %lu MT/s\n",
		       ddr_freq);
		printf("Trying to use the highest speed (%u) parameters\n",
		       pbsp_highest->datarate_mhz_high);
		popts->clk_adjust = pbsp_highest->clk_adjust;
		popts->wrlvl_start = pbsp_highest->wrlvl_start;
		popts->wrlvl_ctl_2 = pbsp->wrlvl_ctl_2;
		popts->wrlvl_ctl_3 = pbsp->wrlvl_ctl_3;
	} else {
		panic("DIMM is not supported by this board");
	}
found:
	debug("Found timing match: n_ranks %d, data rate %d, rank_gb %d\n",
	      pbsp->n_ranks, pbsp->datarate_mhz_high, pbsp->rank_gb);

	/* force DDR bus width to 32 bits */
	popts->data_bus_width = 1;
	popts->otf_burst_chop_en = 0;
	popts->burst_length = DDR_BL8;
	popts->bstopre = 0;		/* enable auto precharge */

	/*
	 * Factors to consider for half-strength driver enable:
	 *	- number of DIMMs installed
	 */
	popts->half_strength_driver_enable = 1;
	/*
	 * Write leveling override
	 */
	popts->wrlvl_override = 1;
	popts->wrlvl_sample = 0xf;

	/*
	 * Rtt and Rtt_WR override
	 */
	popts->rtt_override = 0;

	/* Enable ZQ calibration */
	popts->zq_en = 1;

#ifdef CONFIG_SYS_FSL_DDR4
	popts->ddr_cdr1 = DDR_CDR1_DHC_EN | DDR_CDR1_ODT(DDR_CDR_ODT_80ohm);
	popts->ddr_cdr2 = DDR_CDR2_ODT(DDR_CDR_ODT_80ohm) |
			  DDR_CDR2_VREF_OVRD(70);	/* Vref = 70% */

	/* optimize cpo for erratum A-009942 */
	popts->cpo_sample = 0x46;
#else
	popts->cswl_override = DDR_CSWL_CS0;

	/* DHC_EN =1, ODT = 75 Ohm */
	popts->ddr_cdr1 = DDR_CDR1_DHC_EN | DDR_CDR1_ODT(DDR_CDR_ODT_75ohm);
	popts->ddr_cdr2 = DDR_CDR2_ODT(DDR_CDR_ODT_75ohm);
#endif
}


#ifdef CONFIG_TARGET_LS1028ARDB
#ifdef CONFIG_SYS_DDR_RAW_TIMING
/* MT40A1G8SA-075:E */
dimm_params_t ddr_raw_timing = {
	.n_ranks = 1,
	.rank_density = 4294967296u,
	.capacity = 4294967296u,
	.primary_sdram_width = 32,
	.ec_sdram_width = 4,
	.registered_dimm = 0,
	.mirrored_dimm = 0,
	.n_row_addr = 16,
	.n_col_addr = 10,
	.bank_addr_bits = 0,
	.bank_group_bits = 2,
	.edc_config = 2,
	.burst_lengths_bitmask = 0x0c,

	.tckmin_x_ps = 750,
	.tckmax_ps = 1900,
	.caslat_x = 0x0001FFE00,
	.taa_ps = 13500,
	.trcd_ps = 13500,
	.trp_ps = 13500,
	.tras_ps = 32000,
	.trc_ps = 45500,
	.trfc1_ps = 350000,
	.trfc2_ps = 260000,
	.trfc4_ps = 160000,
	.tfaw_ps = 21000,
	.trrds_ps = 3000,
	.trrdl_ps = 4900,
	.tccdl_ps = 5000,
	.refresh_rate_ps = 7800000,
	.dq_mapping[0] = 0x16,
	.dq_mapping[1] = 0x36,
	.dq_mapping[2] = 0x16,
	.dq_mapping[3] = 0x36,
	.dq_mapping[4] = 0x16,
	.dq_mapping[5] = 0x36,
	.dq_mapping[6] = 0x16,
	.dq_mapping[7] = 0x36,
	.dq_mapping[8] = 0x16,
	.dq_mapping[9] = 0x0,
	.dq_mapping[10] = 0x0,
	.dq_mapping[11] = 0x0,
	.dq_mapping[12] = 0x0,
	.dq_mapping[13] = 0x0,
	.dq_mapping[14] = 0x0,
	.dq_mapping[15] = 0x0,
	.dq_mapping[16] = 0x0,
	.dq_mapping[17] = 0x0,
	.dq_mapping_ors = 0,
};

int fsl_ddr_get_dimm_params(dimm_params_t *pdimm,
			    unsigned int controller_number,
			    unsigned int dimm_number)
{
	static const char dimm_model[] = "Fixed DDR on board";

	if ((controller_number == 0) && (dimm_number == 0)) {
		memcpy(pdimm, &ddr_raw_timing, sizeof(dimm_params_t));
		memset(pdimm->mpart, 0, sizeof(pdimm->mpart));
		memcpy(pdimm->mpart, dimm_model, sizeof(dimm_model) - 1);
	}

	return 0;
}
#else
static phys_size_t fixed_sdram(void)
{
	size_t ddr_size;

#if defined(CONFIG_SPL_BUILD) || !defined(CONFIG_SPL)
	fsl_ddr_cfg_regs_t ddr_cfg_regs = {
		.cs[0].bnds		= 0x000000ff,
		.cs[0].config		= 0x80040422,
		.cs[0].config_2		= 0,
		.cs[1].bnds		= 0,
		.cs[1].config		= 0,
		.cs[1].config_2		= 0,

		.timing_cfg_3		= 0x01111000,
		.timing_cfg_0		= 0xd0550018,
		.timing_cfg_1		= 0xFAFC0C42,
		.timing_cfg_2		= 0x0048c114,
		.ddr_sdram_cfg		= 0xe50c000c,
		.ddr_sdram_cfg_2	= 0x00401110,
		.ddr_sdram_mode		= 0x01010230,
		.ddr_sdram_mode_2	= 0x0,

		.ddr_sdram_md_cntl	= 0x0600001f,
		.ddr_sdram_interval	= 0x18600618,
		.ddr_data_init		= 0xdeadbeef,

		.ddr_sdram_clk_cntl	= 0x02000000,
		.ddr_init_addr		= 0,
		.ddr_init_ext_addr	= 0,

		.timing_cfg_4		= 0x00000002,
		.timing_cfg_5		= 0x07401400,
		.timing_cfg_6		= 0x0,
		.timing_cfg_7		= 0x23300000,

		.ddr_zq_cntl		= 0x8A090705,
		.ddr_wrlvl_cntl		= 0x86550607,
		.ddr_sr_cntr		= 0,
		.ddr_sdram_rcw_1	= 0,
		.ddr_sdram_rcw_2	= 0,
		.ddr_wrlvl_cntl_2	= 0x0708080A,
		.ddr_wrlvl_cntl_3	= 0x0A0B0C09,

		.ddr_sdram_mode_9	= 0x00000400,
		.ddr_sdram_mode_10	= 0x04000000,

		.timing_cfg_8		= 0x06115600,

		.dq_map_0		= 0x5b65b658,
		.dq_map_1		= 0xd96d8000,
		.dq_map_2		= 0,
		.dq_map_3		= 0x01600000,

		.ddr_cdr1		= 0x80040000,
		.ddr_cdr2		= 0x000000C1
	};


	fsl_ddr_set_memctl_regs(&ddr_cfg_regs, 0, 0);
#endif
	ddr_size = 1ULL << 32;

	return ddr_size;
}
#endif	/* CONFIG_SYS_DDR_RAW_TIMING */
#endif

int fsl_initdram(void)
{
#ifdef CONFIG_TARGET_LS1028ARDB
#if defined(CONFIG_SPL) && !defined(CONFIG_SPL_BUILD)
	gd->ram_size = 1ULL << 32;
#else
#ifdef CONFIG_SYS_DDR_RAW_TIMING
	puts("Initializing DDR....\n");
	gd->ram_size = fsl_ddr_sdram();
#else
	puts("Initializing DDR....using fixed timing\n");
	gd->ram_size = fixed_sdram();
#endif	/* CONFIG_SYS_DDR_RAW_TIMING */
#endif
#else
#if defined(CONFIG_SPL) && !defined(CONFIG_SPL_BUILD)
	gd->ram_size = fsl_ddr_sdram_size();
#else
	puts("Initializing DDR....using SPD\n");
	gd->ram_size = fsl_ddr_sdram();
#endif
#endif /* !CONFIG_TARGET_LS1028ARDB */
	return 0;
 }

