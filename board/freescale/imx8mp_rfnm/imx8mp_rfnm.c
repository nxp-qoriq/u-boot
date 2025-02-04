// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2019, 2024-2025 NXP
 */

#include <common.h>
#include <efi_loader.h>
#include <env.h>
#include <errno.h>
#include <init.h>
#include <miiphy.h>
#include <netdev.h>
#include <linux/delay.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <asm/arch/imx8mp_pins.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <spl.h>
#include <asm/mach-imx/dma.h>
#include <power/pmic.h>
#include "../common/tcpc.h"
#include <usb.h>
#include <dwc3-uboot.h>
#include <imx_sip.h>
#include <linux/arm-smccc.h>
#include <mmc.h>
#include "rfnm-shared.h"
#include <command.h>
#include "rfnm_wsled.h"

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_PUE | PAD_CTL_PE)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

static iomux_v3_cfg_t const uart_pads[] = {
	MX8MP_PAD_SD1_DATA2__UART2_DCE_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX8MP_PAD_SD1_DATA3__UART2_DCE_RX | MUX_PAD_CTRL(UART_PAD_CTRL)
};


static iomux_v3_cfg_t const wdog_pads[] = {
	MX8MP_PAD_GPIO1_IO02__WDOG1_WDOG_B  | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

#ifdef CONFIG_NAND_MXS

static void setup_gpmi_nand(void)
{
	init_nand_clk();
}
#endif

#if CONFIG_IS_ENABLED(EFI_HAVE_CAPSULE_SUPPORT)
struct efi_fw_image fw_images[] = {
	{
		.image_type_id = IMX_BOOT_IMAGE_GUID,
		.fw_name = u"IMX8MP-EVK-RAW",
		.image_index = 1,
	},
};

struct efi_capsule_update_info update_info = {
	.dfu_string = "mmc 2=flash-bin raw 0 0x2000 mmcpart 1",
	.images = fw_images,
};

u8 num_image_type_guids = ARRAY_SIZE(fw_images);
#endif /* EFI_HAVE_CAPSULE_SUPPORT */

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

	set_wdog_reset(wdog);

	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));

	init_uart_clk(1);

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
#ifdef CONFIG_IMX8M_DRAM_INLINE_ECC
#ifdef CONFIG_TARGET_IMX8MP_DDR4_EVK
	int rc;
	phys_addr_t ecc_start = 0x120000000;
	size_t ecc_size = 0x20000000;

	rc = add_res_mem_dt_node(blob, "ecc", ecc_start, ecc_size);
	if (rc < 0) {
		printf("Could not create ecc reserved-memory node.\n");
		return rc;
	}
#else
	int rc;
	phys_addr_t ecc0_start = 0xb0000000;
	phys_addr_t ecc1_start = 0x130000000;
	phys_addr_t ecc2_start = 0x1b0000000;
	size_t ecc_size = 0x10000000;

	rc = add_res_mem_dt_node(blob, "ecc", ecc0_start, ecc_size);
	if (rc < 0) {
		printf("Could not create ecc0 reserved-memory node.\n");
		return rc;
	}

	rc = add_res_mem_dt_node(blob, "ecc", ecc1_start, ecc_size);
	if (rc < 0) {
		printf("Could not create ecc1 reserved-memory node.\n");
		return rc;
	}

	rc = add_res_mem_dt_node(blob, "ecc", ecc2_start, ecc_size);
	if (rc < 0) {
		printf("Could not create ecc2 reserved-memory node.\n");
		return rc;
	}
#endif
#endif

	return 0;
}
#endif

#ifdef CONFIG_USB_TCPC
struct tcpc_port port1;
struct tcpc_port port2;

