// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020 NXP
 */

#include <common.h>
#include <i2c.h>
#include <fdt_support.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/arch/soc.h>
#include <asm/arch-fsl-layerscape/fsl_icid.h>
#include <hwconfig.h>
#include <ahci.h>
#include <mmc.h>
#include <scsi.h>
#include <fm_eth.h>
#include <fsl_csu.h>
#include <fsl_esdhc.h>
#include <fsl_sec.h>
#include <fsl_ddr_sdram.h>

DECLARE_GLOBAL_DATA_PTR;

#define BOOT_FROM_QSPI_DEV0		0x1
#define BOOT_FROM_QSPI_DEV1		0x2
#define BOOT_FROM_SD		0x3
#define BOOT_FROM_EMMC		0x4

struct pcal_info {
	u8 data[2];
	u8 offset_addr[2];
};

#ifndef CONFIG_TARGET_DB1046
int select_i2c_ch_pca9847(u8 ch, int bus_num)
{
	int ret;

	struct udevice *dev;

	ret = i2c_get_chip_for_busnum(bus_num, I2C_MUX_PCA_ADDR_PRI,
				      1, &dev);
	if (ret) {
		printf("%s: Cannot find udev for a bus %d\n", __func__,
		       bus_num);
		return ret;
	}
	ret = dm_i2c_write(dev, 0, &ch, 1);
	if (ret) {
		puts("PCA: failed to select proper channel\n");
		return ret;
	}

	return 0;
}
#endif

int board_early_init_f(void)
{
	fsl_lsch2_early_init_f();

	return 0;
}

#ifndef CONFIG_SPL_BUILD
int checkboard(void)
{
	u8 in1;
	int ret, bus_num = 0;
	enum boot_src src = get_boot_src();
	struct udevice *dev;

#ifndef CONFIG_TARGET_DB1046
	select_i2c_ch_pca9847(I2C_MUX_CH_DEFAULT, 0);
	puts("Board: LS1046AWRDB, boot from ");
#else
	puts("Board: DB1046A, boot from ");
#endif
	ret = i2c_get_chip_for_busnum(bus_num, I2C_MUX_IO_ADDR,
				      1, &dev);
	if (ret) {
		printf("%s: Cannot find udev for a bus %d\n", __func__,
		       bus_num);
		return -ENXIO;
	}
	ret = dm_i2c_read(dev, I2C_MUX_IO_1, &in1, 1);
	if (ret < 0) {
		printf("Error reading i2c boot information!\n");
		return 0; /* Don't want to hang() on this error */
	}

	if (src == BOOT_SOURCE_SD_MMC) {
		if ((in1 & SW_SD_EMMC_BOOT_MASK) == SW_BOOT_SD)
			puts(": SD\n");
		else if ((in1 & SW_SD_EMMC_BOOT_MASK) == SW_BOOT_EMMC)
			puts(": eMMC\n");
		else
			puts("unknown\n");
	} else {
		if ((in1 & SW_QSPI_BOOT_MASK) == SW_BOOT_QSPI_DEV0)
			puts(": QSPI Dev0\n");
		else if ((in1 & SW_QSPI_BOOT_MASK) == SW_BOOT_QSPI_DEV1)
			puts(": QSPI Dev1\n");
		else
			puts("unknown\n");
	}

	return 0;
}

int board_init(void)
{
#ifdef CONFIG_SECURE_BOOT
	/*
	 * In case of Secure Boot, the IBR configures the SMMU
	 * to allow only Secure transactions.
	 * SMMU must be reset in bypass mode.
	 * Set the ClientPD bit and Clear the USFCFG Bit
	 */
	u32 val;

	val = (in_le32(SMMU_SCR0) | SCR0_CLIENTPD_MASK) & ~(SCR0_USFCFG_MASK);
	out_le32(SMMU_SCR0, val);
	val = (in_le32(SMMU_NSCR0) | SCR0_CLIENTPD_MASK) & ~(SCR0_USFCFG_MASK);
	out_le32(SMMU_NSCR0, val);
#endif

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif

	return 0;
}

int board_setup_core_volt(u32 vdd)
{
	return 0;
}

