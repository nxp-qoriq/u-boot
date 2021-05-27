/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020-2021 NXP
 */

#ifndef __LA1246RDB_H__
#define __LA1246RDB_H__

#include "ls1046a_common.h"

#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		100000000

#define CONFIG_LAYERSCAPE_NS_ACCESS

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
/* Physical Memory Map */
#define CONFIG_CHIP_SELECTS_PER_CTRL	4

#define CONFIG_DDR_SPD
#define SPD_EEPROM_ADDRESS		0x51
#define CONFIG_SYS_SPD_BUS_NUM		0

#define CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_MEM_INIT_VALUE           0xdeadbeef
#define CONFIG_FSL_DDR_BIST	/* enable built-in memory test */
#ifndef CONFIG_SPL
#define CONFIG_FSL_DDR_INTERACTIVE	/* Interactive debugging */
#endif

#ifndef SPL_NO_IFC
/* IFC */
#define CONFIG_FSL_IFC
/*
 * NAND Flash Definitions
 */
#define CONFIG_NAND_FSL_IFC
#endif

#define CONFIG_SYS_NAND_BASE		0x7e800000
#define CONFIG_SYS_NAND_BASE_PHYS	CONFIG_SYS_NAND_BASE

#define CONFIG_SYS_NAND_CSPR_EXT	(0x0)
#define CONFIG_SYS_NAND_CSPR	(CSPR_PHYS_ADDR(CONFIG_SYS_NAND_BASE_PHYS) \
				| CSPR_PORT_SIZE_8	\
				| CSPR_MSEL_NAND	\
				| CSPR_V)
#define CONFIG_SYS_NAND_AMASK	IFC_AMASK(64 * 1024)
#define CONFIG_SYS_NAND_CSOR	(CSOR_NAND_ECC_ENC_EN	/* ECC on encode */ \
				| CSOR_NAND_ECC_DEC_EN	/* ECC on decode */ \
				| CSOR_NAND_ECC_MODE_4	/* 4-bit ECC */ \
				| CSOR_NAND_RAL_3	/* RAL = 3 Bytes */ \
				| CSOR_NAND_PGS_2K	/* Page Size = 2K */ \
				| CSOR_NAND_SPRZ_128	/* Spare size = 128 */ \
				| CSOR_NAND_PB(64))	/* 64 Pages Per Block */

#define CONFIG_SYS_NAND_ONFI_DETECTION

#define CONFIG_SYS_NAND_FTIM0		(FTIM0_NAND_TCCST(0x7) | \
					FTIM0_NAND_TWP(0x18)   | \
					FTIM0_NAND_TWCHT(0x7) | \
					FTIM0_NAND_TWH(0xa))
#define CONFIG_SYS_NAND_FTIM1		(FTIM1_NAND_TADLE(0x32) | \
					FTIM1_NAND_TWBE(0x39)  | \
					FTIM1_NAND_TRR(0xe)   | \
					FTIM1_NAND_TRP(0x18))
#define CONFIG_SYS_NAND_FTIM2		(FTIM2_NAND_TRAD(0xf) | \
					FTIM2_NAND_TREH(0xa) | \
					FTIM2_NAND_TWHRE(0x1e))
#define CONFIG_SYS_NAND_FTIM3		0x0

#define CONFIG_SYS_NAND_BASE_LIST	{ CONFIG_SYS_NAND_BASE }
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_MTD_NAND_VERIFY_WRITE

#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)