static int setup_pd_switch(uint8_t i2c_bus, uint8_t addr)
{
	struct udevice *bus;
	struct udevice *i2c_dev = NULL;
	int ret;
	uint8_t valb;

	ret = uclass_get_device_by_seq(UCLASS_I2C, i2c_bus, &bus);
	if (ret) {
		printf("%s: Can't find bus\n", __func__);
		return -EINVAL;
	}

	ret = dm_i2c_probe(bus, addr, 0, &i2c_dev);
	if (ret) {
		printf("%s: Can't find device id=0x%x\n",
			__func__, addr);
		return -ENODEV;
	}

	ret = dm_i2c_read(i2c_dev, 0xB, &valb, 1);
	if (ret) {
		printf("%s dm_i2c_read failed, err %d\n", __func__, ret);
		return -EIO;
	}
	valb |= 0x4; /* Set DB_EXIT to exit dead battery mode */
	ret = dm_i2c_write(i2c_dev, 0xB, (const uint8_t *)&valb, 1);
	if (ret) {
		printf("%s dm_i2c_write failed, err %d\n", __func__, ret);
		return -EIO;
	}

	/* Set OVP threshold to 23V */
	valb = 0x6;
	ret = dm_i2c_write(i2c_dev, 0x8, (const uint8_t *)&valb, 1);
	if (ret) {
		printf("%s dm_i2c_write failed, err %d\n", __func__, ret);
		return -EIO;
	}

	return 0;
}

int pd_switch_snk_enable(struct tcpc_port *port)
{
	if (port == &port1) {
		debug("Setup pd switch on port 1\n");
		return setup_pd_switch(1, 0x72);
	} else
		return -EINVAL;
}

/* Port2 is the power supply, port 1 does not support power */
struct tcpc_port_config port1_config = {
	.i2c_bus = 1, /*i2c2*/
	.addr = 0x50,
	.port_type = TYPEC_PORT_UFP,
	.max_snk_mv = 20000,
	.max_snk_ma = 3000,
	.max_snk_mw = 45000,
	.op_snk_mv = 15000,
	.switch_setup_func = &pd_switch_snk_enable,
	.disable_pd = true,
};

struct tcpc_port_config port2_config = {
	.i2c_bus = 2, /*i2c3*/
	.addr = 0x50,
	.port_type = TYPEC_PORT_UFP,
	.max_snk_mv = 20000,
	.max_snk_ma = 3000,
	.max_snk_mw = 45000,
	.op_snk_mv = 15000,
};

#define USB_TYPEC_SEL IMX_GPIO_NR(4, 20)
#define USB_TYPEC_EN IMX_GPIO_NR(2, 20)

