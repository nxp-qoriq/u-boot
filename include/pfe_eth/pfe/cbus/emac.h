/*
 * Copyright 2015-2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _EMAC_H_
#define _EMAC_H_

#define EMAC_IEVENT_REG		0x004
#define EMAC_IMASK_REG		0x008
#define EMAC_R_DES_ACTIVE_REG	0x010
#define EMAC_X_DES_ACTIVE_REG	0x014
#define EMAC_ECNTRL_REG		0x024
#define EMAC_MII_DATA_REG	0x040
#define EMAC_MII_CTRL_REG	0x044
#define EMAC_MIB_CTRL_STS_REG	0x064
#define EMAC_RCNTRL_REG		0x084
#define EMAC_TCNTRL_REG		0x0C4
#define EMAC_PHY_ADDR_LOW	0x0E4
#define EMAC_PHY_ADDR_HIGH	0x0E8
#define EMAC_TFWR_STR_FWD	0x144
#define EMAC_RX_SECTIOM_FULL	0x190
#define EMAC_TX_SECTION_EMPTY	0x1A0
#define EMAC_TRUNC_FL		0x1B0

/* GEMAC definitions and settings */
#define EMAC_PORT_0			0
#define EMAC_PORT_1			1

/* GEMAC Bit definitions */
#define EMAC_IEVENT_HBERR                0x80000000
#define EMAC_IEVENT_BABR                 0x40000000
#define EMAC_IEVENT_BABT                 0x20000000
#define EMAC_IEVENT_GRA                  0x10000000
#define EMAC_IEVENT_TXF                  0x08000000
#define EMAC_IEVENT_TXB                  0x04000000
#define EMAC_IEVENT_RXF                  0x02000000
#define EMAC_IEVENT_RXB                  0x01000000
#define EMAC_IEVENT_MII                  0x00800000
#define EMAC_IEVENT_EBERR                0x00400000
#define EMAC_IEVENT_LC                   0x00200000
#define EMAC_IEVENT_RL                   0x00100000
#define EMAC_IEVENT_UN                   0x00080000

#define EMAC_IMASK_HBERR                 0x80000000
#define EMAC_IMASK_BABR                  0x40000000
#define EMAC_IMASKT_BABT                 0x20000000
#define EMAC_IMASK_GRA                   0x10000000
#define EMAC_IMASKT_TXF                  0x08000000
#define EMAC_IMASK_TXB                   0x04000000
#define EMAC_IMASKT_RXF                  0x02000000
#define EMAC_IMASK_RXB                   0x01000000
#define EMAC_IMASK_MII                   0x00800000
#define EMAC_IMASK_EBERR                 0x00400000
#define EMAC_IMASK_LC                    0x00200000
#define EMAC_IMASKT_RL                   0x00100000
#define EMAC_IMASK_UN                    0x00080000

#define EMAC_RCNTRL_MAX_FL_SHIFT         16
#define EMAC_RCNTRL_LOOP                 0x00000001
#define EMAC_RCNTRL_DRT                  0x00000002
#define EMAC_RCNTRL_MII_MODE             0x00000004
#define EMAC_RCNTRL_PROM                 0x00000008
#define EMAC_RCNTRL_BC_REJ               0x00000010
#define EMAC_RCNTRL_FCE                  0x00000020
#define EMAC_RCNTRL_RGMII                0x00000040
#define EMAC_RCNTRL_SGMII                0x00000080
#define EMAC_RCNTRL_RMII                 0x00000100
#define EMAC_RCNTRL_RMII_10T             0x00000200
#define EMAC_RCNTRL_CRC_FWD		 0x00004000

#define EMAC_TCNTRL_GTS                  0x00000001
#define EMAC_TCNTRL_HBC                  0x00000002
#define EMAC_TCNTRL_FDEN                 0x00000004
#define EMAC_TCNTRL_TFC_PAUSE            0x00000008
#define EMAC_TCNTRL_RFC_PAUSE            0x00000010

