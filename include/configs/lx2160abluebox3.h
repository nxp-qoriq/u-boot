/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020 NXP
 */

#ifndef __LX2_BLUEBOX3_H
#define __LX2_BLUEBOX3_H

#include "lx2160a_common.h"

/* USB */
#define CONFIG_USB_MAX_CONTROLLER_COUNT 1

/* Qixis */
#define QIXIS_XMAP_MASK			0x01
#define QIXIS_XMAP_SHIFT		5
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		QIXIS_XMAP_MASK << QIXIS_XMAP_SHIFT
#define QIXIS_LBMAP_QSPI		0x00
#define QIXIS_RCW_SRC_QSPI		0xff
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define QIXIS_LBMAP_MASK		0x0f
#define QIXIS_LBMAP_SD
#define QIXIS_LBMAP_EMMC
#define QIXIS_RCW_SRC_SD           0x08
#define QIXIS_RCW_SRC_EMMC         0x09
#define NON_EXTENDED_DUTCFG

/* T6 riser I2C extension for slot 6 support */
#define I2C_PCIE_BUS_NUM		2	/* PCIe on I2C6 */
#define I2C_PCIE_MUX_PCA9846_ADDR_PRI	0x77	/* On main board */
#define I2C_PCIE_MUX_CH_PCIE_MB2	0x4
#define I2C_T6_PCA9546_ADDR		0x71	/* On T6 */
#define I2C_T6_MUX_CH_SLOT6CTRL		0x8
#define I2C_T6_PCAL6408_ADDR		0x20
#define I2C_T6_PCAL6408_INPUT_PORT	0
#define I2C_T6_PCAL6408_PRESENCE_DETECT_MASK	0x0f
#define I2C_T6_PTN3944_BASE_ADDR	0x30

#define PTN3944_LINK_CTRL_STATUS	0x06
#define PTN3944_LCS_CHANNEL_MASK	0x0c
#define PTN3944_LCS_0CHANNELS		0x00
#define PTN3944_LCS_1CHANNELS		0x04
#define PTN3944_LCS_2CHANNELS		0x08
#define PTN3944_LCS_4CHANNELS		0x0c

/* VID */

#define I2C_MUX_CH_VOL_MONITOR		0xA
/* Voltage monitor LTC7132 on channel 2*/
#define I2C_VOL_MONITOR_ADDR		0x63
#define CONFIG_VID_FLS_ENV		"lx2160bluebox3_vdd_mv"
#define CONFIG_VID

/* The lowest and highest voltage allowed*/
#define VDD_MV_MIN			775
#define VDD_MV_MAX			855

/* Channel to be used with LTC7132 */
#define PWM_CHANNEL0                    0xff

#define CONFIG_VOL_MONITOR_LTC7132_SET
#define CONFIG_VOL_MONITOR_LTC7132_READ

/* RTC */
#define CONFIG_SYS_RTC_BUS_NUM		0   /* Channel 4 I2C bus 0*/
#define I2C_MUX_CH_RTC			0xC /* Channel 4*/

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC17@rgmii-id"

#define AQR113_PHY_ADDR1	0x08
#define AQR113_PHY_ADDR2	0x00
#define AQR113_PHY_ADDR3	0x08
#define AQR113_PHY_ADDR4	0x00
#define AQR113_IRQ_MASK		0x3C

#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#endif

/* EMC2305 */
#define I2C_MUX_CH_EMC2305	0x9
#define I2C_EMC2305_ADDR1	0x4C
#define I2C_EMC2305_ADDR2	0x4D
#define I2C_EMC2305_CMD		0x40
#define I2C_EMC2305_PWM		0x80

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM	           0
#define CONFIG_SYS_I2C_EEPROM_ADDR	           0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	    1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS     3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

/* SerDes */
#define SERDES_CLOCK_MASK                      0x3

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	EXTRA_ENV_SETTINGS			\
	"boot_scripts=lx2160bluebox3_boot.scr\0"	\
	"boot_script_hdr=hdr_lx2160bluebox3_bs.out\0"	\
	"BOARD=lx2160abluebox3\0"			\
	"xspi_bootcmd=echo Trying load from flexspi..;"		\
		"sf probe 0:0 && sf read $load_addr "		\
		"$kernel_start $kernel_size ; env exists secureboot &&"	\
		"sf read $kernelheader_addr_r $kernelheader_start "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; "\
		" bootm $load_addr#$BOARD\0"			\
	"sd_bootcmd=echo Trying load from sd card..;"		\
		"mmcinfo; mmc read $load_addr "			\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$BOARD\0"			\
	"sd2_bootcmd=echo Trying load from emmc card..;"	\
		"mmc dev 1; mmcinfo; mmc read $load_addr "	\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$BOARD\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_BLUEBOX3_H */
