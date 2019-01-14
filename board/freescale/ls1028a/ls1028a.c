/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <fsl_ddr.h>
#include <asm/io.h>
#include <hwconfig.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
#include <environment.h>
#include <asm/arch-fsl-layerscape/soc.h>
#include <i2c.h>
#include <asm/arch/soc.h>
#ifdef CONFIG_FSL_LS_PPA
#include <asm/arch/ppa.h>
#endif
#include <fsl_immap.h>
#include <netdev.h>

#include <fdtdec.h>
#include <miiphy.h>
#include "../common/qixis.h"

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif

#ifndef CONFIG_SYS_EARLY_PCI_INIT
	/* run PCI init to kick off ENETC */
	pci_init();
#endif

	return 0;
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}

int board_early_init_f(void)
{

#ifdef CONFIG_SYS_I2C_EARLY_INIT
	i2c_early_init_f();
#endif
	fsl_lsch3_early_init_f();
	return 0;
}

void detail_board_ddr_info(void)
{
	puts("\nDDR    ");
	print_size(gd->bd->bi_dram[0].size + gd->bd->bi_dram[1].size, "");
	print_ddr_info(0);
}

#ifdef CONFIG_OF_BOARD_SETUP
#ifdef CONFIG_FSL_ENETC
extern void enetc_setup(void *blob);
#endif
int ft_board_setup(void *blob, bd_t *bd)
{
	u64 base[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	ft_cpu_setup(blob, bd);

	/* fixup DT for the two GPP DDR banks */
	base[0] = gd->bd->bi_dram[0].start;
	size[0] = gd->bd->bi_dram[0].size;
	base[1] = gd->bd->bi_dram[1].start;
	size[1] = gd->bd->bi_dram[1].size;

#ifdef CONFIG_RESV_RAM
	/* reduce size if reserved memory is within this bank */
	if (gd->arch.resv_ram >= base[0] &&
	    gd->arch.resv_ram < base[0] + size[0])
		size[0] = gd->arch.resv_ram - base[0];
	else if (gd->arch.resv_ram >= base[1] &&
		 gd->arch.resv_ram < base[1] + size[1])
		size[1] = gd->arch.resv_ram - base[1];
#endif

	fdt_fixup_memory_banks(blob, base, size, 2);

#ifdef CONFIG_FSL_ENETC
	enetc_setup(blob);
#endif
	return 0;
}
#endif

#ifdef CONFIG_FSL_ENETC

#define MUX_INF(fmt, args...)	do {} while (0)
#define MUX_DBG(fmt, args...)	printf("MDIO MUX: " fmt, ##args)
#define MUX_ERR(fmt, args...)	printf("MDIO MUX: " fmt, ##args)
struct mdio_qixis_mux {
	struct mii_dev *bus;
	int select;
	int mask;
	int mdio_node;
};

void *mdio_mux_get_parent(struct mii_dev *bus)
{
	struct mdio_qixis_mux *priv = bus->priv;
	const char *mdio_name;
	struct mii_dev *parent_bus;

	mdio_name = fdt_get_name(gd->fdt_blob, priv->mdio_node, NULL);
	parent_bus = miiphy_get_dev_by_name(mdio_name);
	if (!parent_bus)
		MUX_ERR("couldn't find MDIO bus %s\n", mdio_name);
	MUX_INF("defer to parent bus %s\n", mdio_name);
	return parent_bus;
}

void mux_group_select(struct mdio_qixis_mux *priv)
{
	u8 brdcfg4, reg;
	static u8 oldsel;

	brdcfg4 = QIXIS_READ(brdcfg[4]);
	reg = brdcfg4;

	reg = (reg & ~priv->mask) | priv->select;

	if (!(brdcfg4 ^ reg))
		return;

	MUX_DBG(" qixis_write %02x\n", reg);
	QIXIS_WRITE(brdcfg[4], brdcfg4);
}

int mdio_mux_write(struct mii_dev *bus, int port_addr, int dev_addr,
		   int regnum, u16 value)
{
	struct mii_dev *parent_bus;

	parent_bus = mdio_mux_get_parent(bus);
	if (!parent_bus)
		return -ENODEV;

	mux_group_select(bus->priv);

	MUX_INF("write to parent MDIO\n");
	return parent_bus->write(parent_bus, port_addr, dev_addr, regnum, value);
}

int mdio_mux_read(struct mii_dev *bus, int port_addr, int dev_addr,
		  int regnum)
{
	struct mii_dev *parent_bus;

	parent_bus = mdio_mux_get_parent(bus);
	if (!parent_bus)
		return -ENODEV;

	mux_group_select(bus->priv);

	MUX_INF("read from parent MDIO\n");
	return parent_bus->read(parent_bus, port_addr, dev_addr, regnum);
}

int mdio_mux_reset(struct mii_dev *bus)
{
	return 0;
}

static void setup_mdio_mux_group(int offset, int mdio_node, int mask)
{
	const void *fdt = gd->fdt_blob;
	struct mdio_qixis_mux *group;
	int select;

	char path[32];

	fdt_get_path(fdt, offset, path, 32);
	MUX_INF("reading node %s\n", path);

	select = fdtdec_get_int(fdt, offset, "reg", -1);
	if (select < 0) {
		MUX_ERR("invalid selection word");
		return;
	}

	group = malloc(sizeof(struct mdio_qixis_mux));
	group->select = select;
	group->mask = mask;
	group->mdio_node = mdio_node;
	group->bus = mdio_alloc();
	if (!group->bus) {
		MUX_ERR("failed to allocate mdio bus\n");
		goto err;
	}
	group->bus->read = mdio_mux_read;
	group->bus->write = mdio_mux_write;
	group->bus->reset = mdio_mux_reset;
	strcpy(group->bus->name, fdt_get_name(fdt, offset, NULL));
	group->bus->priv = group;
	MUX_DBG("register MUX ""%s"" sel=%x, mask=%x\n", group->bus->name, group->select, group->mask);
	mdio_register(group->bus);
	return;

err:
	free(group);
}

void setup_mdio_mux(void)
{
	const void *fdt = gd->fdt_blob;
	int offset, mux_node, mdio_node, mask;

	mux_node = fdt_path_offset(fdt, "/qixis/mdio-mux@54");
	if (mux_node < 0) {
		MUX_ERR("no MDIO MUX node\n");
		return;
	}

	mask = fdtdec_get_int(fdt, mux_node, "mux-mask", -1);
	if (mask < 0)
		MUX_ERR("invalid mux-mask\n");
	mdio_node = fdtdec_lookup_phandle(fdt, mux_node, "mdio-parent-bus");

	/* go through MUX nodes, register MDIO buses */
	for (offset = fdt_first_subnode(fdt, mux_node);
	     offset >= 0;
	     offset = fdt_next_subnode(fdt, offset)) {
		setup_mdio_mux_group(offset, mdio_node, mask);
	};
}
#endif

#ifdef CONFIG_LAST_STAGE_INIT
int last_stage_init(void)
{
#ifdef CONFIG_FSL_ENETC
	setup_mdio_mux();
#endif
	return 0;
}
#endif

#ifdef CONFIG_FSL_QIXIS
int checkboard(void)
{
	u8 sw;

	int clock;
	char *board;
	char buf[64] = {0};
	static const char *freq[6] = {"100.00", "125.00", "156.25",
					"161.13", "322.26", "100.00 SS"};

	cpu_name(buf);
	/* find the board details */
	sw = QIXIS_READ(id);

	switch (sw) {
	case 0x46:
		board = "QDS";
		break;
	case 0x47:
		board = "RDB";
		break;
	case 0x49:
		board = "HSSI";
		break;
	default:
		board = "unknown";
		break;
	}

	sw = QIXIS_READ(arch);
	printf("Board: %s-%s, Version: %c, boot from ",
	       buf, board, (sw & 0xf) + 'A' - 1);

	sw = QIXIS_READ(brdcfg[0]);
	sw = (sw & QIXIS_LBMAP_MASK) >> QIXIS_LBMAP_SHIFT;

#ifdef CONFIG_SD_BOOT
	puts("SD \n");
#elif defined(CONFIG_EMMC_BOOT)
	puts("eMMC \n");
#else
	switch (sw) {
	case 0:
	case 4:
		printf("NOR\n");
		break;
	case 1:
		printf("NAND\n");
		break;
	default:
		printf("invalid setting of SW%u\n", QIXIS_LBMAP_SWITCH);
		break;
	}
#endif

	printf("FPGA: v%d (%s: %s_%s)\n", QIXIS_READ(scver),
	       !qixis_read_released()  ? "INTERIM" : "RELEASED",
			board, qixis_read_date(buf));

	puts("SERDES1 Reference : ");
	sw = QIXIS_READ(brdcfg[2]);
#ifdef CONFIG_TARGET_LS1028ARDB
	clock = (sw >> 6) & 3;
#else
	clock = (sw >> 4) & 0xf;
#endif

	printf("Clock1 = %sMHz ", freq[clock]);
#ifdef CONFIG_TARGET_LS1028ARDB
	clock = (sw >> 4) & 3;
#else
	clock = sw & 0xf;
#endif
	printf("Clock2 = %sMHz\n", freq[clock]);

	return 0;
}
#endif

#ifdef CONFIG_EMMC_BOOT
void *esdhc_get_base_addr(void)
{
	return (void *)CONFIG_SYS_FSL_ESDHC1_ADDR;
}

#endif