void config_board_mux(void)
{
#ifdef CONFIG_HAS_FSL_XHCI_USB
	struct ccsr_scfg *scfg = (struct ccsr_scfg *)CONFIG_SYS_FSL_SCFG_ADDR;
	u32 usb_pwrfault;

	/* USB3 is not used, configure mux to IIC4_SCL/IIC4_SDA */
	out_be32(&scfg->rcwpmuxcr0, 0x3300);
	out_be32(&scfg->usbdrvvbus_selcr, SCFG_USBDRVVBUS_SELCR_USB1);
	usb_pwrfault = (SCFG_USBPWRFAULT_DEDICATED <<
			SCFG_USBPWRFAULT_USB3_SHIFT) |
			(SCFG_USBPWRFAULT_DEDICATED <<
			SCFG_USBPWRFAULT_USB2_SHIFT) |
			(SCFG_USBPWRFAULT_SHARED <<
			SCFG_USBPWRFAULT_USB1_SHIFT);
	out_be32(&scfg->usbpwrfault_selcr, usb_pwrfault);
#endif
}

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
	config_board_mux();
	return 0;
}
#endif

int ft_board_setup(void *blob, bd_t *bd)
{
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	/* fixup DT for the two DDR banks */
	base[0] = gd->bd->bi_dram[0].start;
	size[0] = gd->bd->bi_dram[0].size;
	base[1] = gd->bd->bi_dram[1].start;
	size[1] = gd->bd->bi_dram[1].size;

	fdt_fixup_memory_banks(blob, base, size, 2);
	ft_cpu_setup(blob, bd);

#ifdef CONFIG_SYS_DPAA_FMAN
	fdt_fixup_fman_ethernet(blob);
#endif

	fdt_fixup_icid(blob);

	return 0;
}

int fsl_initdram(void)
{
	gd->ram_size = tfa_get_dram_size();
	if (!gd->ram_size)
		gd->ram_size = fsl_ddr_sdram_size();
	return 0;
}

static int ls1046awrdb_soc_reset(void)
{
	udelay(50000);
	disable_interrupts();
	reset_misc();
	reset_cpu(0);

	return 0;
}

static int switch_boot_source(int src_id)
{
	struct pcal_info pcal0, pcal1;
	int ret, i, bus_num = 0;
	struct udevice *dev;

#ifndef CONFIG_TARGET_DB1046
	select_i2c_ch_pca9847(I2C_MUX_CH_DEFAULT, 0);
#endif
	pcal0.offset_addr[0] = PCAL_P1_CONF_ADDR;
	pcal0.offset_addr[1] = PCAL_P1_OUTPUT_ADDR;

	pcal0.data[0] = PCAL0_P1_0_OUTPUT;
	pcal0.data[1] = PCAL0_P1_0_SD_QSPI;

	ret = i2c_get_chip_for_busnum(bus_num, I2C_MUX_IO2_ADDR,
				      1, &dev);
	if (ret) {
		printf("%s: Cannot find udev for a bus %d\n", __func__,
		       bus_num);
		return -ENXIO;
	}

	for (i = 0; i < 2; i++) {
		ret = dm_i2c_write(dev, pcal0.offset_addr[i], &pcal0.data[i],
				   1);
		if (ret) {
			printf("i2c write error addr: %u reg: %u data: %u\n",
			       I2C_MUX_IO2_ADDR, pcal0.offset_addr[i],
			       pcal0.data[i]);
			goto err;
		}
	}

	pcal0.offset_addr[0] = PCAL_P0_CONF_ADDR;
	pcal0.offset_addr[1] = PCAL_P0_OUTPUT_ADDR;

	pcal1.offset_addr[0] = PCAL_P1_CONF_ADDR;
	pcal1.offset_addr[1] = PCAL_P1_OUTPUT_ADDR;

	switch (src_id) {
	case BOOT_FROM_QSPI_DEV0:
		pcal0.data[0] = PCAL0_P0_OUTPUT;
		pcal0.data[1] = PCAL0_P0_QSPI;
		pcal1.data[0] = PCAL1_P0_6_OUTPUT;
		pcal1.data[1] = PCAL1_P1_6_QSPI_DEV0;
		break;
	case BOOT_FROM_QSPI_DEV1:
		pcal0.data[0] = PCAL0_P0_OUTPUT;
		pcal0.data[1] = PCAL0_P0_QSPI;
		pcal1.data[0] = PCAL1_P0_6_OUTPUT;
		pcal1.data[1] = PCAL1_P1_6_QSPI_DEV1;
		break;
	case BOOT_FROM_SD:
		pcal0.data[0] = PCAL0_P0_OUTPUT;
		pcal0.data[1] = PCAL0_P0_SD_EMMC;
		pcal1.data[0] = PCAL1_P1_7_OUTPUT;
		pcal1.data[1] = PCAL1_P1_7_SD;
		break;
	case BOOT_FROM_EMMC:
		pcal0.data[0] = PCAL0_P0_OUTPUT;
		pcal0.data[1] = PCAL0_P0_SD_EMMC;
		pcal1.data[0] = PCAL1_P1_7_OUTPUT;
		pcal1.data[1] = PCAL1_P1_7_EMMC;
		break;
	}

	for (i = 0; i < 2; i++) {
		ret = dm_i2c_write(dev, pcal0.offset_addr[i],
				   &pcal0.data[i], 1);
		if (ret) {
			printf("i2c write error addr: %u reg: %u data: %u\n",
			       I2C_MUX_IO2_ADDR, pcal0.offset_addr[i],
			       pcal0.data[i]);
			goto err;
		}
	}

	ret = i2c_get_chip_for_busnum(bus_num, I2C_MUX_IO_ADDR,
				      1, &dev);
	if (ret) {
		printf("%s: Cannot find udev for a bus %d\n", __func__,
		       bus_num);
		return -ENXIO;
	}

	for (i = 0; i < 2; i++) {
		ret = dm_i2c_write(dev, pcal1.offset_addr[i],
				   &pcal1.data[i], 1);
		if (ret) {
			printf("i2c write error addr: %u reg: %u data: %u\n",
			       I2C_MUX_IO_ADDR, pcal1.offset_addr[i],
			       pcal1.data[i]);
			goto err;
		}
	}
	ls1046awrdb_soc_reset();
err:
	return ret;
}

