/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
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
#define NON_EXTENDED_DUTCFG
#define QIXIS_SDID_MASK				0x07
#define QIXIS_ESDHC_NO_ADAPTER			0x7


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

/* Initial environment variables */
#ifdef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_EXTRA_ENV_SETTINGS
#endif
#ifdef CONFIG_SD_BOOT
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
	"mcmemsize=0x40000000\0"		\
	"mcinitcmd=mmc read 0x80000000 0x5000 0x800;"  \
	"mmc read 0x80100000 0x7000 0x800;" \
	"fsl_mc start mc 0x80000000 0x80100000\0"		\
	"dpmac=srds:19_5_2;dpmac2:phy=0x00,mdio=1,io=2;"	\
	"dpmac3:phy=0x00,mdio=1,io=1;dpmac4:phy=0x01,mdio=1,io=1;"	\
	"dpmac5:phy=0x00,mdio=1,io=6;dpmac6:phy=0x00,mdio=1,io=6;\0"
#else
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"scriptaddr=0x80800000\0"		\
	"kernel_addr_r=0x81000000\0"		\
	"pxefile_addr_r=0x81000000\0"		\
	"fdt_addr_r=0x88000000\0"		\
	"ramdisk_addr_r=0x89000000\0"		\
	"loadaddr=0xa0000000\0"			\
	"kernel_addr=0x100000\0"		\
	"kernel_size=0x3000000\0"		\
	"kernelheader_start=0x600000\0"	\
	"kernelheader_size=0x4000\0"		\
	"kernelheader_addr_r=0x80200000\0"	\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xa0000000\0"			\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_start=0x1000000\0"		\
	"mcmemsize=0x40000000\0"		\
	"flexspinor_bootcmd=echo Trying load from flexspi nor..;"	\
	"sf probe 0:0 && sf read $loadaddr "	\
	"$kernel_start $kernel_size ; env exists secureboot &&"	\
	"sf read $kernelheader_addr_r $kernelheader_start "	\
	"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; "\
	" bootm $loadaddr\0"	\
	"mcinitcmd=echo trying to load mc....;env exists secureboot && "	\
	"esbc_validate 0x20700000 && "	\
	"esbc_validate 0x20780000 ;"	\
	"fsl_mc start mc 0x20a00000 0x20e00000;\0"	\
	"dpmac=srds:19_5_2;dpmac2:phy=0x00,mdio=1,io=2;"	\
	"dpmac3:phy=0x00,mdio=1,io=1;dpmac4:phy=0x01,mdio=1,io=1;"	\
	"dpmac5:phy=0x00,mdio=1,io=6;dpmac6:phy=0x00,mdio=1,io=6;\0"

#ifdef CONFIG_BOOTCOMMAND
#undef CONFIG_BOOTCOMMAND
#endif

#define CONFIG_BOOTCOMMAND                                             \
                       "env exists mcinitcmd && env exists secureboot "\
                       "&& esbc_validate 0x207C0000; "                 \
                       "env exists mcinitcmd && "                      \
                       "fsl_mc apply dpl 0x20d00000; "         \
                       "run flexspinor_bootcmd; "              \
                       "env exists secureboot && esbc_halt;"
#endif

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

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM	0
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS 3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

#define CONFIG_SYS_RTC_BUS_NUM		0
#define I2C_MUX_CH_RTC			0xB

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC1@xgmii"

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
#ifdef CONFIG_SD_BOOT
#define IN112525_FW_ADDR	0x980000
#else
#define IN112525_FW_ADDR	0x20980000
#endif
#define IN112525_FW_LENGTH	0x40000

#define SGMII_CARD_PORT1_PHY_ADDR 0x1C
#define SGMII_CARD_PORT2_PHY_ADDR 0x1D
#define SGMII_CARD_PORT3_PHY_ADDR 0x1E
#define SGMII_CARD_PORT4_PHY_ADDR 0x1F

#endif

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_QDS_H */
