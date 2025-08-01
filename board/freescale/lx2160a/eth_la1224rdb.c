// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020-2022, 2025 NXP
 *
 */

#include <command.h>
#include <netdev.h>
#include <malloc.h>
#include <fsl_mdio.h>
#include <miiphy.h>
#include <phy.h>
#include <dm.h>
#include <i2c.h>
#include <fm_eth.h>
#include <asm/io.h>
#include <exports.h>
#include <asm/arch/fsl_serdes.h>
#include <fsl-mc/fsl_mc.h>
#include <fsl-mc/ldpaa_wriop.h>
#include "lx2160a.h"

DECLARE_GLOBAL_DATA_PTR;

static int ti_ds250_reg_rmw(u8 addr, u8 offset, u8 val, u8 mask)
{
#ifdef CONFIG_DM_I2C
	u8 tmp;
	struct udevice *dev;

	if (i2c_get_chip_for_busnum(0, addr, 1, &dev)) {
		printf("%s Error in i2c_get_chip_for_busnum !\n", __func__);
		return -1;
	}

	if (dm_i2c_read(dev, offset, &tmp, 1)) {
		printf("%s Error reading from TI retimer (addr=%#x\n",
		       __func__, addr);
		return -1;
	}

	tmp &= 0xFF - mask;
	tmp |= val;

	if (dm_i2c_write(dev, offset, &tmp, 1)) {
		printf("%sError writing to TI retimer (addr=%#x)\n",
		       __func__, addr);
		return -1;
	}

	return 0;
#else
	printf("%s Not supported. Device model for I2C must be enabled\n",
	       __func__);
	return -1;
#endif
}

static int ti_ds250_setup(int addr, int half_rate)
{
	int i = 0, cnt = 0;
	struct ti_retimer_cmd {
		u8 offset;
		u8 val;
		u8 mask;
	} *ti_cmd;

	/* custom TI ds250 parameters for LA1224RDB */
	struct ti_retimer_cmd ti_ds250_25g[] = {
		/* channel 0 custom settings */
		{ 0xFC, 0x01, 0xFF }, { 0xFF, 0x03, 0x03 },
		{ 0x00, 0x04, 0x04 }, { 0x0A, 0x0C, 0x0C },
		{ 0x60, 0x74, 0xFF }, { 0x61, 0xC0, 0xFF },
		{ 0x62, 0x74, 0xFF }, { 0x63, 0xC0, 0xFF },
		{ 0x64, 0xFF, 0xFF }, { 0x09, 0x04, 0x04 },
		{ 0x18, 0x00, 0x70 }, { 0x31, 0x20, 0x60 },
		{ 0x1E, 0x08, 0x08 }, { 0x3D, 0x80, 0x80 },
		{ 0x3D, 0x00, 0x40 }, { 0x3F, 0x40, 0x40 },
		{ 0x3E, 0x40, 0x40 }, { 0x3D, 0x0E, 0x1F },
		{ 0x3F, 0x02, 0x0F }, { 0x3E, 0x01, 0x0F },
		{ 0x0A, 0x00, 0x0C },
		/* channel 1 custom settings */
		{ 0xFC, 0x02, 0xFF }, { 0xFF, 0x01, 0x03 },
		{ 0x00, 0x04, 0x04 }, { 0x0A, 0x0C, 0x0C },
		{ 0x60, 0x74, 0xFF }, { 0x61, 0xC0, 0xFF },
		{ 0x62, 0x74, 0xFF }, { 0x63, 0xC0, 0xFF },
		{ 0x64, 0xFF, 0xFF }, { 0x09, 0x04, 0x04 },
		{ 0x18, 0x00, 0x70 }, { 0x31, 0x40, 0x60 },
		{ 0x1E, 0x02, 0x0A }, { 0x3D, 0x80, 0x80 },
		{ 0x3D, 0x00, 0x40 }, { 0x3F, 0x40, 0x40 },
		{ 0x3E, 0x40, 0x40 }, { 0x3D, 0x0A, 0x1F },
		{ 0x3F, 0x01, 0x0F }, { 0x3E, 0x01, 0x0F },
		{ 0xFF, 0x03, 0x03 }, { 0x0A, 0x00, 0x0C },
	};

	struct ti_retimer_cmd ti_ds250_10g[] = {
		/* channel 0 custom settings */
		{ 0xFC, 0x01, 0xFF }, { 0xFF, 0x03, 0x03 },
		{ 0x00, 0x04, 0x04 }, { 0x0A, 0x0C, 0x0C },
		{ 0x60, 0x90, 0xFF }, { 0x61, 0xB3, 0xFF },
		{ 0x62, 0x90, 0xFF }, { 0x63, 0xB3, 0xFF },
		{ 0x64, 0xFF, 0xFF }, { 0x09, 0x04, 0x04 },
		{ 0x18, 0x10, 0x70 }, { 0x31, 0x20, 0x60 },
		{ 0x1E, 0x08, 0x08 }, { 0x3D, 0x80, 0x80 },
		{ 0x3D, 0x00, 0x40 }, { 0x3F, 0x40, 0x40 },
		{ 0x3E, 0x40, 0x40 }, { 0x3D, 0x0E, 0x1F },
		{ 0x3F, 0x02, 0x0F }, { 0x3E, 0x01, 0x0F },
		{ 0x0A, 0x00, 0x0C },
		/* channel 1 custom settings */
		{ 0xFC, 0x02, 0xFF }, { 0xFF, 0x01, 0x03 },
		{ 0x00, 0x04, 0x04 }, { 0x0A, 0x0C, 0x0C },
		{ 0x60, 0x90, 0xFF }, { 0x61, 0xB3, 0xFF },
		{ 0x62, 0x90, 0xFF }, { 0x63, 0xB3, 0xFF },
		{ 0x64, 0xFF, 0xFF }, { 0x09, 0x04, 0x04 },
		{ 0x18, 0x10, 0x70 }, { 0x31, 0x40, 0x60 },
		{ 0x1E, 0x02, 0x0A }, { 0x3D, 0x80, 0x80 },
		{ 0x3D, 0x00, 0x40 }, { 0x3F, 0x40, 0x40 },
		{ 0x3E, 0x40, 0x40 }, { 0x3D, 0x0A, 0x1F },
		{ 0x3F, 0x01, 0x0F }, { 0x3E, 0x01, 0x0F },
		{ 0xFF, 0x03, 0x03 }, { 0x0A, 0x00, 0x0C },
	};

	/* pick correct settings for 10G or 25G operation */
	ti_cmd = !!half_rate ? ti_ds250_10g : ti_ds250_25g;
	cnt = !!half_rate ? sizeof(ti_ds250_10g) : sizeof(ti_ds250_25g);
	cnt /= sizeof(struct ti_retimer_cmd);

	/* apply settings */
	for (i = 0; i < cnt; i++)
		if (ti_ds250_reg_rmw(addr, ti_cmd[i].offset, ti_cmd[i].val,
				     ti_cmd[i].mask))
			return -1;

	return 0;
}

