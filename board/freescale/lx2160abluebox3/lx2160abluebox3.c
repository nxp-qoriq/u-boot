// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020 NXP
 */

#include <common.h>
#include <clock_legacy.h>
#include <dm.h>
#include <dm/platform_data/serial_pl01x.h>
#include <i2c.h>
#include <malloc.h>
#include <errno.h>
#include <netdev.h>
#include <fsl_ddr.h>
#include <fsl_sec.h>
#include <asm/io.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
#include <fsl-mc/fsl_mc.h>
#include <env_internal.h>
#include <efi_loader.h>
#include <asm/arch/mmu.h>
#include <hwconfig.h>
#include <asm/arch/clock.h>
#include <asm/arch/config.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/arch/soc.h>
#include "../common/qixis.h"
#include "../common/vid.h"
#include <fsl_immap.h>
#include <asm/arch-fsl-layerscape/fsl_icid.h>
#include <asm/gic-v3.h>
#include <cpu_func.h>

#ifdef CONFIG_EMC2305
#include "../common/emc2305.h"
#endif

#define GIC_LPI_SIZE                             0x200000

DECLARE_GLOBAL_DATA_PTR;

static struct pl01x_serial_platdata serial0 = {
#if CONFIG_CONS_INDEX == 0
	.base = CONFIG_SYS_SERIAL0,
#elif CONFIG_CONS_INDEX == 1
	.base = CONFIG_SYS_SERIAL1,
#else
#error "Unsupported console index value."
#endif
	.type = TYPE_PL011,
};

U_BOOT_DEVICE(nxp_serial0) = {
	.name = "serial_pl01x",
	.platdata = &serial0,
};

static struct pl01x_serial_platdata serial1 = {
	.base = CONFIG_SYS_SERIAL1,
	.type = TYPE_PL011,
};

U_BOOT_DEVICE(nxp_serial1) = {
	.name = "serial_pl01x",
	.platdata = &serial1,
};

static int select_i2c_ch(int busnum, int muxaddr, u8 ch)
{
	int ret;

	struct udevice *dev;

	ret = i2c_get_chip_for_busnum(busnum, muxaddr, 1, &dev);
	if (!ret)
		ret = dm_i2c_write(dev, 0, &ch, 1);

	return ret;
}

int select_i2c_ch_pca9547(u8 ch)
{
	int ret;

	ret = select_i2c_ch(0, I2C_MUX_PCA_ADDR_PRI, ch);
	if (ret) {
		puts("PCA: failed to select proper channel\n");
		return ret;
	}

	return 0;
}

int select_i2c_ch_pca9547_sec(u8 ch)
{
	int ret;

	ret = select_i2c_ch(0, I2C_MUX_PCA_ADDR_SEC, ch);
	if (ret) {
		puts("PCA: failed to select proper channel\n");
		return ret;
	}

	return 0;
}

static int select_i2c_ch_t6slot6ctrl(void)
{
	int ret;

	ret = select_i2c_ch(I2C_PCIE_BUS_NUM, I2C_PCIE_MUX_PCA9846_ADDR_PRI, I2C_PCIE_MUX_CH_PCIE_MB2);
	if (!ret)
		ret = select_i2c_ch(I2C_PCIE_BUS_NUM, I2C_T6_PCA9546_ADDR, I2C_T6_MUX_CH_SLOT6CTRL);

	return ret;
}

