/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LX2_COMMON_H
#define __LX2_COMMON_H

#include <asm/arch/stream_id_lsch3.h>
#include <asm/arch/config.h>
#include <asm/arch/soc.h>

#define CONFIG_REMAKE_ELF
#define CONFIG_FSL_LAYERSCAPE
#define CONFIG_MP
#define CONFIG_GICV3
#define CONFIG_FSL_TZPC_BP147
#define CONFIG_FSL_MEMAC

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_FSL_OCRAM_BASE + 0xfff0)
#define CONFIG_SYS_FLASH_BASE		0x20000000

#define CONFIG_SYS_TEXT_BASE		0x20100000
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_SUPPORT_RAW_INITRD

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F	1

/* DDR */
#define CONFIG_FSL_DDR_INTERACTIVE	/* Interactive debugging */
#define CONFIG_SYS_FSL_DDR_INTLV_256B	/* force 256 byte interleaving */

#define CONFIG_VERY_BIG_RAM
#define CONFIG_SYS_DDR_SDRAM_BASE		0x80000000UL
#define CONFIG_SYS_FSL_DDR_SDRAM_BASE_PHY	0
#define CONFIG_SYS_DDR_BLOCK2_BASE		0x2080000000ULL
#define CONFIG_SYS_FSL_DDR_MAIN_NUM_CTRLS	2

#define CONFIG_DDR_SPD
#define CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE
#define CONFIG_MEM_INIT_VALUE		0xdeadbeef
#define SPD_EEPROM_ADDRESS1		0x51
#define SPD_EEPROM_ADDRESS2		0x52
#define SPD_EEPROM_ADDRESS3		0x53
#define SPD_EEPROM_ADDRESS4		0x54
#define SPD_EEPROM_ADDRESS5		0x55
#define SPD_EEPROM_ADDRESS6		0x56
#define SPD_EEPROM_ADDRESS		SPD_EEPROM_ADDRESS1
#define CONFIG_SYS_SPD_BUS_NUM		0	/* SPD on I2C bus 0 */
#define CONFIG_DIMM_SLOTS_PER_CTLR	2
#define CONFIG_CHIP_SELECTS_PER_CTRL	4
#define CONFIG_FSL_DDR_BIST	/* enable built-in memory test */

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		3

/* Miscellaneous configurable options */
#define CONFIG_SYS_LOAD_ADDR	(CONFIG_SYS_DDR_SDRAM_BASE + 0x10000000)

/* SMP Definitinos  */
#define CPU_RELEASE_ADDR		secondary_boot_func

/* Generic Timer Definitions */
/*
 * This is not an accurate number. It is used in start.S. The frequency
 * will be udpated later when get_bus_freq(0) is available.
 */

#define COUNTER_FREQUENCY		25000000	/* 25MHz */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 2048 * 1024)

/* Serial Port */
#define CONFIG_CONS_INDEX		0
#define CONFIG_PL01X_SERIAL
#define CONFIG_PL011_CLOCK		(get_bus_freq(0) / 4)
#define CONFIG_SYS_SERIAL0		0x21c0000
#define CONFIG_SYS_SERIAL1		0x21d0000
#define CONFIG_SYS_SERIAL2		0x21e0000
#define CONFIG_SYS_SERIAL3		0x21f0000
#define CONFIG_PL01x_PORTS		{(void *)CONFIG_SYS_SERIAL0, \
					(void *)CONFIG_SYS_SERIAL1, \
					(void *)CONFIG_SYS_SERIAL2, \
					(void *)CONFIG_SYS_SERIAL3 }
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* MC firmware */
#define CONFIG_SYS_LS_MC_DPC_MAX_LENGTH		0x20000
#define CONFIG_SYS_LS_MC_DRAM_DPC_OFFSET	0x00F00000
#define CONFIG_SYS_LS_MC_DPL_MAX_LENGTH		0x20000
#define CONFIG_SYS_LS_MC_DRAM_DPL_OFFSET	0x00F20000
#define CONFIG_SYS_LS_MC_BOOT_TIMEOUT_MS	5000

