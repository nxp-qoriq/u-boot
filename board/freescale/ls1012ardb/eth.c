/*
 * Copyright 2015-2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier:GPL-2.0+
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
#include <i2c.h>

#define DEFAULT_PFE_MDIO_NAME "PFE_MDIO"


void reset_phy(void)
{
	/* Through reset IO expander reset both RGMII and SGMII PHYs */
	i2c_reg_write(I2C_MUX_IO2_ADDR, 6, __PHY_MASK);
	i2c_reg_write(I2C_MUX_IO2_ADDR, 2, __PHY_ETH2_MASK);
	mdelay(10);
	i2c_reg_write(I2C_MUX_IO2_ADDR, 2, __PHY_ETH1_MASK);
	mdelay(10);
	i2c_reg_write(I2C_MUX_IO2_ADDR, 2, 0xFF);
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

	/* MAC1 */
	ls1012a_set_mdio(0, miiphy_get_dev_by_name(DEFAULT_PFE_MDIO_NAME));
	ls1012a_set_phy_address_mode(0, EMAC1_PHY_ADDR,
				     PHY_INTERFACE_MODE_SGMII);

	/* MAC2 */
	ls1012a_set_mdio(1, miiphy_get_dev_by_name(DEFAULT_PFE_MDIO_NAME));
	ls1012a_set_phy_address_mode(1, EMAC2_PHY_ADDR,
				     PHY_INTERFACE_MODE_RGMII_TXID);

	cpu_eth_init(bis);
#endif
	return pci_eth_init(bis);
}
