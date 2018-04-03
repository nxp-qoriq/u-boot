/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
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
#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
	int i, interface;
	struct memac_mdio_info mdio_info;
	struct mii_dev *dev;
/*	struct ccsr_gur *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR);*/
	struct memac_mdio_controller *reg;

	reg = (struct memac_mdio_controller *)CONFIG_SYS_FSL_WRIOP1_MDIO1;
	mdio_info.regs = reg;
	mdio_info.name = DEFAULT_WRIOP_MDIO1_NAME;

	/* Register the EMI 1 */
	fm_memac_mdio_init(bis, &mdio_info);

	reg = (struct memac_mdio_controller *)CONFIG_SYS_FSL_WRIOP1_MDIO2;
	mdio_info.regs = reg;
	mdio_info.name = DEFAULT_WRIOP_MDIO2_NAME;

	/* Register the EMI 2 */
	fm_memac_mdio_init(bis, &mdio_info);

	/*
	 * Interface type is decided based on RCW. Forcefully register
	 * WRIOP1_DPMAC5 interface XGMII to test CS4223 during Interposer
	 * phase
	 */
	wriop_init_dpmac_enet_if(WRIOP1_DPMAC5, PHY_INTERFACE_MODE_XGMII);

	/*Add serdes specific and interface specific
	 switch in final board eth file*/
	wriop_set_phy_address(WRIOP1_DPMAC1, AQR107_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC2, AQR107_PHY_ADDR2);
	wriop_set_phy_address(WRIOP1_DPMAC3, INPHI_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC4, INPHI_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC5, CORTINA_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC6, CORTINA_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC7, CORTINA_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC8, CORTINA_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC17, RGMII_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC18, RGMII_PHY_ADDR2);

	for (i = WRIOP1_DPMAC1; i <= WRIOP1_DPMAC2; i++) {
		interface = wriop_get_enet_if(i);
		switch (interface) {
		case PHY_INTERFACE_MODE_XGMII:
			dev = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO1_NAME);
			wriop_set_mdio(i, dev);
			break;
		default:
			break;
		}
	}
	for (i = WRIOP1_DPMAC3; i <= WRIOP1_DPMAC4; i++) {
		interface = wriop_get_enet_if(i);
		switch (interface) {
		case PHY_INTERFACE_MODE_XGMII:
			dev = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO2_NAME);
			wriop_set_mdio(i, dev);
			break;
		default:
			break;
		}
	}

	for (i = WRIOP1_DPMAC5; i <= WRIOP1_DPMAC8; i++) {
		switch (wriop_get_enet_if(i)) {
		case PHY_INTERFACE_MODE_XGMII:
			dev = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO1_NAME);
			wriop_set_mdio(i, dev);
			break;
		default:
			break;
		}
	}
	for (i = WRIOP1_DPMAC17; i <= WRIOP1_DPMAC18; i++) {
		switch (wriop_get_enet_if(i)) {
		case PHY_INTERFACE_MODE_RGMII:
			printf("Enabilng RGMII\n");
			dev = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO1_NAME);
			wriop_set_mdio(i, dev);
			break;
		default:
			break;
		}
	}

	cpu_eth_init(bis);
#endif /* CONFIG_FMAN_ENET */

#ifdef CONFIG_PHY_AQUANTIA
	/*
	 * Export functions to be used by AQ firmware
	 * upload application
	 */
	gd->jt->strcpy = strcpy;
	gd->jt->mdelay = mdelay;
	gd->jt->mdio_get_current_dev = mdio_get_current_dev;
	gd->jt->phy_find_by_mask = phy_find_by_mask;
	gd->jt->mdio_phydev_for_ethname = mdio_phydev_for_ethname;
	gd->jt->miiphy_set_current_dev = miiphy_set_current_dev;
#endif
	return pci_eth_init(bis);
}

#if defined(CONFIG_RESET_PHY_R)
void reset_phy(void)
{
       mc_env_boot();
}
#endif /* CONFIG_RESET_PHY_R */