static iomux_v3_cfg_t ss_mux_gpio[] = {
	MX8MP_PAD_SAI1_MCLK__GPIO4_IO20 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_SD2_WP__GPIO2_IO20 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

void ss_mux_select(enum typec_cc_polarity pol)
{
	if (pol == TYPEC_POLARITY_CC1)
		gpio_direction_output(USB_TYPEC_SEL, 0);
	else
		gpio_direction_output(USB_TYPEC_SEL, 1);
}

static int setup_typec(void)
{
	int ret;
	struct gpio_desc per_12v_desc;

	debug("tcpc_init port 2\n");
	ret = tcpc_init(&port2, port2_config, NULL);
	if (ret) {
		printf("%s: tcpc port2 init failed, err=%d\n",
		       __func__, ret);
	} else if (tcpc_pd_sink_check_charging(&port2)) {
		printf("Power supply on USB2\n");

		/* Enable PER 12V, any check before it? */
		ret = dm_gpio_lookup_name("gpio@20_1", &per_12v_desc);
		if (ret) {
			printf("%s lookup gpio@20_1 failed ret = %d\n", __func__, ret);
			return -ENODEV;
		}

		ret = dm_gpio_request(&per_12v_desc, "per_12v_en");
		if (ret) {
			printf("%s request per_12v failed ret = %d\n", __func__, ret);
			return -EIO;
		}

		/* Enable PER 12V regulator */
		dm_gpio_set_dir_flags(&per_12v_desc, GPIOD_IS_OUT | GPIOD_IS_OUT_ACTIVE);
	}

	debug("tcpc_init port 1\n");
	imx_iomux_v3_setup_multiple_pads(ss_mux_gpio, ARRAY_SIZE(ss_mux_gpio));
	gpio_request(USB_TYPEC_SEL, "typec_sel");
	gpio_request(USB_TYPEC_EN, "typec_en");
	gpio_direction_output(USB_TYPEC_EN, 0);

	ret = tcpc_init(&port1, port1_config, &ss_mux_select);
	if (ret) {
		printf("%s: tcpc port1 init failed, err=%d\n",
		       __func__, ret);
	} else {
		return ret;
	}

	return ret;
}
#endif

#ifdef CONFIG_USB_DWC3

#define USB_PHY_CTRL0			0xF0040
#define USB_PHY_CTRL0_REF_SSP_EN	BIT(2)

#define USB_PHY_CTRL1			0xF0044
#define USB_PHY_CTRL1_RESET		BIT(0)
#define USB_PHY_CTRL1_COMMONONN		BIT(1)
#define USB_PHY_CTRL1_ATERESET		BIT(3)
#define USB_PHY_CTRL1_VDATSRCENB0	BIT(19)
#define USB_PHY_CTRL1_VDATDETENB0	BIT(20)

#define USB_PHY_CTRL2			0xF0048
#define USB_PHY_CTRL2_TXENABLEN0	BIT(8)

#define USB_PHY_CTRL6			0xF0058

#define HSIO_GPR_BASE                               (0x32F10000U)
#define HSIO_GPR_REG_0                              (HSIO_GPR_BASE)
#define HSIO_GPR_REG_0_USB_CLOCK_MODULE_EN_SHIFT    (1)
#define HSIO_GPR_REG_0_USB_CLOCK_MODULE_EN          (0x1U << HSIO_GPR_REG_0_USB_CLOCK_MODULE_EN_SHIFT)


static struct dwc3_device dwc3_device_data = {
#ifdef CONFIG_SPL_BUILD
	.maximum_speed = USB_SPEED_HIGH,
#else
	.maximum_speed = USB_SPEED_SUPER,
#endif
	.base = USB1_BASE_ADDR,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.power_down_scale = 2,
};

int usb_gadget_handle_interrupts(int index)
{
	dwc3_uboot_handle_interrupt(index);
	return 0;
}

static void dwc3_nxp_usb_phy_init(struct dwc3_device *dwc3)
{
	u32 RegData;

	/* enable usb clock via hsio gpr */
	RegData = readl(HSIO_GPR_REG_0);
	RegData |= HSIO_GPR_REG_0_USB_CLOCK_MODULE_EN;
	writel(RegData, HSIO_GPR_REG_0);

	/* USB3.0 PHY signal fsel for 100M ref */
	RegData = readl(dwc3->base + USB_PHY_CTRL0);
	RegData = (RegData & 0xfffff81f) | (0x2a<<5);
	writel(RegData, dwc3->base + USB_PHY_CTRL0);

	RegData = readl(dwc3->base + USB_PHY_CTRL6);
	RegData &=~0x1;
	writel(RegData, dwc3->base + USB_PHY_CTRL6);

	RegData = readl(dwc3->base + USB_PHY_CTRL1);
	RegData &= ~(USB_PHY_CTRL1_VDATSRCENB0 | USB_PHY_CTRL1_VDATDETENB0 |
			USB_PHY_CTRL1_COMMONONN);
	RegData |= USB_PHY_CTRL1_RESET | USB_PHY_CTRL1_ATERESET;
	writel(RegData, dwc3->base + USB_PHY_CTRL1);

	RegData = readl(dwc3->base + USB_PHY_CTRL0);
	RegData |= USB_PHY_CTRL0_REF_SSP_EN;
	writel(RegData, dwc3->base + USB_PHY_CTRL0);

	RegData = readl(dwc3->base + USB_PHY_CTRL2);
	RegData |= USB_PHY_CTRL2_TXENABLEN0;
	writel(RegData, dwc3->base + USB_PHY_CTRL2);

	RegData = readl(dwc3->base + USB_PHY_CTRL1);
	RegData &= ~(USB_PHY_CTRL1_RESET | USB_PHY_CTRL1_ATERESET);
	writel(RegData, dwc3->base + USB_PHY_CTRL1);
}
#endif

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_IMX8M)
#define USB2_PWR_EN IMX_GPIO_NR(1, 14)
int board_usb_init(int index, enum usb_init_type init)
{
	int ret = 0;
	imx8m_usb_power(index, true);

	if (index == 0 && init == USB_INIT_DEVICE) {
#ifdef CONFIG_USB_TCPC
		ret = tcpc_setup_ufp_mode(&port1);
		if (ret)
			return ret;
#endif
		dwc3_nxp_usb_phy_init(&dwc3_device_data);
		return dwc3_uboot_init(&dwc3_device_data);
	} else if (index == 0 && init == USB_INIT_HOST) {
#ifdef CONFIG_USB_TCPC
		ret = tcpc_setup_dfp_mode(&port1);
#endif
		return ret;
	}

	return 0;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	int ret = 0;
	if (index == 0 && init == USB_INIT_DEVICE) {
		dwc3_uboot_exit(index);
	} else if (index == 0 && init == USB_INIT_HOST) {
#ifdef CONFIG_USB_TCPC
		ret = tcpc_disable_src_vbus(&port1);
#endif
	}

	imx8m_usb_power(index, false);

	return ret;
}

