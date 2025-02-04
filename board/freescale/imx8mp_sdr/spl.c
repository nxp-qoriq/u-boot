// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2018-2019, 2021, 2024 NXP
 *
 */

#include <common.h>
#include <hang.h>
#include <init.h>
#include <log.h>
#include <spl.h>
#include <asm/global_data.h>
#include <asm/arch/imx8mp_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/boot_mode.h>
#include <power/pmic.h>

#include <power/pca9450.h>
#include <asm/arch/clock.h>
#include <dm/uclass.h>
#include <dm/device.h>
#include <dm/uclass-internal.h>
#include <dm/device-internal.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <fsl_esdhc_imx.h>
#include <mmc.h>
#include <asm/arch/ddr.h>
#include "rfnm_wsled.h"

DECLARE_GLOBAL_DATA_PTR;

int spl_board_boot_device(enum boot_device boot_dev_spl)
{
#ifdef CONFIG_SPL_BOOTROM_SUPPORT
	return BOOT_DEVICE_BOOTROM;
#else
	switch (boot_dev_spl) {
	case SD1_BOOT:
	case MMC1_BOOT:
	case SD2_BOOT:
	case MMC2_BOOT:
		return BOOT_DEVICE_MMC1;
	case SD3_BOOT:
	case MMC3_BOOT:
		return BOOT_DEVICE_MMC2;
	case QSPI_BOOT:
		return BOOT_DEVICE_NOR;
	case NAND_BOOT:
		return BOOT_DEVICE_NAND;
	case USB_BOOT:
		return BOOT_DEVICE_BOARD;
	default:
		return BOOT_DEVICE_NONE;
	}
#endif
}

void spl_dram_init(void)
{
	ddr_init(&dram_timing);
}

void spl_board_init(void)
{
	if (IS_ENABLED(CONFIG_FSL_CAAM)) {
		struct udevice *dev;
		int ret;

		ret = uclass_get_device_by_driver(UCLASS_MISC, DM_DRIVER_GET(caam_jr), &dev);
		if (ret)
			printf("Failed to initialize caam_jr: %d\n", ret);
	}
	/*
	 * Set GIC clock to 500Mhz for OD VDD_SOC. Kernel driver does
	 * not allow to change it. Should set the clock after PMIC
	 * setting done. Default is 400Mhz (system_pll1_800m with div = 2)
	 * set by ROM for ND VDD_SOC
	 */
#if defined(CONFIG_IMX8M_LPDDR4) && !defined(CONFIG_IMX8M_VDD_SOC_850MV)
	clock_enable(CCGR_GIC, 0);
	clock_set_target_val(GIC_CLK_ROOT, CLK_ROOT_ON | CLK_ROOT_SOURCE_SEL(5));
	clock_enable(CCGR_GIC, 1);

	puts("Normal Boot\n");
#endif
}

#if CONFIG_IS_ENABLED(DM_PMIC_PCA9450)
int power_init_board(void)
{
	struct udevice *dev;
	int ret;

	ret = pmic_get("pca9450@25", &dev);
	if (ret == -ENODEV) {
		puts("No pca9450@25\n");
		return 0;
	}
	if (ret != 0)
		return ret;

	/* BUCKxOUT_DVS0/1 control BUCK123 output */
	pmic_reg_write(dev, PCA9450_BUCK123_DVS, 0x29);

#ifdef CONFIG_IMX8M_LPDDR4
	/*
	 * increase VDD_SOC to typical value 0.95V before first
	 * DRAM access, set DVS1 to 0.85v for suspend.
	 * Enable DVS control through PMIC_STBY_REQ and
	 * set B1_ENMODE=1 (ON by PMIC_ON_REQ=H)
	 */
#ifdef CONFIG_IMX8M_VDD_SOC_850MV
	/* set DVS0 to 0.85v for special case*/
	pmic_reg_write(dev, PCA9450_BUCK1OUT_DVS0, 0x14);
#else
	pmic_reg_write(dev, PCA9450_BUCK1OUT_DVS0, 0x1C);
#endif
	pmic_reg_write(dev, PCA9450_BUCK1OUT_DVS1, 0x14);
	pmic_reg_write(dev, PCA9450_BUCK1CTRL, 0x59);

	/* Kernel uses OD/OD freq for SOC */
	/* To avoid timing risk from SOC to ARM,increase VDD_ARM to OD voltage 0.95v */
	pmic_reg_write(dev, PCA9450_BUCK2OUT_DVS0, 0x1C);
#elif defined(CONFIG_IMX8M_DDR4)
	/* DDR4 runs at 3200MTS, uses default ND 0.85v for VDD_SOC and VDD_ARM */
	pmic_reg_write(dev, PCA9450_BUCK1CTRL, 0x59);

	/* Set NVCC_DRAM to 1.2v for DDR4 */
	pmic_reg_write(dev, PCA9450_BUCK6OUT, 0x18);
#endif

	/* set WDOG_B_CFG to cold reset */
	pmic_reg_write(dev, PCA9450_RESET_CTRL, 0xA1);

	return 0;
}
#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	/* Just empty function now - can't decide what to choose */
	debug("%s: %s\n", __func__, name);

	return 0;
}
#endif

