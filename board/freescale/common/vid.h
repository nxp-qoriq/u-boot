/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 */

#ifndef __VID_H_
#define __VID_H_

/* IR36021 command codes */
#define IR36021_LOOP1_MANUAL_ID_OFFSET	0x6A
#define IR36021_LOOP1_VOUT_OFFSET	0x9A
#define IR36021_MFR_ID_OFFSET		0x92
#define IR36021_MFR_ID			0x43
#define IR36021_INTEL_MODE_OFFSET	0x14
#define IR36021_MODE_MASK		0x20
#define IR36021_INTEL_MODE		0x00
#define IR36021_AMD_MODE		0x20

/* Step the IR regulator in 5mV increments */
#define IR_VDD_STEP_DOWN		5
#define IR_VDD_STEP_UP			5

/* PM Bus command codes for standard PMBus chips */
#define PMBUS_CMD_PAGE				0x0
#define PMBUS_CMD_READ_VOUT			0x8B
#define PMBUS_CMD_PAGE_PLUS_WRITE	0x05
#define PMBUS_CMD_VOUT_COMMAND		0x21
#define PMBUS_CMD_VOUT_MODE			0x20

int adjust_vdd(ulong vdd_override);
u16 soc_get_fuse_vid(int vid_index);

#endif  /* __VID_H_ */