/* IFC Timing Params */
#define CONFIG_SYS_CSPR0_EXT		CONFIG_SYS_NAND_CSPR_EXT
#define CONFIG_SYS_CSPR0		CONFIG_SYS_NAND_CSPR
#define CONFIG_SYS_AMASK0		CONFIG_SYS_NAND_AMASK
#define CONFIG_SYS_CSOR0		CONFIG_SYS_NAND_CSOR
#define CONFIG_SYS_CS0_FTIM0		CONFIG_SYS_NAND_FTIM0
#define CONFIG_SYS_CS0_FTIM1		CONFIG_SYS_NAND_FTIM1
#define CONFIG_SYS_CS0_FTIM2		CONFIG_SYS_NAND_FTIM2
#define CONFIG_SYS_CS0_FTIM3		CONFIG_SYS_NAND_FTIM3

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM		0
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x52
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* I2C bus multiplexer */
#define I2C_MUX_PCA_ADDR_PRI			0x77 /* Primary Mux*/
#define I2C_MUX_CH_DEFAULT			0x1 /* Channel 0*/
#define I2C_MUX_CH_RTC				0x2 /* Channel 1*/

/* RTC */
#define RTC
#define CONFIG_SYS_I2C_RTC_ADDR		0x51  /* Channel 0 I2C bus 0*/
#define CONFIG_SYS_RTC_BUS_NUM			0

/* PMIC */
#define CONFIG_POWER
#ifdef CONFIG_POWER
#define CONFIG_POWER_I2C
#endif

/*
 * Environment
 */
#ifndef SPL_NO_ENV
#define CONFIG_ENV_OVERWRITE

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"fdt_addr=0x64f00000\0"                 \
	"kernel_addr=0x65000000\0"              \
	"scriptaddr=0x80000000\0"               \
	"scripthdraddr=0x80080000\0"		\
	"fdtheader_addr_r=0x80100000\0"         \
	"kernelheader_addr_r=0x80200000\0"      \
	"load_addr=0xa0000000\0"            \
	"kernel_addr_r=0x81000000\0"            \
	"fdt_addr_r=0x90000000\0"               \
	"ramdisk_addr_r=0xa0000000\0"           \
	"kernel_start=0x1000000\0"		\
	"kernelheader_start=0x800000\0"		\
	"kernel_load=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"kernelheader_size=0x40000\0"		\
	"kernel_addr_sd=0x8000\0"		\
	"kernel_size_sd=0x14000\0"		\
	"kernelhdr_addr_sd=0x4000\0"		\
	"kernelhdr_size_sd=0x10\0"		\
	"console=ttyS0,115200\0"                \
	 CONFIG_MTDPARTS_DEFAULT "\0"		\
	BOOTENV					\
	"boot_scripts=la1246rdb_boot.scr\0"	\
	"boot_script_hdr=hdr_la1246rdb_bs.out\0"	\
	"scan_dev_for_boot_part="               \
		"part list ${devtype} ${devnum} devplist; "   \
		"env exists devplist || setenv devplist 1; "     \
		"for distro_bootpart in ${devplist}; do "	\
		  "if fstype ${devtype} "                  \
			"${devnum}:${distro_bootpart} "      \
			"bootfstype; then "                  \
			"run scan_dev_for_boot; "            \
		  "fi;"                                   \
		"done\0"					\
	"scan_dev_for_boot="				  \
		"echo Scanning ${devtype} "		  \
				"${devnum}:${distro_bootpart}...; "  \
		"for prefix in ${boot_prefixes}; do "	  \
			"run scan_dev_for_scripts; "	  \
		"done;"					  \
		"\0"					  \
	"scan_dev_for_scripts="                                           \
		"for script in ${boot_scripts}; do "                      \
			"if test -e ${devtype} "                          \
					"${devnum}:${distro_bootpart} "   \
					"${prefix}${script}; then "       \
				"echo Found U-Boot script "               \
					"${prefix}${script} "		\
					"in /boot${distro_bootpart}; "  \
				"run boot_a_script; "                     \
				"echo SCRIPT FAILED: continuing...; "     \
			"fi; "                                            \
		"done\0"                                                  \
	"boot_a_script="				  \
		"load ${devtype} ${devnum}:${distro_bootpart} "  \
			"${scriptaddr} ${prefix}${script}; "    \
		"env exists secureboot && load ${devtype} "     \
			"${devnum}:${distro_bootpart} "		\
			"${scripthdraddr} ${prefix}${boot_script_hdr}; " \
			"env exists secureboot "	\
			"&& esbc_validate ${scripthdraddr};"	\
		"source ${scriptaddr}\0"	  \
	"qspi_bootcmd=echo Trying load from qspi..;"      \
		"sf probe && sf read $load_addr "         \
		"$kernel_start $kernel_size; env exists secureboot "	\
		"&& sf read $kernelheader_addr_r $kernelheader_start "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; " \
		"bootm $load_addr#$board\0"		\
	"sd_bootcmd=echo Trying load from SD ..;"	\
		"mmcinfo; mmc read $load_addr "		\
		"$kernel_addr_sd $kernel_size_sd && "	\
		"env exists secureboot && mmc read $kernelheader_addr_r "		\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "		\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$board\0"			\
	"fwupdate_wdt_disable=wdt dev watchdog@2ad0000 && wdt stop\0"	\
	"fwupdate_partition_select=echo Selecting the partition;"	\
		"env exists bootdev || setenv bootdev 1; "  \
		"env exists bootdev_validate || setenv bootdev_validate 2; "  \
		"env exists bootdev_count || setenv bootdev_count 0; "  \
		"if test \"${bootdev_validate}\" = 2; then echo \"Selected partition /boot${bootdev}\";"	\
		"else if test \"${bootdev_count}\" < 3; then setexpr bootdev_count ${bootdev_count} + 1;"	\
		"echo \"Validating partition /boot/${bootdev}...$bootdev_count\"; else setenv bootdev_validate 1;"	\
		"echo \"Switch partition /boot/$bootdev\"; if test \"${bootdev}\" = 1; then setenv bootdev 2;"	\
		"else setenv bootdev 1; fi; fi; saveenv; fi;\0"	\
	"fwupdate_bootcmd=pci enum; run fwupdate_wdt_disable; run fwupdate_partition_select; run distro_bootcmd; run qspi_bootcmd;"	\
		"env exists secureboot && esbc_halt;\0"

