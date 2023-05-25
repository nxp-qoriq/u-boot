/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020-2023 NXP
 */

#ifndef __LX2160_H
#define __LX2160_H

#if CONFIG_IS_ENABLED(TARGET_LX2160ARDB)
u8 get_board_rev(void);
int fdt_fixup_board_phy_revc(void *fdt);
#endif

#endif /* __LX2160_H */
