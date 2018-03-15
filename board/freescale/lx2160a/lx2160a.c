/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
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
#include <asm/arch/mmu.h>
#include <asm/arch/soc.h>
#include <asm/arch/ppa.h>
#include <fsl_sec.h>
#include <asm/arch/fsl_serdes.h>
#include "../common/vid.h"
#include <fsl_immap.h>
#ifdef CONFIG_FSL_QIXIS
#include "../common/qixis.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
#ifdef CONFIG_SYS_I2C_EARLY_INIT
	i2c_early_init_f();
#endif
	fsl_lsch3_early_init_f();
	return 0;
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

#if defined(CONFIG_VID)
int init_func_vid(void)
{
	if (adjust_vdd(0) < 0)
		printf("core voltage not adjusted\n");

	return 0;
}

int i2c_multiplexer_select_vid_channel(u8 channel)
{
	return select_i2c_ch_pca9547(channel);
}

#endif

int checkboard(void)
{
	char buf[64];
#ifdef CONFIG_FSL_QIXIS
	u8 sw;
#endif

	cpu_name(buf);
	printf("Board: %s-RDB, ", buf);

#ifdef CONFIG_FSL_QIXIS
	sw = QIXIS_READ(arch);
	printf("Board version: %c, boot from ", (sw & 0xf) + 'A');

	sw = QIXIS_READ(brdcfg[0]);
	sw = (sw >> QIXIS_XMAP_SHIFT) & QIXIS_XMAP_MASK;
	switch (sw) {
	case 0:
	case 4:
		puts("QSPI DEV#0\n");
		break;
	case 1:
		puts("QSPI DEV#1\n");
		break;
	case 2:
	case 3:
		puts("QSPI EMU\n");
		break;
	default:
		printf("invalid setting of boot location (xmap): %d\n", sw);
		break;
	}
	printf("FPGA: v%d.%d\n", QIXIS_READ(scver), QIXIS_READ(tagdata));
#endif
	puts("SERDES1 Reference: Clock1 = 161.13MHz Clock2 = 161.13MHz\n");
	puts("SERDES2 Reference: Clock1 = 100MHz Clock2 = 100MHz\n");
	puts("SERDES3 Reference: Clock1 = 100MHz Clock2 = 100Hz\n");
	return 0;
}

unsigned long get_board_sys_clk(void)
{
	return 100000000;
}

unsigned long get_board_ddr_clk(void)
{
	/*
	 *The value returned here depends on
	 *clock of reference platform.
	 *As per LX2160A RDB schematics, this is 100MHz.
	 *For QDS, it dependes on FPGA.
	 *For Simulator, 133MHz is tested to be working fine.
	 *TODO: Add code changes for RDB, QDS.
	 */
	return 133333333;
}

int board_init(void)
{
#ifndef CONFIG_ARCH_LX2160A_EMU_COMMON
#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif
#ifdef CONFIG_TARGET_LX2RDB
	select_i2c_ch_pca9547(I2C_MUX_CH_DEFAULT);
#endif

#ifdef CONFIG_FSL_QIXIS
	QIXIS_WRITE(rst_ctl, QIXIS_RST_CTL_RESET_EN);
#endif
#endif

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif

	return 0;
}

#ifndef CONFIG_ARCH_LX2160A_PXP
void detail_board_ddr_info(void)
{
	int i;
	u64 ddr_size = 0;

	puts("\nDDR    ");
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		ddr_size += gd->bd->bi_dram[i].size;
	print_size(ddr_size, "");
	print_ddr_info(0);
}
#endif

#if defined(CONFIG_ARCH_MISC_INIT)
int arch_misc_init(void)
{
	return 0;
}
#endif

#ifdef CONFIG_FSL_MC_ENET
void fdt_fixup_board_enet(void *fdt)
{
	int offset;

	offset = fdt_path_offset(fdt, "/soc/fsl-mc");

	if (offset < 0)
		offset = fdt_path_offset(fdt, "/fsl-mc");

	if (offset < 0) {
		printf("fsl-mc node not found in device tree\n");
		return;
	}

	if ((get_mc_boot_status() == 0) && (get_dpl_apply_status() == 0))
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
	int i;
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];
	ft_cpu_setup(blob, bd);

	/* fixup DT for the three GPP DDR banks */
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		base[i] = gd->bd->bi_dram[i].start;
		size[i] = gd->bd->bi_dram[i].size;
	}

#ifdef CONFIG_RESV_RAM
	/* reduce size if reserved memory is within this bank */
	if (gd->arch.resv_ram >= base[0] &&
	    gd->arch.resv_ram < base[0] + size[0])
		size[0] = gd->arch.resv_ram - base[0];
	else if (gd->arch.resv_ram >= base[1] &&
		 gd->arch.resv_ram < base[1] + size[1])
		size[1] = gd->arch.resv_ram - base[1];
	else if (gd->arch.resv_ram >= base[2] &&
		 gd->arch.resv_ram < base[2] + size[2])
		size[2] = gd->arch.resv_ram - base[2];
#endif

	fdt_fixup_memory_banks(blob, base, size, CONFIG_NR_DRAM_BANKS);

#ifdef CONFIG_USB
	fsl_fdt_fixup_dr_usb(blob, bd);
#endif

#ifdef CONFIG_FSL_MC_ENET
	fdt_fsl_mc_fixup_iommu_map_entry(blob);
	fdt_fixup_board_enet(blob);
#endif

	return 0;
}
#endif

void qixis_dump_switch(void)
{
#ifdef CONFIG_FSL_QIXIS
	int i, nr_of_cfgsw;

	QIXIS_WRITE(cms[0], 0x00);
	nr_of_cfgsw = QIXIS_READ(cms[1]);

	puts("DIP switch settings dump:\n");
	for (i = 1; i <= nr_of_cfgsw; i++) {
		QIXIS_WRITE(cms[0], i);
		printf("SW%d = (0x%02x)\n", i, QIXIS_READ(cms[1]));
	}
#endif
}