#endif

#ifdef CONFIG_TFABOOT
#define CONFIG_SYS_MMC_ENV_DEV		0

#define CONFIG_ENV_SIZE			0x2000		/* 8KB */
#define CONFIG_ENV_OFFSET		0x500000	/* 5MB */
#define CONFIG_ENV_SECT_SIZE		0x40000		/* 256KB */
#define CONFIG_SYS_FSL_QSPI_BASE	0x40000000
#define CONFIG_ENV_ADDR		CONFIG_SYS_FSL_QSPI_BASE + CONFIG_ENV_OFFSET
#endif

/* FMan */
#ifndef SPL_NO_FMAN

#ifdef CONFIG_SYS_DPAA_FMAN
#define CONFIG_FMAN_ENET

#define FM1_10GEC1_PHY_ADDR		0x0
#define FM2_10GEC1_PHY_ADDR		0x8

#define FDT_SEQ_MACADDR_FROM_ENV

#define CONFIG_ETHPRIME			"FM1@DTSEC9"
#endif

#endif

/* QSPI device */
#ifndef SPL_NO_QSPI
#ifdef CONFIG_FSL_QSPI
#define FSL_QSPI_FLASH_SIZE		SZ_64M
#define FSL_QSPI_FLASH_NUM		1
#endif
#endif

#ifndef SPL_NO_MISC
#undef CONFIG_BOOTCOMMAND
#ifdef CONFIG_TFABOOT
#define QSPI_NOR_BOOTCOMMAND "pci enum; run distro_bootcmd; run qspi_bootcmd; "	\
			   "env exists secureboot && esbc_halt;;"
#define SD_BOOTCOMMAND "pci enum; run distro_bootcmd;run sd_bootcmd; "	\
			   "env exists secureboot && esbc_halt;"
#endif
#endif

#include <asm/fsl_secure_boot.h>

#endif /*__LA1246RDB_H__ */
