// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2017-2020 NXP
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Layerscape PCIe driver
 */

#include <common.h>
#include <pci.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/io.h>
#include <errno.h>
#ifdef CONFIG_OF_BOARD_SETUP
#include <linux/libfdt.h>
#include <fdt_support.h>
#ifdef CONFIG_ARM
#include <asm/arch/clock.h>
#endif
#include <asm/arch/soc.h>
#include <malloc.h>
#include "pcie_layerscape.h"
#include "pcie_layerscape_fixup_common.h"

#if defined(CONFIG_FSL_LSCH3) || defined(CONFIG_FSL_LSCH2)
/*
 * Return next available LUT index.
 */
static int ls_pcie_next_lut_index(struct ls_pcie *pcie)
{
	if (pcie->next_lut_index < PCIE_LUT_ENTRY_COUNT)
		return pcie->next_lut_index++;
	else
		return -ENOSPC;  /* LUT is full */
}

static void lut_writel(struct ls_pcie *pcie, unsigned int value,
		       unsigned int offset)
{
	if (pcie->big_endian)
		out_be32(pcie->lut + offset, value);
	else
		out_le32(pcie->lut + offset, value);
}

/*
 * Program a single LUT entry
 */
static void ls_pcie_lut_set_mapping(struct ls_pcie *pcie, int index, u32 devid,
				    u32 streamid)
{
	/* leave mask as all zeroes, want to match all bits */
	lut_writel(pcie, devid << 16, PCIE_LUT_UDR(index));
	lut_writel(pcie, streamid | PCIE_LUT_ENABLE, PCIE_LUT_LDR(index));
}

static int fdt_pcie_get_nodeoffset(void *blob, struct ls_pcie *pcie)
{
	int nodeoffset;
	uint svr;
	char *compat = NULL;

	/* find pci controller node */
	nodeoffset = fdt_node_offset_by_compat_reg(blob, "fsl,ls-pcie",
						   pcie->dbi_res.start);
	if (nodeoffset < 0) {
#ifdef CONFIG_FSL_PCIE_COMPAT /* Compatible with older version of dts node */
		svr = SVR_SOC_VER(get_svr());
		if (svr == SVR_LS2088A || svr == SVR_LS2084A ||
		    svr == SVR_LS2048A || svr == SVR_LS2044A ||
		    svr == SVR_LS2081A || svr == SVR_LS2041A)
			compat = "fsl,ls2088a-pcie";
		else
			compat = CONFIG_FSL_PCIE_COMPAT;
		if (compat)
			nodeoffset = fdt_node_offset_by_compat_reg(blob,
					compat, pcie->dbi_res.start);
#endif
	}

	return nodeoffset;
}

/*
 * An msi-map is a property to be added to the pci controller
 * node.  It is a table, where each entry consists of 4 fields
 * e.g.:
 *
 *      msi-map = <[devid] [phandle-to-msi-ctrl] [stream-id] [count]
 *                 [devid] [phandle-to-msi-ctrl] [stream-id] [count]>;
 */
static void fdt_pcie_set_msi_map_entry_ls(void *blob, struct ls_pcie *pcie,
					  u32 devid, u32 streamid)
{
	u32 *prop;
	u32 phandle;
	int nodeoffset;

	nodeoffset = fdt_pcie_get_nodeoffset(blob, pcie);
	if (nodeoffset < 0)
		return;

	/* get phandle to MSI controller */
	prop = (u32 *)fdt_getprop(blob, nodeoffset, "msi-parent", 0);
	if (prop == NULL) {
		debug("\n%s: ERROR: missing msi-parent: PCIe%d\n",
		      __func__, pcie->idx);
		return;
	}
	phandle = fdt32_to_cpu(*prop);

	/* set one msi-map row */
	fdt_appendprop_u32(blob, nodeoffset, "msi-map", devid);
	fdt_appendprop_u32(blob, nodeoffset, "msi-map", phandle);
	fdt_appendprop_u32(blob, nodeoffset, "msi-map", streamid);
	fdt_appendprop_u32(blob, nodeoffset, "msi-map", 1);
}

/*
 * An iommu-map is a property to be added to the pci controller
 * node.  It is a table, where each entry consists of 4 fields
 * e.g.:
 *
 *      iommu-map = <[devid] [phandle-to-iommu-ctrl] [stream-id] [count]
 *                 [devid] [phandle-to-iommu-ctrl] [stream-id] [count]>;
 */
static void fdt_pcie_set_iommu_map_entry_ls(void *blob, struct ls_pcie *pcie,
					    u32 devid, u32 streamid)
{
	u32 *prop;
	u32 iommu_map[4];
	int nodeoffset;
	int lenp;

	nodeoffset = fdt_pcie_get_nodeoffset(blob, pcie);
	if (nodeoffset < 0)
		return;

	/* get phandle to iommu controller */
	prop = fdt_getprop_w(blob, nodeoffset, "iommu-map", &lenp);
	if (prop == NULL) {
		debug("\n%s: ERROR: missing iommu-map: PCIe%d\n",
		      __func__, pcie->idx);
		return;
	}

	/* set iommu-map row */
	iommu_map[0] = cpu_to_fdt32(devid);
	iommu_map[1] = *++prop;
	iommu_map[2] = cpu_to_fdt32(streamid);
	iommu_map[3] = cpu_to_fdt32(1);

	if (devid == 0) {
		fdt_setprop_inplace(blob, nodeoffset, "iommu-map",
				    iommu_map, 16);
	} else {
		fdt_appendprop(blob, nodeoffset, "iommu-map", iommu_map, 16);
	}
}

