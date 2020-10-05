// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020 NXP
 *
 */

#include <common.h>
#include <command.h>
#include <net.h>
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
#if defined(CONFIG_FSL_MC_ENET)
	struct memac_mdio_info mdio_info;
	struct memac_mdio_controller *reg;
	int i;
	struct mii_dev *dev;
	struct ccsr_gur *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR);
	u32 srds_s1;

	srds_s1 = in_le32(&gur->rcwsr[28]) &
				FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK;
	srds_s1 >>= FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;

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

	/* BlueBox3 uses custom RCW and SerDes protocol 31, based on 11 */
	if (srds_s1 != 31) {
		printf("SerDes1 protocol 0x%x is not supported by LX2160A-BlueBox3\n",
		       srds_s1);
		goto next;
	}

	/* USXGMII setup */
	wriop_init_dpmac_enet_if(WRIOP1_DPMAC5, PHY_INTERFACE_MODE_USXGMII);
	wriop_init_dpmac_enet_if(WRIOP1_DPMAC6, PHY_INTERFACE_MODE_USXGMII);
	wriop_init_dpmac_enet_if(WRIOP1_DPMAC9, PHY_INTERFACE_MODE_USXGMII);
	wriop_init_dpmac_enet_if(WRIOP1_DPMAC10, PHY_INTERFACE_MODE_USXGMII);
	wriop_set_phy_address(WRIOP1_DPMAC5, 0, AQR113_PHY_ADDR1);
	wriop_set_phy_address(WRIOP1_DPMAC6, 0, AQR113_PHY_ADDR2);
	wriop_set_phy_address(WRIOP1_DPMAC9, 0, AQR113_PHY_ADDR3);
	wriop_set_phy_address(WRIOP1_DPMAC10, 0, AQR113_PHY_ADDR4);


	/* assign DPMAC and corresponding PHYs to MDIO busses */
	for (i = WRIOP1_DPMAC5; i <= WRIOP1_DPMAC6; i++) {
		dev = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO1_NAME);
		wriop_set_mdio(i, dev);
	}

	for (i = WRIOP1_DPMAC9; i <= WRIOP1_DPMAC10; i++) {
		dev = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO2_NAME);
		wriop_set_mdio(i, dev);
	}

next:
	cpu_eth_init(bis);
#endif /* CONFIG_FSL_MC_ENET */

	return pci_eth_init(bis);
}

#if defined(CONFIG_RESET_PHY_R)
void reset_phy(void)
{
#if defined(CONFIG_FSL_MC_ENET)
	mc_env_boot();
#endif
}
#endif /* CONFIG_RESET_PHY_R */

int fdt_fixup_board_phy(void *fdt)
{
	//TO DO
	return 0;
}
