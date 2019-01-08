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

static int xip_load_image(const unsigned int ctrl_num, image_info_t *img,
				uint16_t *msg, size_t len)
{
	int i;
	u16 *src;
	u32 dst;

	src = (u16 *)(uintptr_t)(img->image_start + img->start);
	dst = img->load;

	debug("PHY_GEN2 FW: Load firmware from: %p to 0x%x (len = 0x%lx)..", src,
	       dst, img->image_len);

	if (msg == NULL) { /* write IMEM image */
		for (i = 0; i < (img->image_len / 2); i++)
			phy_io_write16(ctrl_num, dst + i, *(src + i));
	} else { /* write DMEM image */
		/* write the message block */
		for (i = 0; i < (len / 2); i++)
			phy_io_write16(ctrl_num, dst + i, *msg++);

		/* continue to write DMEM image where message block ends */
		for (; i < (img->image_len / 2); i++)
			phy_io_write16(ctrl_num, dst + i, *(src + i));
	}
	debug("..Done\n");

	return 0;
}

PHY_GEN2_LOAD_IMAGE_METHOD("XIP", 0, PHY_GEN2_SRC_XIP, xip_memmove,
		       xip_load_image);