static void configure_t6slot6(void)
{
	char buf[HWCONFIG_BUFFER_SIZE];
	int ret;
	int equalizerschanged = 0;
	int lanes = -1;
	struct udevice *dev_pcal6408;
	struct udevice *dev_ptn3944_0;
	struct udevice *dev_ptn3944_1;
	struct udevice *dev_ptn3944_2;
	struct udevice *dev_ptn3944_3;
	u8 pins, eq_is0to3, eq_is4to7, eq_should;

	/* We assume autoconfig without hwconfig specified */
	lanes = -1;

	/*
	 * Extract hwconfig from environment since we have not properly setup
	 * the environment but need it for T6 params
	 */
	if (env_get_f("hwconfig", buf, sizeof(buf)) < 0)
		buf[0] = '\0';

	if (hwconfig_sub_f("pcie", "t6slot6", buf)) {
		if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "off", buf))
			lanes = 0;
		else if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "auto", buf))
			lanes = 256;
		else if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "x1", buf))
			lanes = 1;
		else if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "x2", buf))
			lanes = 2;
		else if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "x4", buf))
			lanes = 4;
		else if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "x8", buf))
			lanes = 8;
		else if (hwconfig_subarg_cmp_f("pcie", "t6slot6", "x16", buf))
			lanes = 16;
	}

	debug("T6slot6: %d lanes hwconfig '%s'\n", lanes, buf);

	/*
	 * Fail silently and gracefully if there is no T6 riser only
	 * if we do not request a config through the environment!
	 * We may run a system with T2 or without riser.
	 */
	if (select_i2c_ch_t6slot6ctrl()) {
		if (lanes >= 0)
			puts("T6 riser not detected, hwconfig pcie:t6slot6 setting will be ignored!\n");
		return;
	}

	/* No specific T6slot6 setting means auto config! */
	if (lanes < 0 || lanes >= 256) {
		if (i2c_get_chip_for_busnum(I2C_PCIE_BUS_NUM, I2C_T6_PCAL6408_ADDR, 1, &dev_pcal6408)) {
			puts("T6 slot6: Cannot find PCAL6408!\n");
			return;
		}

		/* Determine the number of plugged lanes by presence detect */
		ret = dm_i2c_read(dev_pcal6408, I2C_T6_PCAL6408_INPUT_PORT, (void *)&pins, 1);
		if (ret) {
			puts("T6 slot6: Cannot read PCAL6408!\n");
			return;
		}

		pins &= I2C_T6_PCAL6408_PRESENCE_DETECT_MASK;
		pins ^= I2C_T6_PCAL6408_PRESENCE_DETECT_MASK;

		lanes = (pins & 0x8) ? 16 :
			(pins & 0x4) ? 8 :
			(pins & 0x2) ? 4 :
			(pins & 0x1) ? 1 : 0;

		/*
		 * We don't waste time if no card is plugged in.
		 * This should only happen for auto config!
		 */
		if (!lanes)
			return;

		printf("T6 slot6: Detected card width of %d lanes\n", lanes);
	}

	if (i2c_get_chip_for_busnum(I2C_PCIE_BUS_NUM, I2C_T6_PTN3944_BASE_ADDR + 0, 1, &dev_ptn3944_0) ||
	    i2c_get_chip_for_busnum(I2C_PCIE_BUS_NUM, I2C_T6_PTN3944_BASE_ADDR + 1, 1, &dev_ptn3944_1) ||
	    i2c_get_chip_for_busnum(I2C_PCIE_BUS_NUM, I2C_T6_PTN3944_BASE_ADDR + 2, 1, &dev_ptn3944_2) ||
	    i2c_get_chip_for_busnum(I2C_PCIE_BUS_NUM, I2C_T6_PTN3944_BASE_ADDR + 3, 1, &dev_ptn3944_3)) {
		puts("T6 slot6: Cannot find equalizers on T6 riser I2C!\n");
		return;
	}

	/* Check and update the equalizers */
	ret = dm_i2c_read(dev_ptn3944_0, PTN3944_LINK_CTRL_STATUS, (void *)&eq_is0to3, 1);
	ret |= dm_i2c_read(dev_ptn3944_2, PTN3944_LINK_CTRL_STATUS, (void *)&eq_is4to7, 1);
	if (ret) {
		printf("T6 slot6: Cannot read equalizer settings on T6 riser I2C\n");
		return;
	}
	debug("eq_is0to3: %02x\n", eq_is0to3);
	debug("eq_is4to7: %02x\n", eq_is4to7);

	/* First check lanes 0-3, then 4-7 */
	eq_should = (lanes >= 4) ? PTN3944_LCS_4CHANNELS :
		    (lanes >= 2) ? PTN3944_LCS_2CHANNELS :
		    (lanes >= 1) ? PTN3944_LCS_1CHANNELS :
		    PTN3944_LCS_0CHANNELS;
	if ((eq_is0to3 & PTN3944_LCS_CHANNEL_MASK) != eq_should) {
		ret |= dm_i2c_write(dev_ptn3944_0, PTN3944_LINK_CTRL_STATUS, (void *)&eq_should, 1);
		ret |= dm_i2c_write(dev_ptn3944_1, PTN3944_LINK_CTRL_STATUS, (void *)&eq_should, 1);
		equalizerschanged = 1;
	}

	eq_should = (lanes >= 8) ? PTN3944_LCS_4CHANNELS :
		    PTN3944_LCS_0CHANNELS;
	if ((eq_is4to7 & PTN3944_LCS_CHANNEL_MASK) != eq_should) {
		ret |= dm_i2c_write(dev_ptn3944_2, PTN3944_LINK_CTRL_STATUS, (void *)&eq_should, 1);
		ret |= dm_i2c_write(dev_ptn3944_3, PTN3944_LINK_CTRL_STATUS, (void *)&eq_should, 1);
		equalizerschanged = 1;
	}

	if (ret) {
		printf("T6 slot6: Updating the equalizer settings failed\n");
		return;
	}

	/*
	 * Unfortunately we have to reboot after the equalizer change
	 * because the PCIe controller can get stuck in compliance
	 * pattern handling and we do not have a spec compliant way to
	 * reset just the LTSSM properly. If we can come up with a way
	 * to just reset the LTSSM, it would be much nicer.
	 */
	if (equalizerschanged) {
		enum boot_src src = get_boot_src();

		printf("T6 slot6: Reconfigured to %d lanes, rebooting ...\n", lanes);

		/* Trying hard to preserve the boot source */
		if (src == BOOT_SOURCE_SD_MMC) {
			run_command("qixis_reset sd", 0);
		} else if (src == BOOT_SOURCE_SD_MMC2) {
			run_command("qixis_reset emmc", 0);
		} else {
			u8 sw;

			sw = QIXIS_READ(brdcfg[0]);
			sw = (sw >> QIXIS_XMAP_SHIFT) & QIXIS_XMAP_MASK;
			switch (sw) {
			case 0:
				run_command("qixis_reset qspi", 0);
				break;
			case 1:
				run_command("qixis_reset altbank", 0);
				break;
			default:
				/* generic reset for any other combination */
				run_command("reset", 0);
				break;
			}
		}
		/*
		 * It takes a little while until the Qixis reset kicks
		 * in. We want to wait until it does
		 */
		while (1)
			/* do nothing */;
	}
}

