// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2019 NXP
 *
 * PCIe DT fixup for NXP Layerscape SoCs
 * Author: Wasim Khan <wasim.khan@nxp.com>
 *
 */

#include <common.h>
#include <asm/arch/clock.h>
#include <asm/arch/soc.h>
#include "pcie_layerscape_fixup_common.h"

void ft_pci_setup(void *blob, bd_t *bd)
{
#if defined(CONFIG_FSL_LAYERSCAPE)
	uint svr;

	svr = SVR_SOC_VER(get_svr());

	if (svr == SVR_LX2160A && IS_SVR_REV(get_svr(), 1, 0))
		ft_pci_setup_ls_gen4(blob, bd);
	else
		ft_pci_setup_ls(blob, bd);
#else /* CONFIG_FSL_LAYERSCAPE */
	ft_pci_setup_ls(blob, bd);
#endif /* CONFIG_FSL_LAYERSCAPE */
}