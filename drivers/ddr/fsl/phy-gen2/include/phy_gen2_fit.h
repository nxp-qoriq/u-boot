/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 */

#ifndef __DDR_PHY_GEN2_PHY__
#define __DDR_PHY_GEN2_PHY__

#include <image.h>
#include "input.h"

#define pr_err(fmt, args...) printf("(%s %d)" fmt, __func__, __LINE__, ##args)

struct phy_gen2_fw_info {
	/* boot device */
	struct phy_gen2_image_loader *dev;

	image_info_t *imem;
	image_info_t *dmem;

	/* relevant SPD params */
	struct input *input;

	/* FIT structure related info */
	void	*hdr;
};

enum phy_gen2_src_ids {
	PHY_GEN2_SRC_XIP = 0,
	PHY_GEN2_SRC_MMC,
	PHY_GEN2_SRC_NAND
};

struct phy_gen2_image_loader {
	const char *name;
	unsigned int id;
	/*
	 * dst: this must always point to a directly addressable location
	 *	e.g. OCRAM
	 * src: this is left vague to accomodate multiple devices
	 *	e.g. in XIP code *src will be flash address,
	 *	whereas in MMC/NAND src means block number.
	 * len: number of bytes to read.
	 */
	int (*memmove)(void *dst, ulong src, int len);
	int (*load_image)(const unsigned int ctrl_num, image_info_t *);
};

#define PHY_GEN2_LOAD_IMAGE(__name)                                  \
	ll_entry_declare(struct phy_gen2_image_loader, __name, phy_gen2_image_loader)

#define PHY_GEN2_LOAD_IMAGE_METHOD(_name, _priority, _id, _memmove, _load) \
	PHY_GEN2_LOAD_IMAGE(_memmove ## _priority ## _id) = { \
		.name = _name, \
		.id = _id, \
		.memmove = _memmove, \
		.load_image = _load, \
	}
unsigned int phy_gen2_dimm_train(const unsigned int ctrl_num, struct input *input,
		int train2d);
#endif
