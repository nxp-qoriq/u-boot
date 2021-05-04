/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020-2021 NXP
 */

#ifndef __LA1224_RDB_H
#define __LA1224_RDB_H

#include "lx2160a_common.h"

/* VID */
#define I2C_MUX_CH_VOL_MONITOR		0xA
/* Voltage monitor on channel 2*/
#define I2C_VOL_MONITOR_ADDR		0x63
#define I2C_VOL_MONITOR_BUS_V_OFFSET	0x2
#define I2C_VOL_MONITOR_BUS_V_OVF	0x1
#define I2C_VOL_MONITOR_BUS_V_SHIFT	3
#define CONFIG_VID_FLS_ENV		"la1224rdb_vdd_mv"
#define CONFIG_VID
/* The lowest and highest voltage allowed*/
#define VDD_MV_MIN			775
#define VDD_MV_MAX			855

/* PM Bus commands code for LTC3882*/
#define PMBUS_CMD_PAGE                  0x0
#define PMBUS_CMD_READ_VOUT             0x8B
#define PMBUS_CMD_PAGE_PLUS_WRITE       0x05
#define PMBUS_CMD_VOUT_COMMAND          0x21
#define PWM_CHANNEL0                    0x0

#define CONFIG_VOL_MONITOR_LTC3882_SET
#define CONFIG_VOL_MONITOR_LTC3882_READ

/* RTC */
#define CONFIG_SYS_RTC_BUS_NUM		3

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC17@rgmii-id"

#define AQR113_PHY_ADDR1	0x08
#define AQR113_PHY_ADDR2	0
#define AQR113_IRQ_MASK		0x0C

#define RGMII_PHY_ADDR1		0x03

#define TI_DS250_I2C_ADDR	0x18

#endif

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM	           0
#define CONFIG_SYS_I2C_EEPROM_ADDR	           0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	    1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS     3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

/* PACL6461A IO Expander*/
#define I2C_IO_EXP_ADDR_PRI                  0x20
#define I2C_IO_EXP_ADDR_SEC                  0x21

/* Serial */
#define UART_EN_MASK			      0x4

/*
 * Need to override existing one (lx2160a) with la1224rdb so set_board_info will
 * use proper prefix when creating full board_name (SYS_BOARD + type)
 */
#undef CONFIG_SYS_BOARD
#define CONFIG_SYS_BOARD                "la1224rdb"

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	EXTRA_ENV_SETTINGS			\
	"boot_scripts=la1224rdb_boot.scr\0"	\
	"boot_script_hdr=hdr_la1224rdb_bs.out\0"	\
	"BOARD=la1224rdb\0"			\
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
		"bootm $load_addr#$BOARD\0"				\
	"sd2_bootcmd=echo Trying load from emmc card..;"		\
		"mmc dev 1; mmcinfo; mmc read $load_addr "			\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$BOARD\0"			\
	"othbootargs=default_hugepagesz=1024m hugepagesz=1024m"	\
		" hugepages=2 mem=13758M\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LA1224_RDB_H */
