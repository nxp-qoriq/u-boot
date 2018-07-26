/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LS1088A_RDB_PB_H
#define __LS1088A_RDB_PB_H

#include "ls1088a_common.h"
#include "ls1088ardb.h"

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"BOARD=ls1088ardb_pb\0"			\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xa0000000\0"			\
	"initrd_high=0xffffffffffffffff\0"	\
	"fdt_addr=0x64f00000\0"			\
	"kernel_addr=0x1000000\0"		\
	"kernel_addr_sd=0x8000\0"		\
	"kernelhdr_addr_sd=0x4000\0"		\
	"kernel_start=0x580100000\0"		\
	"kernelheader_start=0x580800000\0"	\
	"scriptaddr=0x80000000\0"		\
	"scripthdraddr=0x80080000\0"		\
	"fdtheader_addr_r=0x80100000\0"		\
	"kernelheader_addr=0x800000\0"		\
	"kernelheader_addr_r=0x80200000\0"	\
	"kernel_addr_r=0x81000000\0"		\
	"kernelheader_size=0x40000\0"		\
	"fdt_addr_r=0x90000000\0"		\
	"load_addr=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"kernel_size_sd=0x14000\0"		\
	"kernelhdr_size_sd=0x10\0"		\
	MC_INIT_CMD				\
	BOOTENV					\
	"boot_scripts=ls1088ardb_boot.scr\0"	\
	"boot_script_hdr=hdr_ls1088ardb_bs.out\0"	\
	"scan_dev_for_boot_part="		\
		"part list ${devtype} ${devnum} devplist; "	\
		"env exists devplist || setenv devplist 1; "	\
		"for distro_bootpart in ${devplist}; do "	\
			"if fstype ${devtype} "			\
				"${devnum}:${distro_bootpart} "	\
				"bootfstype; then "		\
				"run scan_dev_for_boot; "	\
			"fi; "					\
		"done\0"					\
	"scan_dev_for_boot="					\
		"echo Scanning ${devtype} "			\
		"${devnum}:${distro_bootpart}...; "		\
		"for prefix in ${boot_prefixes}; do "		\
			"run scan_dev_for_scripts; "		\
		"done;\0"					\
	"boot_a_script="					\
		"load ${devtype} ${devnum}:${distro_bootpart} " \
		"${scriptaddr} ${prefix}${script}; "		\
	"env exists secureboot && load ${devtype} "		\
		"${devnum}:${distro_bootpart} "			\
		"${scripthdraddr} ${prefix}${boot_script_hdr} " \
		"&& esbc_validate ${scripthdraddr};"		\
		"source ${scriptaddr}\0"			\
	"installer=load mmc 0:2 $load_addr "			\
		"/flex_installer_arm64.itb; "			\
		"env exists mcinitcmd && run mcinitcmd && "	\
		"mmc read 0x80001000 0x6800 0x800;"		\
		"fsl_mc lazyapply dpl 0x80001000;"			\
		"bootm $load_addr#ls1088ardb\0"			\
	"qspi_bootcmd=echo Trying load from qspi..;"		\
		"sf probe && sf read $load_addr "		\
		"$kernel_addr $kernel_size ; env exists secureboot "	\
		"&& sf read $kernelheader_addr_r $kernelheader_addr "	\
		"$kernelheader_size && esbc_validate ${kernelheader_addr_r}; "\
		"bootm $load_addr#ls1088ardb\0"			\
		"sd_bootcmd=echo Trying load from sd card..;"		\
		"mmcinfo; mmc read $load_addr "			\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#ls1088ardb\0"

#endif /* __LS1088A_RDB_H */