static int fdt_fixup_pcie_device_ls(void *blob, pci_dev_t bdf,
				    struct ls_pcie *pcie)
{
	int streamid, index;

	streamid = pcie_next_streamid(pcie->stream_id_cur, pcie->idx);
	if (streamid < 0) {
		printf("ERROR: out of stream ids for BDF %d.%d.%d\n",
		       PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf));
		return -ENOENT;
	}
	pcie->stream_id_cur++;

	index = ls_pcie_next_lut_index(pcie);
	if (index < 0) {
		printf("ERROR: out of LUT indexes for BDF %d.%d.%d\n",
		       PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf));
		return -ENOENT;
	}

	/* map PCI b.d.f to streamID in LUT */
	ls_pcie_lut_set_mapping(pcie, index, bdf >> 8, streamid);
	/* update msi-map in device tree */
	fdt_pcie_set_msi_map_entry_ls(blob, pcie, bdf >> 8, streamid);
	/* update iommu-map in device tree */
	fdt_pcie_set_iommu_map_entry_ls(blob, pcie, bdf >> 8, streamid);

	return 0;
}

#ifdef CONFIG_PCI_IOMMU_EXTRA_MAPPINGS
struct extra_iommu_entry {
	int action;
	pci_dev_t bdf;
	int num_vfs;
};

#define EXTRA_IOMMU_ENTRY_HOTPLUG	1
#define EXTRA_IOMMU_ENTRY_VFS		2

static struct extra_iommu_entry *get_extra_iommu_ents(void *blob,
						      int nodeoffset,
						      phys_addr_t addr,
						      int *cnt)
{
	const char *s, *p, *tok;
	struct extra_iommu_entry *entries;
	int i = 0, b, d, f;

	s = env_get("pci_iommu_extra");
	if (!s) {
		s = fdt_getprop(blob, nodeoffset, "pci-iommu-extra", NULL);
	} else {
		phys_addr_t pci_base;
		char *endp;

		tok = s;
		p = strchrnul(s + 1, ',');
		s = NULL;
		do {
			if (!strncmp(tok, "pci", 3)) {
				pci_base = simple_strtoul(tok  + 4, &endp, 0);
				if (pci_base == addr) {
					s = endp + 1;
					break;
				}
			}
			p = strchrnul(p + 1, ',');
			tok = p + 1;
		} while (*p);
	}

	if (!s)
		return NULL;

	*cnt = 0;
	p = s;
	while (*p && strncmp(p, "pci", 3)) {
		if (*p == ',')
			(*cnt)++;
		p++;
	}
	if (!(*p))
		(*cnt)++;

	if (!(*cnt) || (*cnt) % 2) {
		printf("ERROR: invalid or odd extra iommu token count %d\n",
		       *cnt);
		return NULL;
	}
	*cnt = (*cnt) / 2;

	entries = malloc((*cnt) * sizeof(*entries));
	if (!entries) {
		printf("ERROR: fail to allocate extra iommu entries\n");
		return NULL;
	}

	p = s;
	while (p) {
		b = simple_strtoul(p, (char **)&p, 0); p++;
		d = simple_strtoul(p, (char **)&p, 0); p++;
		f = simple_strtoul(p, (char **)&p, 0); p++;
		entries[i].bdf = PCI_BDF(b, d, f);

		if (!strncmp(p, "hp", 2)) {
			entries[i].action = EXTRA_IOMMU_ENTRY_HOTPLUG;
			p += 3;
		} else if (!strncmp(p, "vfs", 3)) {
			entries[i].action = EXTRA_IOMMU_ENTRY_VFS;

			p = strchr(p, '=');
			entries[i].num_vfs = simple_strtoul(p + 1, (char **)&p,
							    0);
			if (*p)
				p++;
		} else {
			printf("ERROR: invalid action in extra iommu entry\n");
			free(entries);

			return NULL;
		}

		if (!(*p) || !strncmp(p, "pci", 3))
			break;

		i++;
	}

	return entries;
}
#endif /* CONFIG_PCI_IOMMU_EXTRA_MAPPINGS */

