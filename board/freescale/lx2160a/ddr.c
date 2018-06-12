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

DECLARE_GLOBAL_DATA_PTR;

void fsl_ddr_board_options(memctl_options_t *popts,
			   dimm_params_t *pdimm,
			   unsigned int ctrl_num)
{
	/*
	 * Rtt and Rtt_WR override
	 */
	popts->rtt_override = 0;
	popts->rtt_park = 120;

	/* Enable ZQ calibration */
	popts->zq_en = 1;

	popts->otf_burst_chop_en = 0;
	popts->burst_length = DDR_BL8;
	popts->trwt_override = 1;
	popts->trwt = 0x3;
	popts->twrt = 0x3;
	popts->trrt = 0x3;
	popts->twwt = 0x3;
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
	puts("DDRC: 1600 MT/s\n");
	out_le32(0x01080000,0x000001ff);
	out_le32(0x01080008,0x000001ff);
	out_le32(0x01080010,0x020002ff);
	out_le32(0x01080018,0x030003ff);
	out_le32(0x01080080,0x80040322);
	out_le32(0x01080088,0x80000322);
	out_le32(0x0108008C,0x00000322);
	out_le32(0x01080104,0xff550018);
	out_le32(0x01080108,0xbab48c42);
	out_le32(0x0108010C,0x0058c111);
	out_le32(0x01080100,0x010c1000);
	out_le32(0x01080160,0x00000002);
	out_le32(0x01080164,0x03401400);
	out_le32(0x01080168,0x00000000);
	out_le32(0x0108016C,0x13300000);
	out_le32(0x01080250,0x00004600);
	out_le32(0x01080260,0x00000001);
	out_le32(0x01080170,0x8a090705);
	out_le32(0x0108017C,0x00000000);
	out_le32(0x01080114,0x00401101);
	out_le32(0x01080124,0x163c0000);
	out_le32(0x01080128,0xa5a55a5a);
	out_le32(0x01080E44,0x00000000);
	out_le32(0x01080E48,0x00000000);
	out_le32(0x01080E58,0x00ba0000);
	out_le32(0x01080400,0x32c57554);
	out_le32(0x01080404,0xd4bb0bd4);
	out_le32(0x01080408,0x2ec2f554);
	out_le32(0x0108040c,0xd95d4001);
	out_le32(0x01080D20,0xAA55AA55);
	out_le32(0x01080D24,0x11331133);
	out_le32(0x01080D28,0x00000000);
	out_le32(0x01080D2c,0xFFFFFFFF);
	out_le32(0x01080D30,0x12345678);
	out_le32(0x01080D34,0xdeadbeef);
	out_le32(0x01080D38,0xbbeebbee);
	out_le32(0x01080D3c,0xccddccdd);
	out_le32(0x01080D40,0xcdaccdac);
	out_le32(0x01080D44,0x11223344);
	out_le32(0x01080110,0xc5044001);
	out_le32(0x01100004,0x1);
	out_le32(0x01100110,0xc0000000);
	out_le32(0x01100114,0xffffffff);
	out_le32(0x01100128,0xfffff000);
	out_le32(0x0110012C,0xff);
	out_le32(0x01100130,0xc0000001);
	out_le32(0x01100134,0xffffffff);
	out_le32(0x01100008,0x1);
	out_le32(0x01110004,0x1);
	out_le32(0x01110110,0xc0000000);
	out_le32(0x01110114,0xffffffff);
	out_le32(0x01110128,0xfffff000);
	out_le32(0x0111012C,0xff);
	out_le32(0x01110130,0xc0000001);
	out_le32(0x01110134,0xffffffff);
	out_le32(0x01110008,0x1);
	out_le32(0x01120004,0x1);
	out_le32(0x01120110,0xc0000000);
	out_le32(0x01120114,0xffffffff);
	out_le32(0x01120128,0xfffff000);
	out_le32(0x0112012C,0xff);
	out_le32(0x01120130,0xc0000001);
	out_le32(0x01120134,0xffffffff);
	out_le32(0x01120008,0x1);
	out_le32(0x01130004,0x1);
	out_le32(0x01130110,0xc0000000);
	out_le32(0x01130114,0xffffffff);
	out_le32(0x01130128,0xfffff000);
	out_le32(0x0113012C,0xff);
	out_le32(0x01130130,0xc0000001);
	out_le32(0x01130134,0xffffffff);
	out_le32(0x01130008,0x1);
	out_le32(0x04200008,0x8);
	out_le32(0x04210008,0x8);
	out_le32(0x04220008,0x8);
	out_le32(0x04230008,0x8);
	out_le32(0x04240008,0x8);
	out_le32(0x04250008,0x8);
	out_le32(0x04260008,0x8);
	out_le32(0x04270008,0x8);
	out_le32(0x80000000,0xabcd1234);
	out_le32(0x80000010,0xabcd1234);
	out_le32(0x80000000,0x56789abc);
	out_le32(0x80000010,0x56789abc);
	out_le32(0x80000004,0xabcd1234);
	out_le32(0x80000014,0xabcd1234);
	out_le32(0x80000004,0x56789abc);
	out_le32(0x80000014,0x56789abc);
	out_le32(0x80000100,0xabcd1234);
	out_le32(0x80001000,0xabcd1234);
}
#endif
#endif