/* Define phy_reset function to boot the MC based on mcinitcmd.
 * This happens late enough to properly fixup u-boot env MAC addresses.
 */
#define CONFIG_RESET_PHY_R

/*
 * Carve out a DDR region which will not be used by u-boot/Linux
 *
 * It will be used by MC and Debug Server. The MC region must be
 * 512MB aligned, so the min size to hide is 512MB.
 */
#ifdef CONFIG_FSL_MC_ENET
#define CONFIG_SYS_LS_MC_DRAM_BLOCK_MIN_SIZE	(512UL * 1024 * 1024)
#endif

/* I2C bus multiplexer */
#define I2C_MUX_PCA_ADDR_PRI		0x77 /* Primary Mux*/
#define I2C_MUX_CH_DEFAULT		0x8

/* RTC */
#define RTC
#define CONFIG_RTC_PCF8563		1
#define CONFIG_SYS_I2C_RTC_ADDR		0x51  /* Channel 3*/
#define CONFIG_CMD_DATE

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM		0
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Qixis */
#define CONFIG_FSL_QIXIS
#define CONFIG_QIXIS_I2C_ACCESS
#define CONFIG_SYS_I2C_FPGA_ADDR		0x66

/* PCI */
#ifdef CONFIG_PCI
#define CONFIG_PCI_SCAN_SHOW
#endif

/* MMC */
#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#endif

/* SATA */
/*TODO: Add SATA3, SATA4 related configs*/
#ifdef CONFIG_SCSI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_SYS_SATA1		AHCI_BASE_ADDR1
#define CONFIG_SYS_SATA2		AHCI_BASE_ADDR2

#define CONFIG_SYS_SCSI_MAX_SCSI_ID	1
#define CONFIG_SYS_SCSI_MAX_LUN		1
#define CONFIG_SYS_SCSI_MAX_DEVICE	(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
					CONFIG_SYS_SCSI_MAX_LUN)
#endif

/* USB */
#ifdef CONFIG_USB
#define CONFIG_HAS_FSL_XHCI_USB
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2
#endif

/* FlexSPI */
#ifdef CONFIG_NXP_FSPI
#define NXP_FSPI_FLASH_SIZE		SZ_64M
#define NXP_FSPI_FLASH_NUM		1
#endif

#ifndef __ASSEMBLY__
unsigned long get_board_sys_clk(void);
unsigned long get_board_ddr_clk(void);
#endif

#define CONFIG_SYS_CLK_FREQ		get_board_sys_clk()
#define CONFIG_DDR_CLK_FREQ		get_board_ddr_clk()
#define COUNTER_FREQUENCY_REAL		(CONFIG_SYS_CLK_FREQ / 4)

#define CONFIG_HWCONFIG
#define HWCONFIG_BUFFER_SIZE		128

#define CONFIG_ENV_SIZE			0x2000          /* 8KB */
#define CONFIG_ENV_SECT_SIZE		0x20000
#define CONFIG_ENV_OFFSET		0x300000        /* 3MB */
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + \
					 CONFIG_ENV_OFFSET)

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE /* Boot args buffer */
#define CONFIG_SYS_LONGHELP
#define CONFIG_CMDLINE_EDITING		1
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_MAXARGS		64	/* max command args */

#define CONFIG_SYS_BOOTM_LEN   (64 << 20)      /* Increase max gunzip size */

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
	"mcmemsize=0x40000000\0"		\
	"mcinitcmd=fsl_mc start mc 0x20a00000" \
	" 0x20e00000 \0"

#define CONFIG_BOOTCOMMAND	"fsl_mc apply dpl 0x20d00000;" \
				"bootm 0x81000000"

#define CONFIG_BOOTARGS		"console=ttyAMA0,115200 root=/dev/ram0 " \
				"earlycon=pl011,mmio32,0x21c0000 " \
				"ramdisk_size=0x2000000 default_hugepagesz=2m" \
				" hugepagesz=2m hugepages=256"

#endif /* __LX2_COMMON_H */