static void uart_get_clock(void)
{
	serial0.clock = get_serial_clock();
	serial1.clock = get_serial_clock();
}

int board_early_init_f(void)
{
#ifdef CONFIG_SYS_I2C_EARLY_INIT
	i2c_early_init_f();
#endif
	/* get required clock for UART IP */
	uart_get_clock();

#ifdef CONFIG_EMC2305
	select_i2c_ch_pca9547(I2C_MUX_CH_EMC2305);
	emc2305_init(I2C_EMC2305_ADDR1);
	set_fan_speed(I2C_EMC2305_PWM, I2C_EMC2305_ADDR1);
	emc2305_init(I2C_EMC2305_ADDR2);
	set_fan_speed(I2C_EMC2305_PWM, I2C_EMC2305_ADDR2);
	select_i2c_ch_pca9547(I2C_MUX_CH_DEFAULT);
#endif

	fsl_lsch3_early_init_f();
	return 0;
}

#ifdef CONFIG_OF_BOARD_FIXUP
int board_fix_fdt(void *fdt)
{
	char *reg_names, *reg_name;
	int names_len, old_name_len, new_name_len, remaining_names_len;
	struct str_map {
		char *old_str;
		char *new_str;
	} reg_names_map[] = {
		{ "ccsr", "dbi" },
		{ "pf_ctrl", "ctrl" }
	};
	int off = -1, i = 0;

	if (IS_SVR_REV(get_svr(), 1, 0))
		return 0;

	off = fdt_node_offset_by_compatible(fdt, -1, "fsl,lx2160a-pcie");
	while (off != -FDT_ERR_NOTFOUND) {
		fdt_setprop(fdt, off, "compatible", "fsl,ls-pcie",
			    strlen("fsl,ls-pcie") + 1);

		reg_names = (char *)fdt_getprop(fdt, off, "reg-names",
						&names_len);
		if (!reg_names)
			continue;

		reg_name = reg_names;
		remaining_names_len = names_len - (reg_name - reg_names);
		i = 0;
		while ((i < ARRAY_SIZE(reg_names_map)) && remaining_names_len) {
			old_name_len = strlen(reg_names_map[i].old_str);
			new_name_len = strlen(reg_names_map[i].new_str);
			if (memcmp(reg_name, reg_names_map[i].old_str,
				   old_name_len) == 0) {
				/* first only leave required bytes for new_str
				 * and copy rest of the string after it
				 */
				memcpy(reg_name + new_name_len,
				       reg_name + old_name_len,
				       remaining_names_len - old_name_len);
				/* Now copy new_str */
				memcpy(reg_name, reg_names_map[i].new_str,
				       new_name_len);
				names_len -= old_name_len;
				names_len += new_name_len;
				i++;
			}

			reg_name = memchr(reg_name, '\0', remaining_names_len);
			if (!reg_name)
				break;

			reg_name += 1;

			remaining_names_len = names_len -
					      (reg_name - reg_names);
		}

		fdt_setprop(fdt, off, "reg-names", reg_names, names_len);
		off = fdt_node_offset_by_compatible(fdt, off,
						    "fsl,lx2160a-pcie");
	}

	return 0;
}
#endif

