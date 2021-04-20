/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2021 NXP
 */

#ifndef __LA1238_RDB_H
#define __LA1238_RDB_H

#include "lx2160a_common.h"

#define PCAL_BUS_NO		1
#define BOOT_FROM_XSPI		1
#define BOOT_FROM_EMMC		2
#define BOOT_FROM_PCIE		2
#define BOOT_FROM_PEB		3
#define PCAL_CPU_ADDR		0x20
#define PCAL_MODEM_ADDR		0x21
#define PCAL_INPUT_PORT		0x00
#define PCAL_OUTPUT_PORT	0x01
#define PCAL_POL_INV		0x02
#define PCAL_CONFIG		0x03
#define PCAL_ODS_0		0x40
#define PCAL_ODS_1		0x41
#define PCAL_INPUT_LATCH	0x42
#define PCAL_PU_PD_ENABLE	0x43
#define PCAL_PU_PD_SEL		0x44
#define PCAL_INT_MASK		0x45
#define PCAL_INT_STATUS		0x46
#define PCAL_OUT_PORT_CONFIG	0x47

#undef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(DHCP, dhcp, na)
/*
 * Need to override existing (lx2160a) with la1238-rdb so set_board_info will
 * use proper prefix when creating full board_name (SYS_BOARD + type)
 */
#undef CONFIG_SYS_BOARD
#define CONFIG_SYS_BOARD                "la1238rdb"

#undef CONFIG_SYS_NXP_SRDS_3

/* Make channel 0 default */
#undef I2C_MUX_CH_DEFAULT
#define I2C_MUX_CH_DEFAULT		1

/* VID */
#define I2C_MUX_CH_VOL_MONITOR		0x2
/* Voltage monitor on channel 2*/
#define I2C_VOL_MONITOR_ADDR		0x63
#define I2C_VOL_MONITOR_BUS_V_OFFSET	0x2
#define I2C_VOL_MONITOR_BUS_V_OVF	0x1
#define I2C_VOL_MONITOR_BUS_V_SHIFT	3
#define CONFIG_VID_FLS_ENV		"la1238rdb_vdd_mv"
#define CONFIG_VID

/* The lowest and highest voltage allowed*/
#define VDD_MV_MIN			775
#define VDD_MV_MAX			925

/* PM Bus commands code for LTC3882*/
#define PMBUS_CMD_PAGE                  0x0
#define PMBUS_CMD_READ_VOUT             0x8B
#define PMBUS_CMD_PAGE_PLUS_WRITE       0x05
#define PMBUS_CMD_VOUT_COMMAND          0x21
#define PWM_CHANNEL0                    0x0

#define CONFIG_VOL_MONITOR_LTC3882_SET
#define CONFIG_VOL_MONITOR_LTC3882_READ

/* RTC */
#define CONFIG_SYS_RTC_BUS_NUM		0

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC3@usxgmii"
#define AQR113_PHY_ADDR		0x8
#define AQR113_IRQ_MASK		0x800 /* IRQ-11 */
#endif

/* EEPROM */
#undef CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x52
/* Rest of the EEPROM related configs come from common include file */

#undef SD2_MC_INIT_CMD
#define SD2_MC_INIT_CMD				\
	"mmc dev 0; mmc read 0x80a00000 0x5000 0x1200;"	\
	"mmc read 0x80e00000 0x7000 0x800;"	\
	"env exists secureboot && "		\
	"mmc read 0x80640000 0x3200 0x20 && "	\
	"mmc read 0x80680000 0x3400 0x20 && "	\
	"esbc_validate 0x80640000 && "		\
	"esbc_validate 0x80680000 ;"		\
	"fsl_mc start mc 0x80a00000 0x80e00000\0"

#undef SD2_BOOTCOMMAND
#define SD2_BOOTCOMMAND						\
	"mmc dev 0; env exists mcinitcmd && mmcinfo; "	\
	"mmc read 0x80d00000 0x6800 0x800; "		\
	"env exists mcinitcmd && env exists secureboot "	\
	" && mmc read 0x806C0000 0x3600 0x20 "		\
	"&& esbc_validate 0x806C0000;env exists mcinitcmd "	\
	"&& fsl_mc lazyapply dpl 0x80d00000;"		\
	"run distro_bootcmd;run emmc_bootcmd;"		\
	"env exists secureboot && esbc_halt;"

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	EXTRA_ENV_SETTINGS			\
	"boot_scripts=la1238rdb_boot.scr\0"	\
	"boot_script_hdr=hdr_la1238rdb_bs.out\0"	\
	"xspi_bootcmd=echo Trying load from flexspi..;"		\
		"sf probe 0:0 && sf read $load_addr "		\
		"$kernel_start $kernel_size ; env exists secureboot &&"	\
		"sf read $kernelheader_addr_r $kernelheader_start "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; "\
		" bootm $load_addr#$board\0"			\
	"emmc_bootcmd=echo Trying load from emmc card..;"	\
		"mmc dev 0; mmcinfo; mmc read $load_addr "	\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$board\0"			\
	"othbootargs=default_hugepagesz=1024m hugepagesz=1024m"	\
		" hugepages=2 mem=13726M\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LA1238_RDB_H */
