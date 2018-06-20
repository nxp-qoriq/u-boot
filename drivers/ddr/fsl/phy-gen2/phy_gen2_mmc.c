
/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+        BSD-3-Clause
 */

#include <common.h>
#include "include/phy_gen2_fit.h"
#include "include/io.h"
#include "mmc.h"
#include <malloc.h>

static inline int mmc_blk_read(struct mmc *mmc, u32 blk, void *fw_image)
{
	int ret;

	ret = mmc->block_dev.block_read(&mmc->block_dev, blk, 1, fw_image);
	if (ret != 1) {
		printf("MMC/SD read of DDR-PHY at blk number 0x%x fail\n",
		       blk);
		return -EIO;
	}
	return 0;
}

static int mmc_memmove(void *dst, ulong src, int len)
{
	struct mmc *mmc;
	void *fitp;
	int dev = CONFIG_SYS_MMC_ENV_DEV;
	u32 i, blk, cnt, j;
	u32 fw_offset, loop_count = 0;

	mmc = find_mmc_device(dev);
	fitp = malloc(mmc->read_bl_len);
	if (!fitp) {
		printf("DDR-PHY: malloc failed (size 0x%x)\n",
		       mmc->read_bl_len);
		return -ENOMEM;
	}
	blk = src / mmc->read_bl_len; /* starting block */
	fw_offset = src % mmc->read_bl_len; /* image offset within block */
	cnt = DIV_ROUND_UP(len + fw_offset, mmc->read_bl_len); /* ttl blocks */

	for (i = 0; i < cnt; i++) {
		if (mmc_blk_read(mmc, blk, fitp)) {
			free(fitp);
			return -EIO;
		}

		/* copy the current block data to OCRAM */
		for (j = fw_offset; j < (mmc->read_bl_len) &&
		    (loop_count < len); j++, loop_count++)
			*((u8 *)dst + loop_count) = *((u8 *)fitp + j);

		fw_offset = 0;
		++blk;
	}
	free(fitp);
	return 0;
}

static int __mmc_load_image(const unsigned int ctrl_num, image_info_t *img,
		   struct mmc *mmc, u32 dst, u16 *src, void *fw_image,
		   u32 update_len)
{
	u32 i, j, loop_count = update_len;
	u32 blk, cnt, fw_offset;

	src = src + update_len;
	blk = ((ulong)src) / mmc->read_bl_len;
	fw_offset = ((ulong)src) % mmc->read_bl_len;
	cnt = DIV_ROUND_UP(img->image_len + fw_offset, mmc->read_bl_len);

	for (j = 0; j < cnt; j++) {
		if (mmc_blk_read(mmc, blk, fw_image))
			return -EIO;
		src = (u16 *)((u8 *)fw_image + fw_offset);

		for (i = 0; (i < ((mmc->read_bl_len - fw_offset) / 2)) &&
		    (loop_count < (img->image_len / 2)); i++, loop_count++)
			phy_io_write16(ctrl_num, dst + loop_count, *(src + i));
		fw_offset = 0;
		++blk;
	}
	return 0;
}

static int mmc_load_image(const unsigned int ctrl_num, image_info_t *img,
				uint16_t *msg, size_t len)
{
	u32 i;
	u32 dst;
	u16 *src;
	struct mmc *mmc;
	void *fw_image;
	int dev = CONFIG_SYS_MMC_ENV_DEV;

	src = (u16 *)(uintptr_t)(img->image_start + img->start);
	dst = img->load;

	mmc = find_mmc_device(dev);
	fw_image = malloc(mmc->read_bl_len);
	if (!fw_image) {
		printf("DDR-PHY: malloc failed (size 0x%x)\n",
		       mmc->read_bl_len);
		return -ENOMEM;
	}
	debug("PHY_GEN2 FW: Load firmware from: %p to 0x%x (len = 0x%lx)..", src,
	      dst, img->image_len);

	if (msg == NULL) { /* write IMEM image */
		if (__mmc_load_image(ctrl_num, img, mmc, dst, src,
				     fw_image, 0)) {
			free(fw_image);
			return -EIO;
		}
	} else { /* write DMEM image */
		/* write the message block */
		for (i = 0; i < (len / 2); i++)
			phy_io_write16(ctrl_num, dst + i, *msg++);
		if (__mmc_load_image(ctrl_num, img, mmc, dst, src,
				     fw_image, i)) {
			free(fw_image);
			return -EIO;
		}
	}

	debug("..Done\n");
	free(fw_image);
	return 0;
}

PHY_GEN2_LOAD_IMAGE_METHOD("MMC", 0, PHY_GEN2_SRC_MMC, mmc_memmove,
		       mmc_load_image);