#ifdef CONFIG_SYS_DDR_RAW_TIMING
#if 1
/* MT18ASF1G72AZ-2G6B1 */
dimm_params_t ddr_raw_timing = {
	.n_ranks = 2,
	.rank_density = 4294967296u,
	.capacity = 8589934592u,
	.primary_sdram_width = 64,
	.ec_sdram_width = 8,
	.data_width = 72,
	.device_width = 8,
	.die_density = 0x4,
	.registered_dimm = 0,
	.mirrored_dimm = 1,
	.n_row_addr = 15,
	.n_col_addr = 10,
	.bank_addr_bits = 0,
	.bank_group_bits = 2,
	.edc_config = 2,
	.burst_lengths_bitmask = 0x0c,

	.tckmin_x_ps = 750,
	.tckmax_ps = 1600,
	.caslat_x = 0x00FFFC00,
	.taa_ps = 13750,
	.trcd_ps = 13750,
	.trp_ps = 13750,
	.tras_ps = 32000,
	.trc_ps = 45750,
	.trfc1_ps = 260000,
	.trfc2_ps = 160000,
	.trfc4_ps = 110000,
	.tfaw_ps = 21000,
	.trrds_ps = 3000,
	.trrdl_ps = 4900,
	.tccdl_ps = 5000,
	.refresh_rate_ps = 7800000,
};

#else
/* MT36ADS4G72PZ-2G3B1 */
dimm_params_t ddr_raw_timing = {
	.n_ranks = 2,
	.rank_density = 17179869184u,
	.capacity = 34359738368u,
	.primary_sdram_width = 64,
	.ec_sdram_width = 8,
	.data_width = 72,
	.device_width = 4,
	.die_density = 0x5,
	.registered_dimm = 1,
	.mirrored_dimm = 0,
	.n_row_addr = 17,
	.n_col_addr = 10,
	.bank_addr_bits = 0,
	.bank_group_bits = 2,
	.edc_config = 2,
	.burst_lengths_bitmask = 0x0c,

	.tckmin_x_ps = 833,
	.tckmax_ps = 1600,
	.caslat_x = 0x0007FC00,
	.taa_ps = 13750,
	.trcd_ps = 13750,
	.trp_ps = 13750,
	.tras_ps = 32000,
	.trc_ps = 45750,
	.trfc1_ps = 350000,
	.trfc2_ps = 260000,
	.trfc4_ps = 160000,
	.tfaw_ps = 13000,
	.trrds_ps = 3300,
	.trrdl_ps = 4900,
	.tccdl_ps = 5000,
	.refresh_rate_ps = 7800000,
};
#endif

int fsl_ddr_get_dimm_params(dimm_params_t *pdimm,
			    unsigned int controller_number,
			    unsigned int dimm_number)
{
	static const char dimm_model[] = "Fixed DDR on board";

	if (((controller_number == 0) && (dimm_number == 0)) ||
	    ((controller_number == 1) && (dimm_number == 0))) {
		memcpy(pdimm, &ddr_raw_timing, sizeof(dimm_params_t));
		memset(pdimm->mpart, 0, sizeof(pdimm->mpart));
		memcpy(pdimm->mpart, dimm_model, sizeof(dimm_model) - 1);
	}

	return 0;
}
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
	compute_phy_config_regs(0, NULL, NULL, NULL);
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
