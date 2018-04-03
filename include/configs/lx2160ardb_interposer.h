/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LX2_RDB1_H
#define __LX2_RDB1_H

#include "ls2080a_common.h"

#ifndef __ASSEMBLY__
unsigned long get_board_sys_clk(void);
unsigned long get_board_ddr_clk(void);
#endif

#ifdef CONFIG_FSL_QSPI
#define CONFIG_QIXIS_I2C_ACCESS
#define CONFIG_SYS_I2C_EARLY_INIT
#endif

#define CONFIG_SYS_I2C_FPGA_ADDR	0x66
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

/* undefined CONFIG_FSL_DDR_SYNC_REFRESH for simulator */

#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)

#define CONFIG_FSL_QIXIS	/* use common QIXIS code */
#define QIXIS_LBMAP_MASK		0xe0
#define QIXIS_LBMAP_SHIFT		5
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		0x20
#define QIXIS_LBMAP_NAND		0x09
#define QIXIS_LBMAP_SD			0x00
#define QIXIS_LBMAP_QSPI		0x0f
#undef QIXIS_LBMAP_QSPI
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RST_CTL_RESET_EN		0x30
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define QIXIS_RCW_SRC_NAND		0x107
#define QIXIS_RCW_SRC_SD		0x40
#define QIXIS_RCW_SRC_QSPI		0x62
#define	QIXIS_RST_FORCE_MEM		0x03

#define CONFIG_SYS_LS_MC_BOOT_TIMEOUT_MS 5000

/*
 * I2C
 */
#define I2C_MUX_PCA_ADDR		0x76
#define I2C_MUX_PCA_ADDR_PRI		0x77 /* Primary Mux*/

/* I2C bus multiplexer */
#define I2C_MUX_CH_DEFAULT      0x8


/* VID */
#define I2C_MUX_CH_VOL_MONITOR          0xA
/* Voltage monitor on channel 2*/
#define I2C_VOL_MONITOR_ADDR           0x63
#define I2C_VOL_MONITOR_BUS_V_OFFSET   0x2
#define I2C_VOL_MONITOR_BUS_V_OVF      0x1
#define I2C_VOL_MONITOR_BUS_V_SHIFT    3
/* PM Bus commands code for LTC3882*/
#define PMBUS_CMD_PAGE                  0x0
#define PMBUS_CMD_READ_VOUT             0x8B
#define PMBUS_CMD_PAGE_PLUS_WRITE       0x05
#define PMBUS_CMD_VOUT_COMMAND          0x21
#define PWM_CHANNEL0                    0x0

#define CONFIG_VID_FLS_ENV              "lx2rdb1_vdd_mv"
#define CONFIG_VID

/* The lowest and highest voltage allowed for LX2RDB1 */
#define VDD_MV_MIN                     819
#define VDD_MV_MAX                     1212

#define CONFIG_VOL_MONITOR_LTC3882_SET
#define CONFIG_VOL_MONITOR_LTC3882_READ

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
/*
 * Verify QSPI when boot from NAND, QIXIS brdcfg9 need configure.
 * If boot from on-board NAND, ISO1 = 1, ISO2 = 0, IBOOT = 0
 * If boot from IFCCard NAND, ISO1 = 0, ISO2 = 0, IBOOT = 1
 */

#endif

/*
 * RTC configuration
 */
#define RTC
#define CONFIG_RTC_PCF2127		1
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

/* Initial environment variables */
#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
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

#undef CONFIG_BOOTARGS
#define CONFIG_BOOTARGS		"console=ttyS0,115200 root=/dev/ram0 " \
				"earlycon=uart8250,mmio,0x21c0500 " \
				"ramdisk_size=0x2000000 default_hugepagesz=2m" \
				" hugepagesz=2m hugepages=256"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "run mcinitcmd && fsl_mc lazyapply dpl 0x20d00000" \
			   " && bootm $kernel_start"

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
#define CONFIG_FSL_MEMAC
#define CONFIG_PHYLIB
#define CONFIG_PHYLIB_10G
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC1@xgmii"

#define CONFIG_PHY_ATHEROS
#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#define CONFIG_PHY_AQUANTIA
#define AQR107_PHY_ADDR1	0x04
#define AQR107_PHY_ADDR2	0x05

#define CONFIG_PHY_CORTINA
#define CORTINA_NO_FW_UPLOAD
#define CORTINA_PHY_ADDR1	0x0

#define INPHI_PHY_ADDR1		0x00

#endif

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_RDB1_H */
