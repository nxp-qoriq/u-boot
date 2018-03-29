/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
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

#ifdef CONFIG_FSL_QIXIS
#include "../common/qixis.h"
#include "lx2160aqds_qixis.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)

 /* - In LS2080A there are only 16 SERDES lanes, spread across 2 SERDES banks.
 *   Bank 1 -> Lanes A, B, C, D, E, F, G, H
 *   Bank 2 -> Lanes A,B, C, D, E, F, G, H
 */

 /* Mapping of 16 SERDES lanes to LS2080A QDS board slots. A value of '0' here
  * means that the mapping must be determined dynamically, or that the lane
  * maps to something other than a board slot.
  */

static u8 lane_to_slot_fsm1[] = {
	0, 0, 0, 0, 0, 0, 0, 0
};

static u8 lane_to_slot_fsm2[] = {
	0, 0, 0, 0, 0, 0, 0, 0
};

static inline u8 serdes1_get_physical_lane(u8 lane)
{
	/* SerDes 1 is inverted therefore lane H is connected to SD[0] and
	* lane A is connected to SD[7].
	* Registers controlling lane A would still control lane A, and
	* hence control SD[7].
	*/
	return (7 - lane);
}

static int sgmii_riser_phy_addr[] = {
	SGMII_CARD_PORT1_PHY_ADDR,
	SGMII_CARD_PORT2_PHY_ADDR,
	SGMII_CARD_PORT3_PHY_ADDR,
	SGMII_CARD_PORT4_PHY_ADDR,
};

static int xgmii_aq_phy_addr[] = {
	AQ_PHY_ADDR1,
	AQ_PHY_ADDR2,
	AQ_PHY_ADDR3,
	AQ_PHY_ADDR4,
};

static int xgmii_cortina_phy_addr[] = {
	CORTINA_PHY_ADDR1,
	CORTINA_PHY_ADDR1,
	CORTINA_PHY_ADDR1,
	CORTINA_PHY_ADDR1,
};

static int xgmii_phyless_phy_addr[] = {
	0x1C,
	0x1D,
	0x1E,
	0x1F,
};

typedef enum {
	IO_SLOT_NONE = 0,
	IO_SLOT_1,
	IO_SLOT_2,
	IO_SLOT_3,
	IO_SLOT_4,
	IO_SLOT_5,
	IO_SLOT_6,
	IO_SLOT_8,
	EMI1_RGMII1,
	EMI1_RGMII2,
	IO_SLOT_MAX
} IO_SLOT;

struct lx2160a_qds_mdio {
	IO_SLOT ioslot:4;
	u8 realbusnum:4;
	struct mii_dev *realbus;
};

static void lx2160a_qds_mux_mdio(struct lx2160a_qds_mdio *priv)
{
	u8 brdcfg4, reg;

	brdcfg4 = QIXIS_READ(brdcfg[4]);
	reg = brdcfg4;

	switch (priv->realbusnum) {
		case 1:
			switch (priv->ioslot) {
				case EMI1_RGMII1:
					brdcfg4 &= ~0xC0;
					break;
				case EMI1_RGMII2:
					brdcfg4 &= ~0xC0;
					brdcfg4 |= 1U << 6;
					break;
				default:
					brdcfg4 |= 3U << 6;
					brdcfg4 &= ~BRDCFG4_EMI1SEL_MASK;
					brdcfg4 |= (priv->ioslot - 1) << BRDCFG4_EMI1SEL_SHIFT;
					break;
			}
			break;
		case 2:
			brdcfg4 &= ~BRDCFG4_EMI2SEL_MASK;
			brdcfg4 |= (priv->ioslot - 1) << BRDCFG4_EMI2SEL_SHIFT;
			break;
	}

	if (brdcfg4 ^ reg)
		QIXIS_WRITE(brdcfg[4], brdcfg4);
}

static int lx2160a_qds_mdio_read(struct mii_dev *bus, int addr,
				 int devad, int regnum)
{
	struct lx2160a_qds_mdio *priv = bus->priv;

	lx2160a_qds_mux_mdio(priv);

	return priv->realbus->read(priv->realbus, addr, devad, regnum);
}

static int lx2160a_qds_mdio_write(struct mii_dev *bus, int addr, int devad,
				  int regnum, u16 value)
{
	struct lx2160a_qds_mdio *priv = bus->priv;

	lx2160a_qds_mux_mdio(priv);

	return priv->realbus->write(priv->realbus, addr, devad, regnum, value);
}

