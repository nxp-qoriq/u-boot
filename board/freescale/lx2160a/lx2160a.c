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

#if defined (CONFIG_TARGET_LX2160AQDS)
#define CFG_MUX_I2C_SDHC(reg, value)	((reg & 0x3f) | value)
#define SET_CFG_MUX1_SDHC1_SDHC(reg) (reg & 0x3f)
#define SET_CFG_MUX2_SDHC1_SPI(reg, value) ((reg & 0xcf) | value)
#define SET_CFG_MUX3_SDHC1_SPI(reg, value) ((reg & 0xf8) | value)
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
	int clock;
	static const char *const freq[] = {"100", "125", "156.25",
					   "161.13", "322.26","" ,"" ,"" ,
					    "" ,"" ,"" ,"" ,"","" ,"" ,
					    "100 separate SSCG"};
#endif

	cpu_name(buf);
#if defined (CONFIG_TARGET_LX2160AQDS)
	printf("Board: %s-QDS, ", buf);
#else
	printf("Board: %s-RDB, ", buf);
#endif

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

#if defined (CONFIG_TARGET_LX2160AQDS) && defined (CONFIG_FSL_QIXIS)
	printf("FPGA: v%d (%s), build %d",
	       (int)QIXIS_READ(scver), qixis_read_tag(buf),
	       (int)qixis_read_minor());
	/* the timestamp string contains "\n" at the end */
	printf(" on %s", qixis_read_time(buf));

	puts("SERDES1 Reference : ");
	sw = QIXIS_READ(brdcfg[2]);
	clock = sw >> 4;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = sw & 0x0f;
	printf("Clock2 = %sMHz", freq[clock]);

	sw = QIXIS_READ(brdcfg[3]);
	puts("\nSERDES2 Reference : ");
	clock = sw >> 4;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = sw & 0x0f;
	printf("Clock2 = %sMHz", freq[clock]);

	sw = QIXIS_READ(brdcfg[12]);
	puts("\nSERDES3 Reference : ");
	clock = sw >> 4;
	printf("Clock1 = %sMHz Clock2 = %sMHz\n", freq[clock], freq[clock]);
#else
	puts("SERDES1 Reference: Clock1 = 161.13MHz Clock2 = 161.13MHz\n");
	puts("SERDES2 Reference: Clock1 = 100MHz Clock2 = 100MHz\n");
	puts("SERDES3 Reference: Clock1 = 100MHz Clock2 = 100Hz\n");
#endif
	return 0;
}

#if defined (CONFIG_TARGET_LX2160AQDS)
/*
 * implementation of CONFIG_ESDHC_DETECT_QUIRK Macro.
 */
u8 qixis_esdhc_detect_quirk(void)
{
	/*for LX2160AQDS res1[1] @ offset 0x1A is SDHC1 Control/Status (SDHC1)
	* SDHC1 Card ID:
	* Specifies the type of card installed in the SDHC1 adapter slot.
	* 000= (reserved)
	* 001= eMMC V4.5 adapter is installed.
	* 010= SD/MMC 3.3V adapter is installed.
	* 011= eMMC V4.4 adapter is installed.
	* 100= eMMC V5.0 adapter is installed.
	* 101= MMC card/Legacy (3.3V) adapter is installed.
	* 110= SDCard V2/V3 adapter installed.
	* 111= no adapter is installed.
	*/
	return ((QIXIS_READ(res1[1]) & QIXIS_SDID_MASK) != QIXIS_ESDHC_NO_ADAPTER);
}

int config_board_mux(void)
{
	u8 reg11, reg5;

	/* - Routes {I2C2_SCL, I2C2_SDA} to SDHC1 as {SDHC1_CD_B, SDHC1_WP}.
	 * - Routes {I2C3_SCL, I2C3_SDA} to CAN transceiver as {CAN1_TX,CAN1_RX}.
	 * - Routes {I2C4_SCL, I2C4_SDA} to CAN transceiver as {CAN2_TX,CAN2_RX}.
	 * - Qixis and remote systems are isolated from the I2C1 bus.
	 *   Processor connections are still available.
	 * - SPI2 CS2_B controls EN25S64 SPI memory device.
	 * - SPI3 CS2_B controls EN25S64 SPI memory device.
	 * - EC2 connects to PHY #2 using RGMII protocol.
	 * - CLK_OUT connects to FPGA for clock measurement.
	 */
	reg5 = QIXIS_READ(brdcfg[5]);
	reg5 = CFG_MUX_I2C_SDHC(reg5, 0x40);
	QIXIS_WRITE(brdcfg[5], reg5);

	/* - Routes {SDHC1_CMD, SDHC1_CLK } to SDHC1 adapter slot.
	 *          {SDHC1_DAT3, SDHC1_DAT2} to SDHC1 adapter slot.
	 *          {SDHC1_DAT1, SDHC1_DAT0} to SDHC1 adapter slot.
	 */
	reg11 = QIXIS_READ(brdcfg[11]);
	reg11 = SET_CFG_MUX1_SDHC1_SDHC(reg11);
	QIXIS_WRITE(brdcfg[11], reg11);

	/* - Routes {SDHC1_DAT4} to SPI3 devices as {SPI3_M_CS0_B}. */
	reg11 = QIXIS_READ(brdcfg[11]);
	reg11 = SET_CFG_MUX2_SDHC1_SPI(reg11, 0x10);
	QIXIS_WRITE(brdcfg[11], reg11);

	/* - Routes {SDHC1_DAT5, SDHC1_DAT6} nowhere.
	 * {SDHC1_DAT7, SDHC1_DS } to SPI3 devices as {nothing, SPI3_M0_CLK }.
	 * {I2C5_SCL, I2C5_SDA } to SPI3 devices as {SPI3_M0_MOSI, SPI3_M0_MISO}.
	 */
	reg11 = QIXIS_READ(brdcfg[11]);
	reg11 = SET_CFG_MUX3_SDHC1_SPI(reg11, 0x01);
	QIXIS_WRITE(brdcfg[11], reg11);

	return 0;
}
#endif

unsigned long get_board_sys_clk(void)
{
#if defined (CONFIG_TARGET_LX2160AQDS) && !defined(CONFIG_ARCH_LX2160A_SIMU)
	u8 sysclk_conf = QIXIS_READ(brdcfg[1]);

	switch (sysclk_conf & 0x03) {
	case QIXIS_SYSCLK_100:
		return 100000000;
	case QIXIS_SYSCLK_125:
		return 125000000;
	case QIXIS_SYSCLK_133:
		return 133333333;
	}
	return 100000000;
#else
	return 100000000;
#endif
}

unsigned long get_board_ddr_clk(void)
{
#if defined (CONFIG_TARGET_LX2160AQDS) && !defined(CONFIG_ARCH_LX2160A_SIMU)
	u8 ddrclk_conf = QIXIS_READ(brdcfg[1]);

	switch ((ddrclk_conf & 0x30) >> 4) {
	case QIXIS_DDRCLK_100:
		return 100000000;
	case QIXIS_DDRCLK_125:
		return 125000000;
	case QIXIS_DDRCLK_133:
		return 133333333;
	}
	return 100000000;
#else
#if defined CONFIG_ARCH_LX2160A_SIMU
	return 133333333;
#else
	return 100000000;
#endif
#endif
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
#if defined (CONFIG_TARGET_LX2160AQDS)
	config_board_mux();
#endif

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
