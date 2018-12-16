/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 */

#include <common.h>
#include <image.h>
#include <mapmem.h>
#include <malloc.h>

#include "include/phy_gen2_fit.h"
#include "include/input.h"
#include "include/io.h"

static int phy_gen2_boot_device(void)
{
#ifdef CONFIG_SPL_MMC_SUPPORT
	return PHY_GEN2_SRC_MMC;
#endif
#ifdef CONFIG_SPL_NAND_SUPPORT
	return PHY_GEN2_SRC_NAND;
#endif
	return PHY_GEN2_SRC_XIP;
}

static struct phy_gen2_image_loader *phy_gen2_ll_find_loader(
		unsigned int dev_id)
{
	struct phy_gen2_image_loader *drv =
		ll_entry_start(struct phy_gen2_image_loader, phy_gen2_image_loader);
	const int n_ents =
		ll_entry_count(struct phy_gen2_image_loader, phy_gen2_image_loader);
	struct phy_gen2_image_loader *entry;

	for (entry = drv; entry != drv + n_ents; entry++) {
		if (dev_id == entry->id)
			return entry;
	}

	/* Not found */
	return NULL;
}

static void *phy_gen2_phy_prep_img(const unsigned int ctrl_num,
			       struct phy_gen2_image_loader *dev, ulong addr)
{
	int ret;
	void *blob;
	ulong fit_size;
	image_header_t hdr;

	if (!dev->memmove) {
		pr_err("PHY_GEN2 FW: memmove() not defined for %s\n", dev->name);
		return NULL;
	}

	/* copy FIT header and verify */
	ret = dev->memmove((void *)&hdr, addr, sizeof(hdr));
	if (genimg_get_format(&hdr) != IMAGE_FORMAT_FIT) {
		pr_err("PHY_GEN2 FW: image at 0x%lx not FIT\n", addr);
		return NULL;
	}

	/*
	 * Now that we've verified that the image is indeed FIT
	 * calculate the total size of FIT blob
	 * NOTE: We are assuming simple-FIT i.e. actual images
	 * are outside of the FIT structure.
	 * This is essential because DDR is not available to us
	 * and OCRAM is limited
	 */
	fit_size = fdt_totalsize(&hdr);
	debug("PHY_GEN2 FW: Total size of FIT structure = 0x%lx\n", fit_size);

	/*
	 * The image might be in a non-directly addressible source,
	 * so move it to OCRAM
	 */
	blob = calloc(1, fit_size);

	if (!blob) {
		pr_err("PHY_GEN2 FW: unable allocate memory from OCRAM\n");
		return NULL;
	}
	ret = dev->memmove(blob, addr, fit_size);

	if (ret)
		return NULL;
	return blob;
}