static void fdt_fixup_pcie_ls(void *blob)
{
	struct udevice *dev, *bus;
	struct ls_pcie *pcie;
	pci_dev_t bdf;
#ifdef CONFIG_PCI_IOMMU_EXTRA_MAPPINGS
	struct extra_iommu_entry *entries;
	unsigned short vf_offset, vf_stride;
	int i, j, cnt, sriov_pos, nodeoffset;
#endif

	/* Scan all known buses */
	for (pci_find_first_device(&dev);
	     dev;
	     pci_find_next_device(&dev)) {
		for (bus = dev; device_is_on_pci_bus(bus);)
			bus = bus->parent;
		pcie = dev_get_priv(bus);


		/* the DT fixup must be relative to the hose first_busno */
		bdf = dm_pci_get_bdf(dev) - PCI_BDF(bus->seq, 0, 0);

		if (fdt_fixup_pcie_device_ls(blob, bdf, pcie) < 0)
			break;
	}

#ifdef CONFIG_PCI_IOMMU_EXTRA_MAPPINGS
	list_for_each_entry(pcie, &ls_pcie_list, list) {
		nodeoffset = fdt_pcie_get_nodeoffset(blob, pcie);
		if (nodeoffset < 0) {
			printf("ERROR: couldn't find pci node\n");
			continue;
		}

		entries = get_extra_iommu_ents(blob, nodeoffset,
					       pcie->dbi_res.start, &cnt);
		if (!entries)
			continue;

		for (i = 0; i < cnt; i++) {
			if (entries[i].action == EXTRA_IOMMU_ENTRY_HOTPLUG) {
				bdf = entries[i].bdf -
					PCI_BDF(pcie->bus->seq + 1, 0, 0);
				printf("Added iommu map for hotplug %d.%d.%d\n",
				       PCI_BUS(entries[i].bdf),
				       PCI_DEV(entries[i].bdf),
				       PCI_FUNC(entries[i].bdf));
				if (fdt_fixup_pcie_device_ls(blob,
							     bdf, pcie) < 0) {
					free(entries);
					return;
				}
				continue;
			}

			/* EXTRA_IOMMU_ENTRY_VFS case */
			if (dm_pci_bus_find_bdf(entries[i].bdf, &dev)) {
				printf("ERROR: BDF %d.%d.%d not found\n",
				       PCI_BUS(entries[i].bdf),
				       PCI_DEV(entries[i].bdf),
				       PCI_FUNC(entries[i].bdf));
				continue;
			}
			sriov_pos = dm_pci_find_ext_capability
						(dev, PCI_EXT_CAP_ID_SRIOV);
			if (!sriov_pos) {
				printf("WARN: setting VFs on non-SRIOV dev\n");
				continue;
			}
			dm_pci_read_config16(dev, sriov_pos + 0x14,
					     &vf_offset);
			dm_pci_read_config16(dev, sriov_pos + 0x16,
					     &vf_stride);

			bdf = entries[i].bdf -
				PCI_BDF(pcie->bus->seq + 1, 0, 0) +
				(vf_offset << 8);
			printf("Added %d iommu VF mappings for PF %d.%d.%d\n",
			       entries[i].num_vfs, PCI_BUS(entries[i].bdf),
			       PCI_DEV(entries[i].bdf),
			       PCI_FUNC(entries[i].bdf));
			for (j = 0; j < entries[i].num_vfs; j++) {
				if (fdt_fixup_pcie_device_ls(blob,
							     bdf, pcie) < 0) {
					free(entries);
					return;
				}
				bdf += vf_stride << 8;
			}
		}
		free(entries);
	}
#endif /* CONFIG_PCI_IOMMU_EXTRA_MAPPINGS */

	pcie_board_fix_fdt(blob);
}
#endif

static void ft_pcie_rc_fix(void *blob, struct ls_pcie *pcie)
{
	int off;

	off = fdt_pcie_get_nodeoffset(blob, pcie);
	if (off < 0)
		return;

	if (pcie->enabled && pcie->mode == PCI_HEADER_TYPE_BRIDGE)
		fdt_set_node_status(blob, off, FDT_STATUS_OKAY, 0);
	else
		fdt_set_node_status(blob, off, FDT_STATUS_DISABLED, 0);
}

static void ft_pcie_ep_fix(void *blob, struct ls_pcie *pcie)
{
	int off;

	off = fdt_node_offset_by_compat_reg(blob, CONFIG_FSL_PCIE_EP_COMPAT,
					    pcie->dbi_res.start);
	if (off < 0)
		return;

	if (pcie->enabled && pcie->mode == PCI_HEADER_TYPE_NORMAL)
		fdt_set_node_status(blob, off, FDT_STATUS_OKAY, 0);
	else
		fdt_set_node_status(blob, off, FDT_STATUS_DISABLED, 0);
}

static void ft_pcie_ls_setup(void *blob, struct ls_pcie *pcie)
{
	ft_pcie_ep_fix(blob, pcie);
	ft_pcie_rc_fix(blob, pcie);
}

/* Fixup Kernel DT for PCIe */
void ft_pci_setup_ls(void *blob, bd_t *bd)
{
	struct ls_pcie *pcie;

	list_for_each_entry(pcie, &ls_pcie_list, list)
		ft_pcie_ls_setup(blob, pcie);

#if defined(CONFIG_FSL_LSCH3) || defined(CONFIG_FSL_LSCH2)
	fdt_fixup_pcie_ls(blob);
#endif
}

#else /* !CONFIG_OF_BOARD_SETUP */
void ft_pci_setup_ls(void *blob, bd_t *bd)
{
}
#endif
