/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018,2020 NXP
 */

#ifndef __LX2_PCHIPLET_H
#define __LX2_PCHIPLET_H

#include "lx2160a_common.h"

/* Qixis */
#undef CONFIG_FSL_QIXIS

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC1@xgmii"

#define AQR107_PHY_ADDR1	0x04
#define AQR107_PHY_ADDR2	0x05
#define AQR107_IRQ_MASK		0x0C

#define CORTINA_NO_FW_UPLOAD
#define CORTINA_PHY_ADDR1	0x0
#define INPHI_PHY_ADDR1		0x0
#ifdef CONFIG_SD_BOOT
#define IN112525_FW_ADDR        0x980000
#else
#define IN112525_FW_ADDR        0x20980000
#endif
#define IN112525_FW_LENGTH      0x40000

#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#endif

/* FlexSPI */
#undef NXP_FSPI_FLASH_SIZE
#define NXP_FSPI_FLASH_SIZE        SZ_256M

/* EEPROM */
#undef CONFIG_ID_EEPROM

#define CONFIG_EXTRA_ENV_SETTINGS		\
	EXTRA_ENV_SETTINGS			\
	"boot_scripts=lx2160apchiplet_boot.scr\0"	\
	"boot_script_hdr=hdr_lx2160ardb_bs.out\0"	\
	"BOARD=lx2160ardb\0"			\
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
	"emmc_bootcmd=echo Trying load from emmc card..;"	\
		"mmc dev 1; mmcinfo; mmc read $load_addr "	\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$BOARD\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_PCHIPLET_H */