#ifdef CONFIG_USB_TCPC
/* Not used so far */
int board_typec_get_mode(int index)
{
	int ret = 0;
	enum typec_cc_polarity pol;
	enum typec_cc_state state;

	if (index == 0) {
		tcpc_setup_ufp_mode(&port1);

		ret = tcpc_get_cc_status(&port1, &pol, &state);
		if (!ret) {
			if (state == TYPEC_STATE_SRC_RD_RA || state == TYPEC_STATE_SRC_RD)
				return USB_INIT_HOST;
		}

		return USB_INIT_DEVICE;
	} else {
		return USB_INIT_HOST;
	}
}
#endif
#endif

static void setup_fec(void)
{
	struct iomuxc_gpr_base_regs *gpr =
		(struct iomuxc_gpr_base_regs *)IOMUXC_GPR_BASE_ADDR;

	/* Enable RGMII TX clk output */
	setbits_le32(&gpr->gpr[1], BIT(22));
}

static int setup_eqos(void)
{
	struct iomuxc_gpr_base_regs *gpr =
		(struct iomuxc_gpr_base_regs *)IOMUXC_GPR_BASE_ADDR;

	/* set INTF as RGMII, enable RGMII TXC clock */
	clrsetbits_le32(&gpr->gpr[1],
			IOMUXC_GPR_GPR1_GPR_ENET_QOS_INTF_SEL_MASK, BIT(16));
	setbits_le32(&gpr->gpr[1], BIT(19) | BIT(21));

	return set_clk_eqos(ENET_125MHZ);
}

#if CONFIG_IS_ENABLED(NET)
int board_phy_config(struct phy_device *phydev)
{
	if (phydev->drv->config)
		phydev->drv->config(phydev);
	return 0;
}
#endif

#define DISPMIX				13
#define MIPI				15

static iomux_v3_cfg_t ss_mux_rfnm[] = {
	MX8MP_PAD_NAND_READY_B__GPIO3_IO16 | MUX_PAD_CTRL(NO_PAD_CTRL),
	//MX8MP_PAD_SPDIF_EXT_CLK__GPIO5_IO05 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_SD2_WP__GPIO2_IO20 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_GPIO1_IO10__GPIO1_IO10 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_GPIO1_IO05__GPIO1_IO05 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_GPIO1_IO07__GPIO1_IO07 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_SAI1_MCLK__GPIO4_IO20 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_GPIO1_IO08__GPIO1_IO08 | MUX_PAD_CTRL(NO_PAD_CTRL),

	MX8MP_PAD_GPIO1_IO06__GPIO1_IO06 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_GPIO1_IO11__GPIO1_IO11 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX8MP_PAD_GPIO1_IO12__GPIO1_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL),

};


