/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018 NXP
 */

#ifndef __LX2_QDS_H
#define __LX2_QDS_H

#include "lx2160a_common.h"

/* Qixis */
#define QIXIS_XMAP_MASK			0x07
#define QIXIS_XMAP_SHIFT		5
#define QIXIS_RST_CTL_RESET_EN		0x30
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		0x20
#define QIXIS_LBMAP_QSPI		0x00
#define QIXIS_RCW_SRC_QSPI		0xff
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define QIXIS_LBMAP_MASK		0x0f
#define QIXIS_LBMAP_SD
#define QIXIS_RCW_SRC_SD           0x08
#define QIXIS_SDID_MASK				0x07
#define QIXIS_ESDHC_NO_ADAPTER			0x7
#define NON_EXTENDED_DUTCFG

/* SYSCLK */
#define QIXIS_SYSCLK_100		0x0
#define QIXIS_SYSCLK_125		0x1
#define QIXIS_SYSCLK_133		0x2

/* DDRCLK */
#define QIXIS_DDRCLK_100		0x0
#define QIXIS_DDRCLK_125		0x1
#define QIXIS_DDRCLK_133		0x2

#define BRDCFG4_EMI1SEL_MASK		0x38
#define BRDCFG4_EMI1SEL_SHIFT		3
#define BRDCFG4_EMI2SEL_MASK		0x07
#define BRDCFG4_EMI2SEL_SHIFT		0

/*
 * MMC
 */
#ifdef CONFIG_MMC
#ifndef __ASSEMBLY__
u8 qixis_esdhc_detect_quirk(void);
#endif
#define CONFIG_ESDHC_DETECT_QUIRK  qixis_esdhc_detect_quirk()
#endif

/* DSPI */
#ifdef CONFIG_FSL_DSPI
#define CONFIG_SPI_FLASH_SST
#define CONFIG_SPI_FLASH_EON
#endif

/* RTC */
#define CONFIG_SYS_RTC_BUS_NUM		0
#define I2C_MUX_CH_RTC			0xB

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC17@rgmii-id"

#define AQ_PHY_ADDR1		0x00
#define AQ_PHY_ADDR2		0x01
#define AQ_PHY_ADDR3		0x02
#define AQ_PHY_ADDR4		0x03
#define AQR405_IRQ_MASK		0x36

#define CORTINA_NO_FW_UPLOAD
#define CORTINA_PHY_ADDR1	0x0

#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#define INPHI_PHY_ADDR1		0x0
#define INPHI_PHY_ADDR2		0x1

#define SGMII_CARD_PORT1_PHY_ADDR 0x1C
#define SGMII_CARD_PORT2_PHY_ADDR 0x1D
#define SGMII_CARD_PORT3_PHY_ADDR 0x1E
#define SGMII_CARD_PORT4_PHY_ADDR 0x1F

#endif

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"scriptaddr=0x80800000\0"		\
	"kernel_addr_r=0x81000000\0"		\
	"pxefile_addr_r=0x81000000\0"		\
	"fdt_addr_r=0x88000000\0"		\
	"ramdisk_addr_r=0x89000000\0"		\
	"loadaddr=0x80100000\0"			\
	"kernel_addr=0x100000\0"		\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xa0000000\0"			\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_start=0x21000000\0"		\
	"mcmemsize=0x40000000\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_QDS_H */