static int lx2160a_qds_mdio_reset(struct mii_dev *bus)
{
	struct lx2160a_qds_mdio *priv = bus->priv;

	return priv->realbus->reset(priv->realbus);
}

static struct mii_dev* lx2160a_qds_mdio_init(u8 realbusnum, IO_SLOT ioslot)
{
	struct lx2160a_qds_mdio *pmdio;
	struct mii_dev *bus;
	/*should be within MDIO_NAME_LEN*/
	char dummy_mdio_name[] = "LX2160A_QDS_MDIO1_IOSLOT1";

	if (realbusnum == 2) {
		if ((ioslot < IO_SLOT_1) || (ioslot > IO_SLOT_8)) {
			printf("invalid ioslot %d\n", ioslot);
			return NULL;
		}
	} else if (realbusnum == 1) {
		if ((ioslot < IO_SLOT_1) ||
			(ioslot > EMI1_RGMII2))
		{
			printf("invalid ioslot %d\n", ioslot);
			return NULL;
		}
	} else {
		printf("not supported real mdio bus %d\n", realbusnum);
		return NULL;
	}

	if (ioslot == EMI1_RGMII1)
		strcpy(dummy_mdio_name, "LX2160A_QDS_MDIO1_RGMII1");
	else if (ioslot == EMI1_RGMII2)
		strcpy(dummy_mdio_name, "LX2160A_QDS_MDIO1_RGMII2");
	else
		sprintf(dummy_mdio_name, "LX2160A_QDS_MDIO%d_IOSLOT%d",
				realbusnum, ioslot);
	bus = miiphy_get_dev_by_name(dummy_mdio_name);

	if (bus)
		return bus;

	bus = mdio_alloc();
	if (!bus) {
		printf("Failed to allocate %s bus\n", dummy_mdio_name);
		return NULL;
	}

	pmdio = malloc(sizeof(*pmdio));
	if (!pmdio) {
		printf("Failed to allocate %s private data\n", dummy_mdio_name);
		free(bus);
		return NULL;
	}

	switch (realbusnum) {
		case 1:
			pmdio->realbus = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO1_NAME);
			break;
		case 2:
			pmdio->realbus = miiphy_get_dev_by_name(DEFAULT_WRIOP_MDIO2_NAME);
			break;
	}

	if (!pmdio->realbus) {
		printf("No real mdio bus num %d found\n", realbusnum);
		free(bus);
		free(pmdio);
		return NULL;
	}

	pmdio->realbusnum = realbusnum;
	pmdio->ioslot = ioslot;
	bus->read = lx2160a_qds_mdio_read;
	bus->write = lx2160a_qds_mdio_write;
	bus->reset = lx2160a_qds_mdio_reset;
	strcpy(bus->name, dummy_mdio_name);
	bus->priv = pmdio;

	if (!mdio_register(bus))
		return bus;

	printf("No bus with name %s\n", dummy_mdio_name);
	free(bus);
	free(pmdio);
	return NULL;
}

/*
 * Initialize the dpmac_info array.
 *
 */