static unsigned int phy_gen2_phy_image_load(const unsigned int ctrl_num,
					struct phy_gen2_fw_info *fw, int train2d,
					void *msg, size_t len)
{
	void *fit;
	ulong fit_size;
	int images, ret, noffset, offset;
	char imem_fit_uname[20];
	char dmem_fit_uname[20];

	/*
	 * void *fit points to a FIT blob that constains
	 * PHY images for all the supported configurations,
	 *
	 * NOTE: This code assumes simple FIT image i.e.,
	 * the actual image data is outside the FIT blob
	 *
	 *      +-------------------------------+
	 *      |       FIT MAGIC               |
	 *      |       addr of /images node    |
	 *      |       ....                    |
	 *      +-------------------------------+
	 *      | /images :                     |
	 *      |       image0                  |
	 *      |         data-offset           |
	 *      |         data-size             |
	 *      |                               |
	 *      |       image1                  |
	 *      |         data-offset           |
	 *      |         data-size             |
	 *      |                               |
	 *      |       image2                  |
	 *      |         data-offset           |
	 *      |         data-size             |
	 *      |       .....                   |
	 *      |       .....                   |
	 *      +-------------------------------+
	 *      |image0.bin                     |
	 *      +-------------------------------+
	 *      |image1.bin                     |
	 *      +-------------------------------+
	 *      |image2.bin                     |
	 *      +-------------------------------+
	 *      .....
	 *      .....
	 */

	fit = fw->hdr;

	if (train2d == 0)
		phy_io_write16(ctrl_num, t_master | csr_mem_reset_l_addr,
			       csr_protect_mem_reset_mask);


	/* Initilaizing firmware mailbox */
	phy_io_write16(ctrl_num, t_apbonly | csr_dct_write_prot, 0x1);
	phy_io_write16(ctrl_num, t_drtub | csr_uct_write_prot, 0x1);

	switch (fw->input->basic.dimm_type) {
	case UDIMM:
	case SODIMM:
	case NODIMM:
		sprintf(imem_fit_uname, "imem_udimm_%dd", train2d + 1);
		sprintf(dmem_fit_uname, "dmem_udimm_%dd", train2d + 1);
		break;
	case RDIMM:
		sprintf(imem_fit_uname, "imem_rdimm_%dd", train2d + 1);
		sprintf(dmem_fit_uname, "dmem_rdimm_%dd", train2d + 1);
		break;
	case LRDIMM:
		sprintf(imem_fit_uname, "imem_lrdimm_%dd", train2d + 1);
		sprintf(dmem_fit_uname, "dmem_lrdimm_%dd", train2d + 1);
		break;
	default:
		pr_err("Unsupported DIMM type\n");
		return -EINVAL;
	}
	printf("PHY_GEN2 FW: selected configuration: %s, %s\n",
	       imem_fit_uname, dmem_fit_uname);

	/*
	 * since actual image data is outside FIT blob,
	 * find the size of said blob
	 */
	fit_size = fdt_totalsize(fit);
	fit_size = (fit_size + 3) & ~3;
	debug("PHY_GEN2 FW: size of FIT blob = 0x%lx\n", fit_size);

	/* find the /images node */
	images = fdt_path_offset(fit, FIT_IMAGES_PATH);
	if (images < 0) {
		pr_err("PHY_GEN2 phy FW: /images not found\n");
		return -EINVAL;
	}
	debug("PHY_GEN2 FW: images found at: 0x%x\n", images);

	fw->imem = malloc(sizeof(image_info_t));
	fw->dmem = malloc(sizeof(image_info_t));

	if (!fw->imem || !fw->dmem) {
		pr_err("PHY_GEN2 FW: unable allocate memory from OCRAM\n");
		return -ENOMEM;
	}

	/* find imem_* subnode */
	noffset = fdt_subnode_offset(fit, images, imem_fit_uname);
	if (noffset < 0) {
		pr_err("PHY_GEN2 phy FW: imem not found\n");
		return -EINVAL;
	}
	debug("PHY_GEN2 FW: imem noffset = 0x%x\n", noffset);

	/* find offset of imem outside FIT blob */
	fw->imem->start = CONFIG_FSL_PHY_GEN2_DDR_IMG_ADDR;
	fit_image_get_data_offset(fit, noffset, &offset);
	fw->imem->image_start = fit_size + offset;
	fit_image_get_data_size(fit, noffset, &offset);
	fw->imem->image_len = offset;
	fw->imem->load = CONFIG_FSL_PHY_GEN2_DDR_PHY_IMEM_ADDR;
	debug("PHY_GEN2 FW: data-offset = 0x%lx, size = 0x%lx\n",
	      fw->imem->image_start, fw->imem->image_len);

	/* find dmem_* subnode */
	noffset = fdt_subnode_offset(fit, images, dmem_fit_uname);
	if (noffset < 0) {
		pr_err("PHY_GEN2 phy FW: dmem not found\n");
		return -EINVAL;
	}
	debug("PHY_GEN2 FW: dmem noffset = 0x%x\n", noffset);

	/* find offset of dmem outside FIT blob */
	fw->dmem->start = CONFIG_FSL_PHY_GEN2_DDR_IMG_ADDR;
	fit_image_get_data_offset(fit, noffset, &offset);
	fw->dmem->image_start = fit_size + offset;
	fit_image_get_data_size(fit, noffset, &offset);
	fw->dmem->image_len = offset;
	fw->dmem->load = CONFIG_FSL_PHY_GEN2_DDR_PHY_DMEM_ADDR;
	debug("PHY_GEN2 FW: data-offset = 0x%lx, size = 0x%lx\n",
	      fw->dmem->image_start, fw->dmem->image_len);

	ret = fw->dev->load_image(ctrl_num, fw->imem, NULL, 0);
	if (!ret)
		ret = fw->dev->load_image(ctrl_num, fw->dmem, msg, len);

	return ret;
}

unsigned int phy_gen2_dimm_train(const unsigned int ctrl_num, struct input *input,
		int train2d, void *msg, size_t len)
{
	int ret;
	void *blob;
	struct phy_gen2_fw_info *info;
	struct phy_gen2_image_loader *dev;

	info = calloc(1, sizeof(struct phy_gen2_fw_info));
	if (!info) {
		printf("failed to allocate memory\n");
		goto dimm_train_failed;
	}

	/* Find boot device, hang if fail*/
	dev = phy_gen2_ll_find_loader(phy_gen2_boot_device());
	if (!dev)
		goto dimm_train_failed;
	else
		info->dev = dev;
	debug("PHY_GEN2 FW: device %s found\n", dev->name);


	/*
	 * The image might be in a non-directly addressible source,
	 * so find it's size and move it to OCRAM
	 */
	blob = phy_gen2_phy_prep_img(ctrl_num, dev,
			CONFIG_FSL_PHY_GEN2_DDR_IMG_ADDR);
	if (!blob)
		goto dimm_train_failed;

	info->hdr = blob;
	info->input = input;

	/*
	 * Now that we have simple-FIT structure in OCRAM
	 * parse and load
	 */
	ret = phy_gen2_phy_image_load(ctrl_num, info, train2d, msg, len);
	if (ret)
		goto dimm_train_failed;

	free(info);
	free(blob);

	return 0;

dimm_train_failed:
	hang();
}
