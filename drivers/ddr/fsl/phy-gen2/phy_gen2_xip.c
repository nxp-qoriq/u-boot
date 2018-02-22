/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+        BSD-3-Clause
 */

#include <common.h>
#include "include/phy_gen2_fit.h"
#include "include/io.h"

static int xip_memmove(void *dst, ulong src, int len)
{
	memcpy(dst, (void *)src, len);
	return 0;
}

static int xip_load_image(const unsigned int ctrl_num, image_info_t *img)
{
	int i;
	u16 *src;
	u32 dst;

	src = (u16 *)(uintptr_t)(img->image_start + img->start);
	dst = img->load;

	printf("PHY_GEN2 FW: Load firmware from: %p to 0x%x (len = 0x%lx)..", src,
	       dst, img->image_len);

	for (i = 0; i < (img->image_len / 2); i++)
		phy_io_write16(ctrl_num, dst + i, *(src + i));

	printf("..Done\n");

	return 0;
}

PHY_GEN2_LOAD_IMAGE_METHOD("XIP", 0, PHY_GEN2_SRC_XIP, xip_memmove,
		       xip_load_image);
