// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2021 NXP
 *
 */

#include <common.h>
#include <env.h>
#include <hwconfig.h>
#include <command.h>
#include <netdev.h>
#include <malloc.h>
#include <fsl_mdio.h>
#include <miiphy.h>
#include <phy.h>
#include <fm_eth.h>
#include <asm/io.h>
#include <exports.h>
#include <asm/arch/fsl_serdes.h>
#include <fsl-mc/fsl_mc.h>
#include <fsl-mc/ldpaa_wriop.h>

DECLARE_GLOBAL_DATA_PTR;

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}

#if defined(CONFIG_FSL_MC_ENET)
int fdt_fixup_board_phy(void *fdt)
{
	return 0;
}
#endif /* CONFIG_FSL_MC_ENET */

#if defined(CONFIG_RESET_PHY_R)
void reset_phy(void)
{
}
#endif /* CONFIG_RESET_PHY_R */