int esdhc_status_fixup(void *blob, const char *compat)
{
	/* Enable esdhc DT nodes */
	do_fixup_by_compat(blob, compat, "status", "okay",
			   sizeof("okay"), 1);

	return 0;
}

#if defined(CONFIG_VID)
int i2c_multiplexer_select_vid_channel(u8 channel)
{
	return select_i2c_ch_pca9547(channel);
}

int init_func_vid(void)
{
	int set_vid;

	set_vid = adjust_vdd(0);

	if (set_vid < 0)
		printf("core voltage not adjusted\n");

	return 0;
}
#endif

int checkboard(void)
{
	enum boot_src src = get_boot_src();
	char buf[64];
	u8 sw;
	int clock;
	static const char *const freq[] = {"100", "", "", "161.13"};
	u16 minor;

	cpu_name(buf);

	printf("Board: %s-BLUEBOX3, ", buf);

	sw = QIXIS_READ(arch);
	printf("Board version: %c, boot from ", (sw & 0xf) - 1 + 'A');

	if (src == BOOT_SOURCE_SD_MMC) {
		puts("SD\n");
	} else if (src == BOOT_SOURCE_SD_MMC2) {
		puts("eMMC\n");
	} else {
		sw = QIXIS_READ(brdcfg[0]);
		sw = (sw >> QIXIS_XMAP_SHIFT) & QIXIS_XMAP_MASK;
		switch (sw) {
		case 0:
			puts("FlexSPI DEV#0\n");
			break;
		case 1:
			puts("FlexSPI DEV#1\n");
			break;
		default:
			printf("invalid setting, xmap: %d\n", sw);
			break;
		}
	}

	printf("FPGA: v%d", (int)QIXIS_READ(scver));

	minor = qixis_read_minor();
	if (minor)
		printf(".%d", minor);

	/* the timestamp string contains "\n" at the end */
	printf(" built on %s", qixis_read_time(buf));

	puts("SERDES1 Reference : ");
	sw = QIXIS_READ(brdcfg[2]);
	clock = sw >> 6 & SERDES_CLOCK_MASK;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = sw >> 4 & SERDES_CLOCK_MASK;
	printf("Clock2 = %sMHz", freq[clock]);

	puts("\nSERDES2 Reference : ");
	sw = QIXIS_READ(brdcfg[2]);
	clock = sw >> 2 & SERDES_CLOCK_MASK;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = sw & SERDES_CLOCK_MASK;
	printf("Clock2 = %sMHz", freq[clock]);

	puts("\nSERDES3 Reference : ");
	sw = QIXIS_READ(brdcfg[3]);
	clock = sw >> 6 & SERDES_CLOCK_MASK;
	printf("Clock1 = %sMHz ", freq[clock]);
	clock = sw >> 4 & SERDES_CLOCK_MASK;
	printf("Clock2 = %sMHz\n", freq[clock]);

	return 0;
}

int config_board_mux(void)
{
	u8 brdcfg;

	brdcfg = QIXIS_READ(brdcfg[4]);
	/* The BRDCFG4 register controls general board configuration.
	 *|-------------------------------------------|
	 *|Field  | Function                          |
	 *|-------------------------------------------|
	 *|5      | CAN I/O Enable (net CFG_CAN_EN_B):|
	 *|CAN_EN | 0= CAN transceivers are disabled. |
	 *|       | 1= CAN transceivers are enabled.  |
	 *|-------------------------------------------|
	 */
	brdcfg |= BIT_MASK(5);
	QIXIS_WRITE(brdcfg[4], brdcfg);

	return 0;
}

unsigned long get_board_sys_clk(void)
{
	return 100000000;
}

unsigned long get_board_ddr_clk(void)
{
	return 100000000;
}

int board_init(void)
{
	struct ccsr_gur __iomem *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR);
	struct ccsr_gur __iomem *dcsr = (void *)(DCFG_DCSR_BASE);
	u32 rcwsr28;
	u32 srds_s1;

#if defined(CONFIG_FSL_MC_ENET)
	u32 __iomem *irq_ccsr = (u32 __iomem *)ISC_BASE;