static void initialize_dpmac_to_slot(void)
{
	struct ccsr_gur __iomem *gur = (void *)CONFIG_SYS_FSL_GUTS_ADDR;
	int serdes1_prtcl = (in_le32(&gur->rcwsr[28]) &
				FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK)
		>> FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;
	int serdes2_prtcl = (in_le32(&gur->rcwsr[28]) &
				FSL_CHASSIS3_RCWSR28_SRDS2_PRTCL_MASK)
		>> FSL_CHASSIS3_RCWSR28_SRDS2_PRTCL_SHIFT;

	switch (serdes1_prtcl) {
	case 0x07:
		printf("qds: WRIOP: Supported SerDes1 Protocol 0x%02x\n",
		       serdes1_prtcl);
		lane_to_slot_fsm1[0] = IO_SLOT_1;
		lane_to_slot_fsm1[1] = IO_SLOT_1;
		lane_to_slot_fsm1[2] = IO_SLOT_1;
		lane_to_slot_fsm1[3] = IO_SLOT_1;
		lane_to_slot_fsm1[4] = IO_SLOT_3;
		lane_to_slot_fsm1[5] = IO_SLOT_3;
		lane_to_slot_fsm1[6] = IO_SLOT_3;
		lane_to_slot_fsm1[7] = IO_SLOT_3;
		break;
	case 0x28:
		lane_to_slot_fsm1[4] = IO_SLOT_3;
		lane_to_slot_fsm1[5] = IO_SLOT_3;
		lane_to_slot_fsm1[6] = IO_SLOT_3;
		lane_to_slot_fsm1[7] = IO_SLOT_3;
		break;
	case 0x2A:
		lane_to_slot_fsm1[0] = IO_SLOT_1;
		lane_to_slot_fsm1[1] = IO_SLOT_1;
		lane_to_slot_fsm1[2] = IO_SLOT_1;
		lane_to_slot_fsm1[3] = IO_SLOT_1;
		lane_to_slot_fsm1[4] = IO_SLOT_3;
		lane_to_slot_fsm1[5] = IO_SLOT_3;
		lane_to_slot_fsm1[6] = IO_SLOT_3;
		lane_to_slot_fsm1[7] = IO_SLOT_3;
	case 0x4B:
		lane_to_slot_fsm1[0] = IO_SLOT_1;
		lane_to_slot_fsm1[1] = IO_SLOT_1;
		lane_to_slot_fsm1[2] = IO_SLOT_1;
		lane_to_slot_fsm1[3] = IO_SLOT_1;
		break;
	case 0x4C:
		lane_to_slot_fsm1[4] = IO_SLOT_3;
		lane_to_slot_fsm1[5] = IO_SLOT_3;
		lane_to_slot_fsm1[6] = IO_SLOT_3;
		lane_to_slot_fsm1[7] = IO_SLOT_3;
		break;
	default:
		printf("%s qds: WRIOP: Unsupported SerDes1 Protocol 0x%02x\n",
		       __func__, serdes1_prtcl);
		break;
	}

	switch (serdes2_prtcl) {
	case 0x49:
	case 0x52:
		printf("qds: WRIOP: Supported SerDes2 Protocol 0x%02x\n",
		       serdes2_prtcl);
		lane_to_slot_fsm2[0] = IO_SLOT_5;
		lane_to_slot_fsm2[1] = IO_SLOT_5;
		lane_to_slot_fsm2[2] = IO_SLOT_5;
		lane_to_slot_fsm2[3] = IO_SLOT_5;
		break;

	default:
		printf(" %s qds: WRIOP: Unsupported SerDes2 Protocol 0x%02x\n",
		       __func__ , serdes2_prtcl);
		break;
	}
}

void lx2160a_handle_phy_interface_sgmii(int dpmac_id)
{
	int lane, slot;
	struct mii_dev *bus;
	struct ccsr_gur __iomem *gur = (void *)CONFIG_SYS_FSL_GUTS_ADDR;
	int serdes1_prtcl = (in_le32(&gur->rcwsr[28]) &
				FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK)
		>> FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;
	int serdes2_prtcl = (in_le32(&gur->rcwsr[28]) &
				FSL_CHASSIS3_RCWSR28_SRDS2_PRTCL_MASK)
		>> FSL_CHASSIS3_RCWSR28_SRDS2_PRTCL_SHIFT;

	if (dpmac_id > WRIOP1_DPMAC8)
		goto serdes2;

	switch (serdes1_prtcl) {
	case 0x07:
	case 0x28:
		lane = serdes_get_first_lane(FSL_SRDS_1, SGMII1 + dpmac_id - 1);
		lane = serdes1_get_physical_lane(lane);
		slot = lane_to_slot_fsm1[lane];
		if (lane > 3)
			wriop_set_phy_address(dpmac_id, sgmii_riser_phy_addr[lane - 4]);
		else
			wriop_set_phy_address(dpmac_id, sgmii_riser_phy_addr[lane]);
		bus = lx2160a_qds_mdio_init(1, slot);
		if(bus)
			wriop_set_mdio(dpmac_id, bus);
		break;
	default:
		printf("%s qds: WRIOP: Unsupported SerDes1 Protocol 0x%02x\n",
		       __func__ , serdes1_prtcl);
		break;
	}

serdes2:
	switch (serdes2_prtcl) {
	case 0x52:
	case 0x49:
		lane = serdes_get_first_lane(FSL_SRDS_2, SGMII9 + dpmac_id - 9);
		slot = lane_to_slot_fsm2[lane];
		if (lane > 3)
			printf ("not supported lane %d\n", lane);
		else
			wriop_set_phy_address(dpmac_id, sgmii_riser_phy_addr[lane]);
		bus = lx2160a_qds_mdio_init(1, slot);
		if(bus)
			wriop_set_mdio(dpmac_id, bus);
		break;
	default:
		printf("%s qds: WRIOP: Unsupported SerDes2 Protocol 0x%02x\n",
		       __func__, serdes2_prtcl);
		break;
	}
}