int board_retimer_init(void)
{
	struct ccsr_gur *gur = (void *)(CFG_SYS_FSL_GUTS_ADDR);
	u32 srds_s1;

	srds_s1 = in_le32(&gur->rcwsr[28]) & FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK;
	srds_s1 >>= FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;

	/* Configure the retimer according to the SerDes protocol on block #1 */
	switch (srds_s1) {
	case 18:
		ti_ds250_setup(TI_DS250_I2C_ADDR, 0);
		break;
	case 22:
		ti_ds250_setup(TI_DS250_I2C_ADDR, 1);
		break;
	default:
		printf("%sSerDes1 protocol 0x%x is not supported on LA1224RDB\n",
		       __func__, srds_s1);
		return -EOPNOTSUPP;
	}

	return 0;
}

#if defined(CONFIG_RESET_PHY_R)
void reset_phy(void)
{
#if defined(CONFIG_FSL_MC_ENET)
	mc_env_boot();
#endif
}
#endif /* CONFIG_RESET_PHY_R */

int fdt_fixup_board_phy(void *fdt)
{
	/* TO DO */
	return 0;
}

int board_fit_config_name_match(const char *name)
{
	struct ccsr_gur *gur = (void *)(CFG_SYS_FSL_GUTS_ADDR);
	u32 rcw_status = in_le32(&gur->rcwsr[28]);
	char expected_dts[100];
	u32 srds_s1;

	srds_s1 = rcw_status & FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK;
	srds_s1 >>= FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;

	if (srds_s1 == 22)
		sprintf(expected_dts, "fsl-la1224-rdb-sd1-22");
	else
		sprintf(expected_dts, "fsl-la1224-rdb");

	if (!strcmp(name, expected_dts))
		return 0;

	return -1;
}
