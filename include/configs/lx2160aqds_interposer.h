/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LX2_QDS_H
#define __LX2_QDS_H

#include "ls2080a_common.h"

#ifndef __ASSEMBLY__
unsigned long get_board_sys_clk(void);
unsigned long get_board_ddr_clk(void);
#endif

#define CONFIG_FSL_QIXIS	/* use common QIXIS code */
#ifdef CONFIG_FSL_QSPI
#define CONFIG_QIXIS_I2C_ACCESS
#define CONFIG_SYS_I2C_EARLY_INIT
#endif
#define CONFIG_SYS_I2C_FPGA_ADDR	0x66
#define I2C_MUX_CH_VOL_MONITOR		0xa
#define I2C_VOL_MONITOR_ADDR		0x63
#define CONFIG_SYS_CLK_FREQ		get_board_sys_clk()
#define CONFIG_DDR_CLK_FREQ		get_board_ddr_clk()
#define COUNTER_FREQUENCY_REAL		(CONFIG_SYS_CLK_FREQ/4)

#define CONFIG_DDR_SPD
#define CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_MEM_INIT_VALUE		0xdeadbeef
#define SPD_EEPROM_ADDRESS1	0x51
#define SPD_EEPROM_ADDRESS2	0x52
#define SPD_EEPROM_ADDRESS3	0x53
#define SPD_EEPROM_ADDRESS4	0x54
#define SPD_EEPROM_ADDRESS5	0x55
#define SPD_EEPROM_ADDRESS6	0x56	/* dummy address */
#define SPD_EEPROM_ADDRESS	SPD_EEPROM_ADDRESS1
#define CONFIG_SYS_SPD_BUS_NUM	0	/* SPD on I2C bus 0 */
#define CONFIG_DIMM_SLOTS_PER_CTLR		2
#define CONFIG_CHIP_SELECTS_PER_CTRL		4
#ifdef CONFIG_SYS_FSL_HAS_DP_DDR
#define CONFIG_DP_DDR_DIMM_SLOTS_PER_CTLR	1
#endif
#define CONFIG_FSL_DDR_BIST	/* enable built-in memory test */

/* SATA */
#define CONFIG_LIBATA
#define CONFIG_SCSI_AHCI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_SCSI

#define CONFIG_SYS_SATA1			AHCI_BASE_ADDR1
#define CONFIG_SYS_SATA2			AHCI_BASE_ADDR2

#define CONFIG_SYS_SCSI_MAX_SCSI_ID		1
#define CONFIG_SYS_SCSI_MAX_LUN			1
#define CONFIG_SYS_SCSI_MAX_DEVICE		(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
						CONFIG_SYS_SCSI_MAX_LUN)

#define QIXIS_LBMAP_MASK               0xe0
#define QIXIS_LBMAP_SHIFT              5
#define QIXIS_LBMAP_DFLTBANK           0x00
#define QIXIS_LBMAP_ALTBANK            0x20
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RST_CTL_RESET_EN		0x30
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define	QIXIS_RST_FORCE_MEM		0x03

#define CONFIG_SYS_LS_MC_BOOT_TIMEOUT_MS 5000

/*
 * I2C
 */
#define I2C_MUX_PCA_ADDR		0x76
#define I2C_MUX_PCA_ADDR_PRI		0x77 /* Primary Mux*/

/* I2C bus multiplexer */
#define I2C_MUX_CH_DEFAULT      0x8

/* SPI */
#if defined(CONFIG_FSL_QSPI) || defined(CONFIG_FSL_DSPI)
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_SPANSION

#ifdef CONFIG_FSL_DSPI
#define CONFIG_SPI_FLASH_SST
#define CONFIG_SPI_FLASH_EON
#endif

#ifdef CONFIG_FSL_QSPI
#define FSL_QSPI_FLASH_SIZE		(1 << 26) /* 64MB */
#define FSL_QSPI_FLASH_NUM		4
#endif
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

/*
 * RTC configuration
 */
#define RTC
#define CONFIG_RTC_PCF8563		1
#define CONFIG_SYS_I2C_RTC_ADDR         0x51
#define CONFIG_CMD_DATE

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM	0
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS 3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

#define BOOT_TARGET_DEVICES(func) \
	func(USB, usb, 0) \
	func(MMC, mmc, 0) \
	func(SCSI, scsi, 0) \
	func(DHCP, dhcp, na)

#ifdef CONFIG_PCI
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI
#endif

/*  MMC  */
#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#endif
#define CONFIG_MISC_INIT_R

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"     \
	"scriptaddr=0x80800000\0"		\
	"kernel_addr_r=0x81000000\0"		\
	"pxefile_addr_r=0x81000000\0"		\
	"fdt_addr_r=0x88000000\0"		\
	"ramdisk_addr_r=0x89000000\0"		\
	"loadaddr=0x80100000\0"			\
	"kernel_addr=0x100000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xa0000000\0"			\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_start=0x21000000\0"		\
	"mcmemsize=0x40000000\0"		\
	"mcinitcmd=fsl_mc start mc 0x20a00000" \
	" 0x20e00000 \0"                       \

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "run mcinitcmd && fsl_mc lazyapply dpl 0x20d00000" \
			   " && bootm $kernel_start"

#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
#define CONFIG_FSL_MEMAC
#define CONFIG_PHYLIB
#define CONFIG_PHYLIB_10G
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC1@xgmii"

#define CONFIG_PHY_AQUANTIA
#define AQ_PHY_ADDR1		0x00
#define AQ_PHY_ADDR2		0x01
#define AQ_PHY_ADDR3		0x02
#define AQ_PHY_ADDR4		0x03
#define AQR405_IRQ_MASK		0x36

#define CONFIG_PHY_CORTINA
#define CORTINA_NO_FW_UPLOAD
#define CORTINA_PHY_ADDR1	0x0

#define CONFIG_PHY_REALTEK
#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#define SGMII_CARD_PORT1_PHY_ADDR 0x1C
#define SGMII_CARD_PORT2_PHY_ADDR 0x1D
#define SGMII_CARD_PORT3_PHY_ADDR 0x1E
#define SGMII_CARD_PORT4_PHY_ADDR 0x1F

#endif

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_QDS_H */
