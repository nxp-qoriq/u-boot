/*
 * Copyright 2017-2018 NXP.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <netdev.h>
#include <fsl_ddr.h>
#include <asm/io.h>
#include <hwconfig.h>
#include <fdt_support.h>
#include <libfdt.h>
#include <fsl-mc/fsl_mc.h>
#include <environment.h>
#include <efi_loader.h>
#include <i2c.h>
#include <asm/arch/mmu.h>
#include <asm/arch/soc.h>
#include <fsl_sec.h>
#ifdef CONFIG_FSL_LS_PPA
#include <asm/arch/ppa.h>
#endif

#ifdef CONFIG_FSL_QIXIS
#include "../common/qixis.h"
#include "lx2160ardb_qixis.h"
#endif
#include "../common/vid.h"

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	u8 sw;
	char buf[64];
	int clock;
	static const char *const freq[] = {"100", "125", "156.25",
					   "161.13", "322.26","" ,"" ,"" ,
					    "" ,"" ,"" ,"" ,"","" ,"" ,
					    "100 separate SSCG"};

	cpu_name(buf);
	printf("Board: %s,RDB ", buf);

	sw = QIXIS_READ(arch);
	printf("Board version: %c, ", (sw & 0xf) + 'A');

	sw = QIXIS_READ(brdcfg[0]);
	sw = (sw & QIXIS_LBMAP_MASK) >> QIXIS_LBMAP_SHIFT;
	switch (sw) {
	case 0:
	case 4:
		puts("boot from QSPI DEV#0\n");
		break;
	case 1:
		puts("boot from QSPI DEV#1\n");
		break;
	case 2:
	case 3:
		puts("boot from QSPI EMU\n");
		break;
	default:
		printf("invalid setting of SW%u\n", sw);
		break;
	}

	printf("CPLD: v%d.%d\n",
		(int)QIXIS_READ(scver), QIXIS_READ(tagdata));

	puts("SERDES1 Reference : ");
	sw = QIXIS_READ(brdcfg[2]);
	clock = sw >> 6;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = (sw >> 4) & 0x03;
	printf("Clock2 = %sMHz", freq[clock]);

	sw = QIXIS_READ(brdcfg[2]);
	puts("\nSERDES2 Reference : ");
	clock = (sw >> 2) & 0x03;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = sw & 0x03;
	printf("Clock2 = %sMHz", freq[clock]);

	puts("\nSERDES3 Reference : ");
	sw = QIXIS_READ(brdcfg[3]);
	clock = sw >> 6;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = (sw >> 4) & 0x03;
	printf("Clock2 = %sMHz\n", freq[clock]);

	return 0;
}

unsigned long get_board_sys_clk(void)
{
	u8 sysclk_conf = QIXIS_READ(brdcfg[1]);

	switch (sysclk_conf & 0x03) {
	case QIXIS_SYSCLK_100:
		return 100000000;
	default:
		printf("%s Value reserved\n", __func__);
		return 0;
	}
}

unsigned long get_board_ddr_clk(void)
{
	u8 ddrclk_conf = QIXIS_READ(brdcfg[1]);

	switch ((ddrclk_conf & 0x30) >> 4) {
	case QIXIS_DDRCLK_100:
		return 100000000;
	default:
		printf("%s Value reserved\n", __func__);
		return 0;
	}
}

int select_i2c_ch_pca9547(u8 ch)
{
	int ret;

	ret = i2c_write(I2C_MUX_PCA_ADDR_PRI, 0, 1, &ch, 1);
	if (ret) {
		puts("PCA: failed to select proper channel\n");
		return ret;
	}

	return 0;
}

int board_init(void)
{
#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif
	select_i2c_ch_pca9547(I2C_MUX_CH_DEFAULT);

	QIXIS_WRITE(rst_ctl, QIXIS_RST_CTL_RESET_EN);

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif
#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif

	return 0;
}

int board_early_init_f(void)
{
#ifdef CONFIG_SYS_I2C_EARLY_INIT
	i2c_early_init_f();
#endif
	fsl_lsch3_early_init_f();
	return 0;
}

int misc_init_r(void)
{
	return 0;
}

int i2c_multiplexer_select_vid_channel(u8 channel)
{
       return select_i2c_ch_pca9547(channel);
}

#if defined(CONFIG_VID)
int init_func_vid(void)
{
       if (adjust_vdd(0) < 0)
             printf("vid: core voltage not adjusted\n");

        return 0;
}
#endif

void detail_board_ddr_info(void)
{
	puts("\nDDR    ");
	print_size(gd->bd->bi_dram[0].size + gd->bd->bi_dram[1].size, "");
	print_ddr_info(0);
#ifdef CONFIG_SYS_FSL_HAS_DP_DDR
	if (soc_has_dp_ddr() && gd->bd->bi_dram[2].size) {
		puts("\nDP-DDR ");
		print_size(gd->bd->bi_dram[2].size, "");
		print_ddr_info(CONFIG_DP_DDR_CTRL);
	}
#endif
}

#if defined(CONFIG_ARCH_MISC_INIT)
int arch_misc_init(void)
{
	return 0;
}
#endif

#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
void fdt_fixup_board_enet(void *fdt)
{
	int offset;

	offset = fdt_path_offset(fdt, "/soc/fsl-mc");

	if (offset < 0)
		offset = fdt_path_offset(fdt, "/fsl-mc");

	if (offset < 0) {
		printf("%s: ERROR: fsl-mc node not found in device tree (error %d)\n",
		       __func__, offset);
		return;
	}

	if (get_mc_boot_status() == 0)
		fdt_status_okay(fdt, offset);
	else
		fdt_status_fail(fdt, offset);
}

void board_quiesce_devices(void)
{
	fsl_mc_ldpaa_exit(gd->bd);
}
#endif

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	ft_cpu_setup(blob, bd);

	/* fixup DT for the two GPP DDR banks */
	base[0] = gd->bd->bi_dram[0].start;
	size[0] = gd->bd->bi_dram[0].size;
	base[1] = gd->bd->bi_dram[1].start;
	size[1] = gd->bd->bi_dram[1].size;

#ifdef CONFIG_RESV_RAM
	/* reduce size if reserved memory is within this bank */
	if (gd->arch.resv_ram >= base[0] &&
	    gd->arch.resv_ram < base[0] + size[0])
		size[0] = gd->arch.resv_ram - base[0];
	else if (gd->arch.resv_ram >= base[1] &&
		 gd->arch.resv_ram < base[1] + size[1])
		size[1] = gd->arch.resv_ram - base[1];
#endif

	fdt_fixup_memory_banks(blob, base, size, 2);

	fsl_fdt_fixup_dr_usb(blob, bd);

	fdt_fsl_mc_fixup_iommu_map_entry(blob);

#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
	fdt_fixup_board_enet(blob);
#endif

	return 0;
}
#endif

void qixis_dump_switch(void)
{
	int i, nr_of_cfgsw;

	QIXIS_WRITE(cms[0], 0x00);
	nr_of_cfgsw = QIXIS_READ(cms[1]);

	puts("DIP switch settings dump:\n");
	for (i = 1; i <= nr_of_cfgsw; i++) {
		QIXIS_WRITE(cms[0], i);
		printf("SW%d = (0x%02x)\n", i, QIXIS_READ(cms[1]));
	}
}