#endif
#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif

	/*
	 * Ensure the PCIe slot 6 on the T6 riser is preconfigured
	 * properly for the inserted card. This is benign and silent
	 * if the riser is not inserted as it carefully probes the I2C.
	 */
	configure_t6slot6();

	select_i2c_ch_pca9547(I2C_MUX_CH_DEFAULT);

	/* bluebox3 board uses a custom serdes protocol encoding */
	rcwsr28 = in_le32(&gur->rcwsr[28]);
	srds_s1 = rcwsr28 & FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK;
	srds_s1 >>= FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;

	/* SerDes1 protocol 11 becomes 31 */
	if (srds_s1 == 11) {
		rcwsr28 &= ~FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK;
		rcwsr28 |= 31 << FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;
		out_le32(&dcsr->rcwsr[28], rcwsr28);
	}

#if defined(CONFIG_FSL_MC_ENET)
	/* invert AQR113 IRQ pins polarity */
	out_le32(irq_ccsr + IRQCR_OFFSET / 4, AQR113_IRQ_MASK);
#endif

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif

	return 0;
}

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

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
	config_board_mux();

	return 0;
}
#endif

#ifdef CONFIG_FSL_MC_ENET
extern int fdt_fixup_board_phy(void *fdt);

void fdt_fixup_board_enet(void *fdt)
{
	int offset;

	offset = fdt_path_offset(fdt, "/soc/fsl-mc");

	if (offset < 0)
		offset = fdt_path_offset(fdt, "/fsl-mc");

	if (offset < 0) {
		printf("%s: fsl-mc node not found in device tree (error %d)\n",
		       __func__, offset);
		return;
	}

	if (get_mc_boot_status() == 0 &&
	    (is_lazy_dpl_addr_valid() || get_dpl_apply_status() == 0)) {
		fdt_status_okay(fdt, offset);
		fdt_fixup_board_phy(fdt);
	} else {
		fdt_status_fail(fdt, offset);
	}
}

void board_quiesce_devices(void)
{
	fsl_mc_ldpaa_exit(gd->bd);
}
#endif

#ifdef CONFIG_GIC_V3_ITS
void fdt_fixup_gic_lpi_memory(void *blob, u64 gic_lpi_base)
{
	u32 phandle;
	int err;
	struct fdt_memory gic_lpi;

	gic_lpi.start = gic_lpi_base;
	gic_lpi.end = gic_lpi_base + GIC_LPI_SIZE - 1;
	err = fdtdec_add_reserved_memory(blob, "gic-lpi", &gic_lpi, &phandle);
	if (err < 0)
		debug("failed to add reserved memory: %d\n", err);
}
#endif

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	int i;
	u16 mc_memory_bank = 0;

	u64 *base;
	u64 *size;
	u64 mc_memory_base = 0;
	u64 mc_memory_size = 0;
	u16 total_memory_banks;
	u64 gic_lpi_base;

	ft_cpu_setup(blob, bd);

	fdt_fixup_mc_ddr(&mc_memory_base, &mc_memory_size);

	if (mc_memory_base != 0)
		mc_memory_bank++;

	total_memory_banks = CONFIG_NR_DRAM_BANKS + mc_memory_bank;

	base = calloc(total_memory_banks, sizeof(u64));
	size = calloc(total_memory_banks, sizeof(u64));

	/* fixup DT for the three GPP DDR banks */
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		base[i] = gd->bd->bi_dram[i].start;
		size[i] = gd->bd->bi_dram[i].size;
	}

#ifdef CONFIG_GIC_V3_ITS
	gic_lpi_base = gd->arch.resv_ram - GIC_LPI_SIZE;
	gic_lpi_tables_init(gic_lpi_base, cpu_numcores());
	fdt_fixup_gic_lpi_memory(blob, gic_lpi_base);
#endif

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

	if (mc_memory_base != 0) {
		for (i = 0; i <= total_memory_banks; i++) {
			if (base[i] == 0 && size[i] == 0) {
				base[i] = mc_memory_base;
				size[i] = mc_memory_size;
				break;
			}
		}
	}

	fdt_fixup_memory_banks(blob, base, size, total_memory_banks);

#ifdef CONFIG_USB
	fsl_fdt_fixup_dr_usb(blob, bd);
#endif

#ifdef CONFIG_FSL_MC_ENET
	fdt_fsl_mc_fixup_iommu_map_entry(blob);
	fdt_fixup_board_enet(blob);
#endif
	fdt_fixup_icid(blob);

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
