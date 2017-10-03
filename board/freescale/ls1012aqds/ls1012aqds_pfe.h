/*
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#define ETH_1_1G_BUS_ID		0x1
#define ETH_1_1G_PHY_ID		0x1e
#define ETH_1_1G_MDIO_MUX	0x2
#define ETH_1G_MDIO_PHY_MASK	0xBFFFFFFD
#define ETH_1_1G_PHY_MODE	"sgmii"
#define ETH_2_1G_BUS_ID		0x1
#define ETH_2_1G_PHY_ID		0x1
#define ETH_2_1G_MDIO_MUX	0x1
#define ETH_2_1G_PHY_MODE	"rgmii"

#define ETH_1_2_5G_BUS_ID	0x0
#define ETH_1_2_5G_PHY_ID	0x1
#define ETH_1_2_5G_MDIO_MUX	0x2
#define ETH_2_5G_MDIO_PHY_MASK	0xFFFFFFF9
#define ETH_2_5G_PHY_MODE	"sgmii-2500"
#define ETH_2_2_5G_BUS_ID	0x1
#define ETH_2_2_5G_PHY_ID	0x2
#define ETH_2_2_5G_MDIO_MUX	0x3

#define SERDES_1_G_PROTOCOL	0x3508
#define SERDES_2_5_G_PROTOCOL	0x2205

#define PFE_PROP_LEN		4

#define ETH_1_SOC_PATH		"/soc/pfe@04000000/ethernet@0"
#define ETH_1_PATH		"/pfe@04000000/ethernet@0"
#define ETH_2_SOC_PATH		"/soc/pfe@04000000/ethernet@1"
#define ETH_2_PATH		"/pfe@04000000/ethernet@1"

#define ETH_1_MDIO_SOC_PATH	"/soc/pfe@04000000/ethernet@0/mdio@0"
#define ETH_1_MDIO		"/pfe@04000000/ethernet@0/mdio@0"
#define ETH_2_MDIO_SOC_PATH	"/soc/pfe@04000000/ethernet@1/mdio@0"
#define ETH_2_MDIO		"/pfe@04000000/ethernet@1/mdio@0"

#define NUM_ETH_NODE		2
struct pfe_prop_val {
	int busid;
	int phyid;
	int mux_val;
	int phy_mask;
	char *phy_mode;
};