/*
static iomux_v3_cfg_t ss_mux_rfnm_pwr[] = {
	MX8MP_PAD_GPIO1_IO12__GPIO1_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_NAND_READY_B__GPIO3_IO16 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_NAND_DATA01__GPIO3_IO07 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_NAND_DATA02__GPIO3_IO08 | MUX_PAD_CTRL(NO_PAD_CTRL)
};

#define PWR_EN_33V IMX_GPIO_NR(1, 12)
#define Si5510_PWR_EN IMX_GPIO_NR(3, 16)
#define LED_DIN1 IMX_GPIO_NR(3, 7)
#define LED_DIN2 IMX_GPIO_NR(3, 8)
*/



void board_init_f(ulong dummy)
{
	struct udevice *dev;
	int ret;

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	arch_cpu_init();

	// LEDs and power init needs to happen early in the boot process (before pmic i2c requests)

	volatile unsigned int *addr;

	addr = 0x30220004; // GPIO3 direction register

	*addr = *addr | (1 << 7); // LED1
	*addr = *addr | (1 << 8); // LED2
	*addr = *addr | (1 << 16); // Si5510_PWR_EN

	addr = 0x30200004; // GPIO1 direction register

	*addr = *addr | (1 << 12); // PWR_EN_33V
	*addr = *addr | (1 << 10); // PWR_EN_18V



	addr = 0x30200000;
	*addr = *addr | (1 << 12);
	*addr = *addr | (1 << 10);

	addr = 0x30220000;

	*addr = *addr | (1 << 16);

	rfnm_wsled_spl(0x100, 0x00ff00, 0x000000, 0x000000);
	rfnm_wsled_spl(0x80, 0x00ff00, 0x000000, 0x000000);

	board_early_init_f();

	timer_init();

	preloader_console_init();

	ret = spl_early_init();
	if (ret) {
		debug("spl_early_init() failed: %d\n", ret);
		hang();
	}

	ret = uclass_get_device_by_name(UCLASS_CLK,
					"clock-controller@30380000",
					&dev);
	if (ret < 0) {
		printf("Failed to find clock node. Check device tree\n");
		hang();
	}

	//imx_iomux_v3_setup_multiple_pads(ss_mux_rfnm_pwr, ARRAY_SIZE(ss_mux_rfnm_pwr));

	//gpio_request(PWR_EN_33V, "pwr_en_33v");
	//gpio_direction_output(PWR_EN_33V, 1);

	//gpio_request(Si5510_PWR_EN, "si5510_pwr_en");
	//gpio_direction_output(Si5510_PWR_EN, 1);

	//gpio_request(LED_DIN1, "LED_DIN1");
	//gpio_direction_output(LED_DIN1, 0);

	//gpio_request(LED_DIN2, "LED_DIN2");
	//gpio_direction_output(LED_DIN2, 0);

	//printf("Done pwr en init\n");

	enable_tzc380();

	power_init_board();

	/* DDR initialization */
	spl_dram_init();

	board_init_r(NULL, 0);
}


















