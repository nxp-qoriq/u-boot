/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020 NXP
 */

#ifndef __LS1046AWRDB_H__
#define __LS1046AWRDB_H__

#include "ls1046a_common.h"

#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		100000000

#define CONFIG_LAYERSCAPE_NS_ACCESS

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
/* Physical Memory Map */
#define CONFIG_CHIP_SELECTS_PER_CTRL	4

/* IFC: Not used on this board */
#ifndef SPL_NO_IFC
#define CONFIG_FSL_IFC
#define CONFIG_NAND_FSL_IFC
#endif
#define CONFIG_SYS_NAND_BASE		0x7e800000
#define CONFIG_SYS_MAX_NAND_DEVICE	1

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM		0
#ifdef CONFIG_TARGET_DB1046
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x52
#else
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x57
#endif
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/*RTC*/
#define CONFIG_SYS_I2C_RTC_ADDR         0x51

/*
 * I2C IO expander
 */
#define I2C_MUX_IO_ADDR		0x20
#define I2C_MUX_IO2_ADDR	0x21
#define I2C_MUX_IO_1		1
#define SW_QSPI_BOOT_MASK	0x40
#define SW_SD_EMMC_BOOT_MASK	0x80
#define SW_BOOT_QSPI_DEV0	0x00
#define SW_BOOT_QSPI_DEV1	0x40
#define SW_BOOT_SD		0x00
#define SW_BOOT_EMMC		0x80

#define PCAL_P0_CONF_ADDR	0x6
#define PCAL_P1_CONF_ADDR	0x7
#define PCAL_P0_OUTPUT_ADDR	0x2
#define PCAL_P1_OUTPUT_ADDR	0x3
#define PCAL0_P0_OUTPUT		0x00
#define PCAL0_P0_QSPI		0x44
#define PCAL0_P0_SD_EMMC	0x04
#define PCAL0_P1_0_OUTPUT	0xfe
#define PCAL1_P0_6_OUTPUT	0xbf
#define PCAL1_P1_7_OUTPUT	0x7f

#define PCAL0_P1_0_SD_QSPI	0xfe

#define PCAL1_P1_6_QSPI_DEV0	0xbf
#define PCAL1_P1_6_QSPI_DEV1	0xff
#define PCAL1_P1_7_SD		0x7f
#define PCAL1_P1_7_EMMC		0xff

#ifndef CONFIG_TARGET_DB1046
#define I2C_MUX_PCA_ADDR_PRI	0x77
#define I2C_MUX_CH_DEFAULT	0x8
#endif

/*TODO: Add more details here*/

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C

/*
 * Environment
 */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_SYS_MMC_ENV_DEV		0

#define CONFIG_ENV_SIZE			0x2000		/* 8KB */
#define CONFIG_ENV_OFFSET		0x500000	/* 5MB */
#define CONFIG_ENV_SECT_SIZE		0x10000		/* 64KB */
#define CONFIG_SYS_FSL_QSPI_BASE        0x40000000
#define CONFIG_ENV_ADDR CONFIG_SYS_FSL_QSPI_BASE + CONFIG_ENV_OFFSET

/* FMan */
#ifndef CONFIG_TARGET_DB1046
#define RGMII_PHY1_ADDR			0x1

#define SGMII_PHY1_ADDR			0x2
#define SGMII_PHY2_ADDR			0x3
#endif

#define FM1_10GEC2_PHY_ADDR		0x0

#define FDT_SEQ_MACADDR_FROM_ENV

#define CONFIG_ETHPRIME			"FM1@TGEC1"

/* QSPI device */
#define FSL_QSPI_FLASH_SIZE		SZ_64M	/* 64MB */
#define FSL_QSPI_FLASH_NUM		2

#undef CONFIG_BOOTCOMMAND
#define QSPI_NOR_BOOTCOMMAND "run distro_bootcmd; run qspi_bootcmd; "	\
			   "env exists secureboot && esbc_halt;;"
#define SD_BOOTCOMMAND "run distro_bootcmd;run sd_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"

#include <asm/fsl_secure_boot.h>

#endif /* __LS1046AWRDB_H__ */