#define Si5510_PWR_EN IMX_GPIO_NR(3, 16)
//#define ENET_nRST IMX_GPIO_NR(5, 5)
#define PWR_EN_LA IMX_GPIO_NR(2, 20)
#define PWR_EN_215V IMX_GPIO_NR(1, 10)
#define PWR_EN_18V_RB IMX_GPIO_NR(1, 5)
#define PWR_EN_09V IMX_GPIO_NR(1, 7)
#define Si5510_RST IMX_GPIO_NR(4, 20)
#define LA_BOOTSTRAP_EN IMX_GPIO_NR(1, 8)

#define PWR_EN_33V_RBA IMX_GPIO_NR(1, 6)
#define PWR_EN_33V_RBB IMX_GPIO_NR(1, 11)
#define PWR_EN_375V IMX_GPIO_NR(1, 12)


int board_init(void)
{
        int ret, boot_cfg;
        struct fdt_resource res;

	imx_iomux_v3_setup_multiple_pads(ss_mux_rfnm, ARRAY_SIZE(ss_mux_rfnm));

        boot_cfg = fdt_path_offset(gd->fdt_blob, RFNM_BOOT_CONFIG_NODE_NAME);
        if (boot_cfg < 0) {
                printf("%s: Missing %s node\n", __func__,RFNM_BOOT_CONFIG_NODE_NAME);
                return -EINVAL;
        }
        else {
               debug("%s: Found %s node\n", __func__,RFNM_BOOT_CONFIG_NODE_NAME);
        }

        ret = fdt_get_resource(gd->fdt_blob, boot_cfg, "reg", 0, &res);
        if (ret != 0) {
                printf("%s: Unable to decode %s node\n", __func__,RFNM_BOOT_CONFIG_NODE_NAME);
                return -EINVAL;
        }

        printf("%s: boot config  start address  %llx\n", __func__,(phys_size_t)res.start);
        printf("%s: boot config  mem size %llx\n", __func__,
              (phys_size_t)(res.end - res.start + 1));

	// init memory for bootconfig struct
	int i;
	for(i = 0; i < 1024; i++) {
		writel(0xffffffff, (phys_size_t)res.start + (i*4));
	}
	

	

	gpio_request(Si5510_RST, "Si5510_RST");
	gpio_direction_output(Si5510_RST, 0);
	//gpio_direction_input(PWR_EN_33V);

	gpio_request(Si5510_PWR_EN, "Si5510_PWR_EN");
	gpio_direction_output(Si5510_PWR_EN, 1);

	gpio_request(PWR_EN_LA, "PWR_EN_LA");
	gpio_direction_output(PWR_EN_LA, 0);

	gpio_request(PWR_EN_215V, "PWR_EN_215V");
	gpio_direction_output(PWR_EN_215V, 1);

	gpio_request(PWR_EN_09V, "PWR_EN_09V");
	gpio_direction_output(PWR_EN_09V, 0);

	gpio_request(LA_BOOTSTRAP_EN, "LA_BOOTSTRAP_EN");
	gpio_direction_output(LA_BOOTSTRAP_EN, 1);

	gpio_request(PWR_EN_18V_RB, "PWR_EN_18V_RB");
	gpio_direction_output(PWR_EN_18V_RB, 1);

	gpio_request(PWR_EN_33V_RBA, "PWR_EN_33V_RBA");
	gpio_direction_output(PWR_EN_33V_RBA, 1);

	// breaks boot on v1 for reasons unknown
	gpio_request(PWR_EN_33V_RBB, "PWR_EN_33V_RBB");
	gpio_direction_output(PWR_EN_33V_RBB, 1);

	gpio_request(PWR_EN_375V, "PWR_EN_375V");
	gpio_direction_output(PWR_EN_375V, 1);
	



	mdelay(10);

	gpio_direction_output(Si5510_RST, 1);


	printf("Done pwr en init\n");

	//while(1);

	//gpio_request(ENET_nRST, "ENET_nRST");
	//gpio_direction_output(ENET_nRST, 1);





#ifdef CONFIG_USB_TCPC
	//setup_typec();

	/* Enable USB power default */
	//imx8m_usb_power(0, true);
	//imx8m_usb_power(1, true);
#endif

//	if (CONFIG_IS_ENABLED(FEC_MXC)) {
//		setup_fec();
//	}

	if (CONFIG_IS_ENABLED(DWC_ETH_QOS)) {
		setup_eqos();
	}

#ifdef CONFIG_NAND_MXS
	setup_gpmi_nand();
#endif

#if defined(CONFIG_USB_DWC3) || defined(CONFIG_USB_XHCI_IMX8M)
	init_usb_clk();
#endif

	/* enable the dispmix & mipi phy power domain */
	/*arm_smccc_smc(IMX_SIP_GPC, IMX_SIP_GPC_PM_DOMAIN,
		      DISPMIX, true, 0, 0, 0, 0, &res);
	arm_smccc_smc(IMX_SIP_GPC, IMX_SIP_GPC_PM_DOMAIN,
		      MIPI, true, 0, 0, 0, 0, &res);*/

	return 0;
}