void lx2160a_handle_phy_interface_xsgmii(int dpmac_id)
{
	int lane, slot;
	struct mii_dev *bus;
	struct ccsr_gur __iomem *gur = (void *)CONFIG_SYS_FSL_GUTS_ADDR;
	int serdes1_prtcl = (in_le32(&gur->rcwsr[28]) &
				FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_MASK)
		>> FSL_CHASSIS3_RCWSR28_SRDS1_PRTCL_SHIFT;
	char *env_hwconfig = getenv("hwconfig");

	switch (serdes1_prtcl) {
	case 0x2A:
		lane = serdes_get_first_lane(FSL_SRDS_1, XFI1 + dpmac_id - 1);
		lane = serdes1_get_physical_lane(lane);
		slot = lane_to_slot_fsm1[lane];
		if (lane > 3) {
			wriop_set_phy_address(dpmac_id, xgmii_aq_phy_addr[lane - 4]);
			bus = lx2160a_qds_mdio_init(1, slot);
		} else {
			if (env_hwconfig && hwconfig_f("phyless", env_hwconfig)) {
				wriop_set_phy_address(dpmac_id, xgmii_phyless_phy_addr[lane]);
				bus = NULL;
			} else {
				wriop_set_phy_address(dpmac_id, xgmii_cortina_phy_addr[lane]);
				bus = lx2160a_qds_mdio_init(2, slot);
			}
		}
		if(bus)
			wriop_set_mdio(dpmac_id, bus);
		break;
	case 0x4B:
		lane = serdes_get_first_lane(FSL_SRDS_1, XFI1 + dpmac_id - 1);
		lane = serdes1_get_physical_lane(lane);
		slot = lane_to_slot_fsm1[lane];
		wriop_set_phy_address(dpmac_id, xgmii_aq_phy_addr[lane]);
		bus = lx2160a_qds_mdio_init(1, slot);
		if(bus)
			wriop_set_mdio(dpmac_id, bus);
		break;
	case 0x4C:
		lane = serdes_get_first_lane(FSL_SRDS_1, XFI5 + dpmac_id - 5);
		lane = serdes1_get_physical_lane(lane);
		slot = lane_to_slot_fsm1[lane];
		wriop_set_phy_address(dpmac_id, xgmii_aq_phy_addr[lane - 4]);
		bus = lx2160a_qds_mdio_init(1, slot);
		if(bus)
			wriop_set_mdio(dpmac_id, bus);
		break;
	default:
		printf("qds: WRIOP: Unsupported SerDes Protocol 0x%02x\n",
		       serdes1_prtcl);
		break;
	}
}
#endif /* CONFIG_FMAN_ENET */

int board_eth_init(bd_t *bis)
{
#if defined(CONFIG_FSL_MC_ENET) && !defined(CONFIG_SPL_BUILD)
	struct memac_mdio_info mdio_info;
	struct memac_mdio_controller *regs;
	int i;
	u8 ioslot;
	struct mii_dev *bus;

	regs = (struct memac_mdio_controller *)CONFIG_SYS_FSL_WRIOP1_MDIO1;
	mdio_info.regs = regs;
	mdio_info.name = DEFAULT_WRIOP_MDIO1_NAME;

	/*Register the EMI 1*/
	fm_memac_mdio_init(bis, &mdio_info);

	regs = (struct memac_mdio_controller *)CONFIG_SYS_FSL_WRIOP1_MDIO2;
	mdio_info.regs = regs;
	mdio_info.name = DEFAULT_WRIOP_MDIO2_NAME;

	/*Register the EMI 2*/
	fm_memac_mdio_init(bis, &mdio_info);

	initialize_dpmac_to_slot();

	for(i = WRIOP1_DPMAC1; i <= WRIOP1_DPMAC16; i++) {
		switch (wriop_get_enet_if(i)) {
		case PHY_INTERFACE_MODE_SGMII:
			lx2160a_handle_phy_interface_sgmii(i);
			break;
		case PHY_INTERFACE_MODE_XGMII:
			lx2160a_handle_phy_interface_xsgmii(i);
			break;
		default:
			break;
		}
	}

	/*Set MDIO bus 1 muxing front-ends for RGMII interfaces*/
	for (ioslot = EMI1_RGMII1; ioslot <= EMI1_RGMII2; ioslot++) {
		i = (ioslot - EMI1_RGMII1) + 1;
		bus = lx2160a_qds_mdio_init(1, ioslot);
		if (!bus)
			printf("could not get bus for RGMII%d\n", i);
		else
			wriop_set_mdio(WRIOP1_DPMAC16 + i, bus);
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
