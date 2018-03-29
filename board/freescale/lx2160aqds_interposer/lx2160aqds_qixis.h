/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LX2160AQDS_QIXIS_H__
#define __LX2160AQDS_QIXIS_H__

/* Definitions of QIXIS Registers for LX2160QDS */

/* SYSCLK */
#define QIXIS_SYSCLK_100		0x0
#define QIXIS_SYSCLK_125		0x1
#define QIXIS_SYSCLK_133		0x2

/* DDRCLK */
#define QIXIS_DDRCLK_100		0x0
#define QIXIS_DDRCLK_125		0x1
#define QIXIS_DDRCLK_133		0x2

#define BRDCFG4_EMI1SEL_MASK		0x38
#define BRDCFG4_EMI1SEL_SHIFT		3
#define BRDCFG4_EMI2SEL_MASK		0x07
#define BRDCFG4_EMI2SEL_SHIFT		0

#endif