static int switch_sd_emmc(int src_id)
{
	struct pcal_info  pcal1;
	int ret, i, bus_num = 0;
	struct udevice *dev;

#ifndef CONFIG_TARGET_DB1046
	select_i2c_ch_pca9847(I2C_MUX_CH_DEFAULT, 0);
#endif
	pcal1.offset_addr[0] = PCAL_P1_CONF_ADDR;
	pcal1.offset_addr[1] = PCAL_P1_OUTPUT_ADDR;

	pcal1.data[0] = PCAL1_P1_7_OUTPUT;

	switch (src_id) {
	case BOOT_FROM_SD:
		pcal1.data[1] = PCAL1_P1_7_SD;
		break;
	case BOOT_FROM_EMMC:
		pcal1.data[1] = PCAL1_P1_7_EMMC;
		break;
	}

	ret = i2c_get_chip_for_busnum(bus_num, I2C_MUX_IO_ADDR,
				      1, &dev);
	if (ret) {
		printf("%s: Cannot find udev for a bus %d\n", __func__,
		       bus_num);
		return -ENXIO;
	}

	for (i = 0; i < 2; i++) {
		ret = dm_i2c_write(dev, pcal1.offset_addr[i],
				   &pcal1.data[i], 1);
		if (ret) {
			printf("i2c write error addr: %u reg: %u data: %u\n",
			       I2C_MUX_IO_ADDR, pcal1.offset_addr[i],
			       pcal1.data[i]);
			goto err;
		}
	}
	printf("Run mmc rescan cmd to rescan the sd/emmc card\n");
err:
	return ret;
}

static int flash_boot_cmd(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
	if (argc <= 1)
		return CMD_RET_USAGE;
	if (strcmp(argv[1], "qspi") == 0) {
		if (strcmp(argv[2], "1") == 0)
			switch_boot_source(BOOT_FROM_QSPI_DEV1);
		else
			switch_boot_source(BOOT_FROM_QSPI_DEV0);
	} else if (strcmp(argv[1], "sd") == 0)
		switch_boot_source(BOOT_FROM_SD);
	else if (strcmp(argv[1], "emmc") == 0)
		switch_boot_source(BOOT_FROM_EMMC);
	else
		return CMD_RET_USAGE;

	return 0;
}

static int select_sd_emmc(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
	if (argc <= 1)
		return CMD_RET_USAGE;
	if (strcmp(argv[1], "sd") == 0)
		switch_sd_emmc(BOOT_FROM_SD);
	else if (strcmp(argv[1], "emmc") == 0)
		switch_sd_emmc(BOOT_FROM_EMMC);
	else
		return CMD_RET_USAGE;

	return 0;
}

U_BOOT_CMD(boot_source, 3, 0, flash_boot_cmd,
	   "Flash boot Selection Control",
	   "boot_source qspi 0 : reset to qspi primary bank\n"
	   "boot_source qspi 1 : reset to qspi alternate bank\n"
	   "boot_source sd : reset to sd\n"
	   "boot_source emmc : reset to emmc\n"
);

U_BOOT_CMD(select, 3, 0, select_sd_emmc,
	   "sd/emmc Selection Control",
	   "select sd : select sd card\n"
	   "select emmc : select emmc\n"
);

#endif