#define EMAC_ECNTRL_RESET                0x00000001      /* reset the EMAC */
#define EMAC_ECNTRL_ETHER_EN             0x00000002      /* enable the EMAC */
#define EMAC_ECNTRL_SPEED                0x00000020
#define EMAC_ECNTRL_DBSWAP               0x00000100

#define EMAC_X_WMRK_STRFWD               0x00000100

#define EMAC_X_DES_ACTIVE_TDAR           0x01000000
#define EMAC_R_DES_ACTIVE_RDAR           0x01000000

/*
 * The possible operating speeds of the MAC, currently supporting 10, 100 and
 * 1000Mb modes.
 */
enum mac_speed {SPEED_10M, SPEED_100M, SPEED_1000M, SPEED_1000M_PCS};

#define GMII	1
#define MII	2
#define RMII	3
#define RGMII	4
#define SGMII	5

#define DUPLEX_HALF	0x00
#define DUPLEX_FULL	0x01

/* Default configuration */
#define EMAC0_DEFAULT_DUPLEX_MODE	FULLDUPLEX
#define EMAC0_DEFAULT_EMAC_MODE		RGMII
#define EMAC0_DEFAULT_EMAC_SPEED	SPEED_1000M

#define EMAC1_DEFAULT_DUPLEX_MODE	FULLDUPLEX
#define EMAC1_DEFAULT_EMAC_MODE		SGMII
#define EMAC1_DEFAULT_EMAC_SPEED	SPEED_1000M

/* MII-related definitios */
#define EMAC_MII_DATA_ST         0x40000000      /* Start of frame delimiter */
#define EMAC_MII_DATA_OP_RD      0x20000000      /* Perform a read operation */
#define EMAC_MII_DATA_OP_CL45_RD 0x30000000      /* Perform a read operation */
#define EMAC_MII_DATA_OP_WR      0x10000000      /* Perform a write operation */
#define EMAC_MII_DATA_OP_CL45_WR 0x10000000      /* Perform a write operation */
#define EMAC_MII_DATA_PA_MSK     0x0f800000      /* PHY Address field mask */
#define EMAC_MII_DATA_RA_MSK     0x007c0000      /* PHY Register field mask */
#define EMAC_MII_DATA_TA         0x00020000      /* Turnaround */
#define EMAC_MII_DATA_DATAMSK    0x0000ffff      /* PHY data field */

#define EMAC_MII_DATA_RA_SHIFT   18      /* MII Register address bits */
#define EMAC_MII_DATA_RA_MASK	 0x1F      /* MII Register address mask */
#define EMAC_MII_DATA_PA_SHIFT   23      /* MII PHY address bits */
#define EMAC_MII_DATA_PA_MASK    0x1F      /* MII PHY address mask */

#define EMAC_MII_DATA_RA(v) ((v & EMAC_MII_DATA_RA_MASK) <<\
				EMAC_MII_DATA_RA_SHIFT)
#define EMAC_MII_DATA_PA(v) ((v & EMAC_MII_DATA_RA_MASK) <<\
				EMAC_MII_DATA_PA_SHIFT)
#define EMAC_MII_DATA(v)    (v & 0xffff)

#define EMAC_MII_SPEED_SHIFT	1
#define EMAC_HOLDTIME_SHIFT	8
#define EMAC_HOLDTIME_MASK	0x7
#define EMAC_HOLDTIME(v)    ((v & EMAC_HOLDTIME_MASK) << EMAC_HOLDTIME_SHIFT)

/* Internal PHY Registers - SGMII */
#define PHY_SGMII_CR_PHY_RESET      0x8000
#define PHY_SGMII_CR_RESET_AN       0x0200
#define PHY_SGMII_CR_DEF_VAL        0x1140
#define PHY_SGMII_DEV_ABILITY_SGMII 0x4001
#define PHY_SGMII_IF_MODE_AN        0x0002
#define PHY_SGMII_IF_MODE_SGMII     0x0001
#define PHY_SGMII_IF_MODE_SGMII_GBT 0x0008
#define PHY_SGMII_ENABLE_AN         0x1000

#endif /* _EMAC_H_ */