int board_late_init(void)
{
#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("board_name", "EVK");
	env_set("board_rev", "iMX8MP");
#endif

	return 0;
}

#ifdef CONFIG_ANDROID_SUPPORT
bool is_power_key_pressed(void) {
	return (bool)(!!(readl(SNVS_HPSR) & (0x1 << 6)));
}
#endif

#ifdef CONFIG_SPL_MMC
#define UBOOT_RAW_SECTOR_OFFSET 0x40
unsigned long spl_mmc_get_uboot_raw_sector(struct mmc *mmc)
{
	u32 boot_dev = spl_boot_device();
	switch (boot_dev) {
		case BOOT_DEVICE_MMC2:
			return CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR - UBOOT_RAW_SECTOR_OFFSET;
		default:
			return CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR;
	}
}
#endif

#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_ANDROID_RECOVERY
int is_recovery_key_pressing(void)
{
	return 0; /* TODO */
}
#endif /* CONFIG_ANDROID_RECOVERY */
#endif /* CONFIG_FSL_FASTBOOT */


//extern void rfnm_wsled(uint32_t reg, uint32_t led1, uint32_t led2, uint32_t led3);

static int rfnm_wsled_uboot_cmd(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[]) {
	
	uint32_t which;
	uint8_t r, g, b;

	uint32_t addr = 0x100;


	if (argc != 5)
		return CMD_RET_USAGE;

	which = simple_strtoul(argv[1], NULL, 16);
	r = simple_strtoul(argv[2], NULL, 16);
	g = simple_strtoul(argv[3], NULL, 16);
	b = simple_strtoul(argv[4], NULL, 16);


	if(which) {
		addr = 0x80;
	}

	uint32_t c = (g << 16) | (r << 8) | (b << 0);

	rfnm_wsled(addr, c, 0x000000, 0x000000);

	return 0;
}
U_BOOT_CMD(rfnm_set_led, 5, 0, rfnm_wsled_uboot_cmd,
	"Setup RFNM LEDs",
	"[led_id] [r] [g] [b]\n"
	"\t- Setup LEDs, \n"
	"\t  0-255 input"
);


static int rfnm_uboot_is_rescue_mode(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[]) {
	
	if (argc != 1)
		return CMD_RET_USAGE;
	
	volatile unsigned int *addr;
	addr = 0x30390070;
	uint32_t SRC_SBMR2 = *addr;

	if(SRC_SBMR2 & (0xf << 24)) {
		return 1;
	}

	return 0;
}
U_BOOT_CMD(rfnm_is_rescue_mode, 1, 0, rfnm_uboot_is_rescue_mode,
	"Check if boot pins are set to rescue",
	"[led_id] [r] [g] [b]\n"
	"\t- Check if boot pins are set to rescue \n"
);
