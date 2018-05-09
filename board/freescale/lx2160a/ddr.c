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
#if defined(CONFIG_ARCH_LX2160A_PXP) || defined(CONFIG_DDR_FIXED_SETTINGS)
#include <asm/io.h>
#include <fsl_ddr.h>
#endif
#include "ddr.h"

DECLARE_GLOBAL_DATA_PTR;

void fsl_ddr_board_options(memctl_options_t *popts,
			   dimm_params_t *pdimm,
			   unsigned int ctrl_num)
{
	const struct board_specific_parameters *pbsp, *pbsp_highest = NULL;
	ulong ddr_freq;
	int slot;

	if (ctrl_num > CONFIG_SYS_FSL_DDR_MAIN_NUM_CTRLS) {
		printf("Not supported controller number %d\n", ctrl_num);
		return;
	}

	for (slot = 0; slot < CONFIG_DIMM_SLOTS_PER_CTLR; slot++) {
		if (pdimm[slot].n_ranks)
			break;
	}

	if (slot >= CONFIG_DIMM_SLOTS_PER_CTLR)
		return;

	/*
	 * we use identical timing for all slots. If needed, change the code
	 * to  pbsp = rdimms[ctrl_num] or pbsp = udimms[ctrl_num];
	 */
	if (popts->registered_dimm_en)
		pbsp = rdimms[ctrl_num];
	else
		pbsp = udimms[ctrl_num];

	/* Get clk_adjust, wrlvl_start, wrlvl_ctl, according to the board ddr
	 * freqency and n_banks specified in board_specific_parameters table.
	 */
	ddr_freq = get_ddr_freq(ctrl_num) / 1000000;
	while (pbsp->datarate_mhz_high) {
		if (pbsp->n_ranks == pdimm[slot].n_ranks &&
		    (pdimm[slot].rank_density >> 30) >= pbsp->rank_gb) {
			if (ddr_freq <= pbsp->datarate_mhz_high) {
				popts->clk_adjust = pbsp->clk_adjust;
				popts->wrlvl_start = pbsp->wrlvl_start;
				popts->wrlvl_ctl_2 = pbsp->wrlvl_ctl_2;
				popts->wrlvl_ctl_3 = pbsp->wrlvl_ctl_3;
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
	debug("Found timing match: n_ranks %d, data rate %d, rank_gb %d\n"
		"\tclk_adjust %d, wrlvl_start %d, wrlvl_ctrl_2 0x%x, wrlvl_ctrl_3 0x%x\n",
		pbsp->n_ranks, pbsp->datarate_mhz_high, pbsp->rank_gb,
		pbsp->clk_adjust, pbsp->wrlvl_start, pbsp->wrlvl_ctl_2,
		pbsp->wrlvl_ctl_3);
	/* To work at higher than 1333MT/s */
	popts->half_strength_driver_enable = 0;
	/*
	 * Write leveling override
	 */
	popts->wrlvl_override = 1;
	popts->wrlvl_sample = 0x0;	/* 32 clocks */

	/*
	 * Rtt and Rtt_WR override
	 */
	popts->rtt_override = 0;

	/* Enable ZQ calibration */
	popts->zq_en = 1;

	/* optimize cpo for erratum A-009942 */
	popts->cpo_sample = 0x6e;

	if (ddr_freq < 2350) {
		if (pdimm[0].n_ranks == 2 && pdimm[1].n_ranks == 2) {
			/* four chip-selects */
			popts->ddr_cdr1 = DDR_CDR1_DHC_EN |
					  DDR_CDR1_ODT(DDR_CDR_ODT_80ohm);
			popts->ddr_cdr2 = DDR_CDR2_ODT(DDR_CDR_ODT_80ohm);
			popts->twot_en = 1;	/* enable 2T timing */
		} else {
			popts->ddr_cdr1 = DDR_CDR1_DHC_EN |
					  DDR_CDR1_ODT(DDR_CDR_ODT_60ohm);
			popts->ddr_cdr2 = DDR_CDR2_ODT(DDR_CDR_ODT_60ohm) |
					  DDR_CDR2_VREF_RANGE_2;
		}
	} else {
		popts->ddr_cdr1 = DDR_CDR1_DHC_EN |
				  DDR_CDR1_ODT(DDR_CDR_ODT_100ohm);
		popts->ddr_cdr2 = DDR_CDR2_ODT(DDR_CDR_ODT_100ohm) |
				  DDR_CDR2_VREF_RANGE_2;
	}
}

#if defined(CONFIG_ARCH_LX2160A_PXP) || defined(CONFIG_DDR_FIXED_SETTINGS)
#ifdef CONFIG_ARCH_LX2160A_PXP
static void ddr_cntlr_fixed_settings(void)
{
#if 1
	puts("Setting sysbardisable to 1; hni_pos and early_wr_comp to 0 (04D7)\n");
	out_le32(0x1e00620,0x80000000);
	out_le32(0x4080000,0x0);
	out_le32(0x4090000,0x0);
	out_le32(0x4080500,0x000004D7);
	out_le32(0x4090500,0x000004D7);

	puts("CCN HN-F snoop domains -> 0x000800 for 1 cluster!\n");
	out_le32(0x4200210,0x000800);
	out_le32(0x4210210,0x000800);
	out_le32(0x4220210,0x000800);
	out_le32(0x4230210,0x000800);
	out_le32(0x4240210,0x000800);
	out_le32(0x4250210,0x000800);
	out_le32(0x4260210,0x000800);
	out_le32(0x4270210,0x000800);

	puts("CCN HN-F -> SBSX 3, 8 mapping\n");
	out_le32(0x4200008,0x00000003);
	out_le32(0x4210008,0x00000003);
	out_le32(0x4220008,0x00000003);
	out_le32(0x4230008,0x00000003);

	out_le32(0x4240008,0x00000008);
	out_le32(0x4250008,0x00000008);
	out_le32(0x4260008,0x00000008);
	out_le32(0x4270008,0x00000008);
#endif
	out_le32(0x01080000,0x000001ff);
	out_le32(0x01080008,0x000001ff);
	out_le32(0x01080010,0x020002ff);
	out_le32(0x01080018,0x030003ff);
	out_le32(0x01080040,0xfcfcfc80);
	out_le32(0x01080044,0x7c787470);
	out_le32(0x01080048,0x6c686460);
	out_le32(0x0108004c,0x5c585450);
	out_le32(0x01080050,0x4c48fc34);
	out_le32(0x01080054,0x302c2824);
	out_le32(0x01080058,0x201c1410);
	out_le32(0x0108005c,0x0c403cfc);
	out_le32(0x01080060,0xfcfc4438);
	out_le32(0x01080064,0x18000001);
	out_le32(0x01080080,0x80000322);
	out_le32(0x01080084,0x80000322);
	out_le32(0x01080088,0x00000322);
	out_le32(0x0108008C,0x00000322);
	out_le32(0x01080104,0x37c5c01c);
	out_le32(0x01080108,0x44478844);
	out_le32(0x0108010C,0x0005c9e6);
	out_le32(0x01080100,0x134f2100);
	out_le32(0x01080160,0x00000001);
	out_le32(0x01080164,0x01203200);
	out_le32(0x01080168,0x00000000);
	out_le32(0x0108016C,0x3bb00000);
	out_le32(0x01080250,0x00448c00);
	out_le32(0x01080260,0x00000000);
	out_le32(0x01080170,0x8a09070f);
	out_le32(0x0108017C,0x00000000);
	out_le32(0x01080114,0x00400100);
	out_le32(0x01080118,0x00010c44);
	out_le32(0x0108011C,0x00280000);
	out_le32(0x01080204,0x00280000);
	out_le32(0x0108020C,0x00280000);
	out_le32(0x01080214,0x00280000);
	out_le32(0x01080200,0x00010c44);
	out_le32(0x01080208,0x00010c44);
	out_le32(0x01080210,0x00010c44);
	out_le32(0x01080220,0x00000400);
	out_le32(0x01080224,0x04000000);
	out_le32(0x01080228,0x00000400);
	out_le32(0x0108022C,0x04000000);
	out_le32(0x01080230,0x00000400);
	out_le32(0x01080234,0x04000000);
	out_le32(0x01080238,0x00000400);
	out_le32(0x0108023C,0x04000000);
	out_le32(0x01080124,0x30c00200);
	out_le32(0x01080124,0x30c00080);
	out_le32(0x01080128,0xa5a55a5a);
	out_le32(0x01080E44,0x00000000);
	out_le32(0x01080E48,0x00000000);
	out_le32(0x01080E58,0x00ba0000);
	out_le32(0x01080F04,0x00000080);
	out_le32(0x01080F48,0x00200000);
#if 0
	out_le32(0x01080110,0xc5044000); /* SDRAM_CFG [BI] = 0 */
#else
	out_le32(0x01080110,0xc5044001); /* SDRAM_CFG [BI] = 1 */
#endif

#if 1
	// Programming TZC memory
	puts("Programming TZC1 memory\n");
	out_le32(0x01100004,0x1);
	out_le32(0x01100110,0xc0000000);
	out_le32(0x01100114,0xffffffff);
	out_le32(0x01100128,0xfffff000);
	out_le32(0x0110012C,0xff);
	out_le32(0x01100130,0xc0000001);
	out_le32(0x01100134,0xffffffff);
	out_le32(0x01100008,0x1);

	puts("Programming TZC1_1 memory\n");
	out_le32(0x01110004,0x1);
	out_le32(0x01110110,0xc0000000);
	out_le32(0x01110114,0xffffffff);
	out_le32(0x01110128,0xfffff000);
	out_le32(0x0111012C,0xff);
	out_le32(0x01110130,0xc0000001);
	out_le32(0x01110134,0xffffffff);
	out_le32(0x01110008,0x1);

	puts("Programming TZC2 memory\n");
	out_le32(0x01120004,0x1);
	out_le32(0x01120110,0xc0000000);
	out_le32(0x01120114,0xffffffff);
	out_le32(0x01120128,0xfffff000);
	out_le32(0x0112012C,0xff);
	out_le32(0x01120130,0xc0000001);
	out_le32(0x01120134,0xffffffff);
	out_le32(0x01120008,0x1);

	puts("Programming TZC2_2 memory\n");
	out_le32(0x01130004,0x1);
	out_le32(0x01130110,0xc0000000);
	out_le32(0x01130114,0xffffffff);
	out_le32(0x01130128,0xfffff000);
	out_le32(0x0113012C,0xff);
	out_le32(0x01130130,0xc0000001);
	out_le32(0x01130134,0xffffffff);
	out_le32(0x01130008,0x1);
#endif
}
#else
static void ddr_cntlr_fixed_settings(void)
{
	puts("Setting Hardcoded values for DDRC for 1600 frequency\n");
	out_le32(0x01080000,0x000000ff);	/* CS0_BNDS */
	out_le32(0x01080008,0x010001ff);	/* CS1_BNDS */
	out_le32(0x01080010,0x00000000);	/* CS2_BNDS */
	out_le32(0x01080018,0x00000000);	/* CS3_BNDS */
	out_le32(0x01080080,0x80020322);	/* CS0_CONFIG */
	out_le32(0x01080084,0x80020322);	/* CS1_CONFIG */
	out_le32(0x01080088,0x00000000);	/* CS2_CONFIG */
	out_le32(0x0108008C,0x00000000);	/* CS3_CONFIG */
	out_le32(0x01080100,0x010c1000);	/* TIMING_CFG_3 */
	out_le32(0x01080104,0xfa550018);	/* TIMING_CFG_0 */
	out_le32(0x01080108,0xbab48c42);	/* TIMING_CFG_1 */
	out_le32(0x0108010C,0x0058c111);	/* TIMING_CFG_2 */
	out_le32(0x01080110,0xe5040001);	/* DDR_SDRAM_CFG */
	out_le32(0x01080114,0x00401101);	/* DDR_SDRAM_CFG_2 */
	out_le32(0x01080124,0x18600100);	/* DDR_SDRAM_INTERVAL */
	out_le32(0x01080160,0x00000002);	/* TIMING_CFG_4 */
	out_le32(0x01080164,0x03401400);	/* TIMING_CFG_5 */
	out_le32(0x01080168,0x00000000);	/* TIMING_CFG_6 */
	out_le32(0x0108016C,0x13300000);	/* TIMING_CFG_7 */
	out_le32(0x01080170,0x8a090705);	/* DDR_ZQ_CNTL */
	out_le32(0x01080250,0x00004600);	/* TIMING_CFG_8 */
	out_le32(0x01080254,0x00000000);	/* TIMING_CFG_9 */
	out_le32(0x01080260,0x00000001);	/* DDR_SDRAM_CFG_3 */
	out_le32(0x01080400,0x32C57554);	/* DQ_MAPPING_0 */
	out_le32(0x01080404,0xD4BB0BD4);	/* DQ_MAPPING_1 */
	out_le32(0x01080408,0x2EC2F554);	/* DQ_MAPPING_2 */
	out_le32(0x0108040C,0xD95D4001);	/* DQ_MAPPING_3 */
}
#endif
#endif

int fsl_initdram(void)
{
#ifndef CONFIG_SYS_PEB_BOOT
#if defined(CONFIG_SPL) && !defined(CONFIG_SPL_BUILD)
	gd->ram_size = fsl_ddr_sdram_size();
#else
#ifndef CONFIG_DDR_BOOT
#if defined(CONFIG_ARCH_LX2160A_PXP) || defined(CONFIG_DDR_FIXED_SETTINGS)
	puts("Initializing DDR....using fixed timing\n");
	compute_phy_config_regs(0, NULL, NULL);
	ddr_cntlr_fixed_settings();
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
#else
	puts("Initializing DDR....using SPD\n");

	gd->ram_size = fsl_ddr_sdram();
#endif
#else
	puts("DDR initialized via backend\n");
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	puts("Limiting dram_size to SDRAM_SIZE.\n");
#endif
#endif
#else
	gd->ram_size = (phys_size_t)CONFIG_SYS_PEB_SIZE;
	puts("Limiting dram size to PEB memsize\n");
#endif

	return 0;
}
