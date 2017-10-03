/*
 * Copyright 2015-2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <netdev.h>
#include <fm_eth.h>
#include <fsl_mdio.h>
#include <malloc.h>
#include <fsl_dtsec.h>
#include <asm/arch/soc.h>
#include <asm/arch-fsl-layerscape/config.h>
#include <asm/arch/fsl_serdes.h>

#include <pfe_eth/pfe_eth.h>
#include <asm/arch-fsl-layerscape/immap_lsch2.h>

#define DEFAULT_PFE_MDIO_NAME "PFE_MDIO"
#define DEFAULT_PFE_MDIO1_NAME "PFE_MDIO1"

#define MASK_ETH_PHY_RST	0x00000100

void reset_phy(void)
{
	unsigned int val;
	struct ccsr_gpio *pgpio = (void *)(GPIO1_BASE_ADDR);

	setbits_be32(&pgpio->gpdir, MASK_ETH_PHY_RST);

	val = in_be32(&pgpio->gpdat);
	setbits_be32(&pgpio->gpdat, val & ~MASK_ETH_PHY_RST);
	mdelay(10);

	val = in_be32(&pgpio->gpdat);
	setbits_be32(&pgpio->gpdat, val | MASK_ETH_PHY_RST);
	mdelay(50);
}

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_FSL_PFE
	struct mii_dev *bus;
	struct mdio_info mac1_mdio_info;

	reset_phy();

	init_pfe_scfg_dcfg_regs();

	mac1_mdio_info.reg_base = (void *)EMAC1_BASE_ADDR;
	mac1_mdio_info.name = DEFAULT_PFE_MDIO_NAME;

	bus = ls1012a_mdio_init(&mac1_mdio_info);
	if (!bus) {
		printf("Failed to register mdio\n");
		return -1;
	}

	/* We don't really need this MDIO bus,
	 * this is called just to initialize EMAC2 MDIO interface */
	mac1_mdio_info.reg_base = (void *)0x04220000; /*EMAC2_BASE_ADDR*/
	mac1_mdio_info.name = DEFAULT_PFE_MDIO1_NAME;

	bus = ls1012a_mdio_init(&mac1_mdio_info);
	if (!bus) {
		printf("Failed to register mdio\n");
		return -1;
	}

	/* MAC1 */
	ls1012a_set_mdio(0, miiphy_get_dev_by_name(DEFAULT_PFE_MDIO_NAME));
	ls1012a_set_phy_address_mode(0, EMAC1_PHY_ADDR,
				     PHY_INTERFACE_MODE_SGMII);

	/* MAC2 */
	ls1012a_set_mdio(1, miiphy_get_dev_by_name(DEFAULT_PFE_MDIO_NAME));
	ls1012a_set_phy_address_mode(1, EMAC2_PHY_ADDR,
				     PHY_INTERFACE_MODE_SGMII);


	return cpu_eth_init(bis);
#endif
}
