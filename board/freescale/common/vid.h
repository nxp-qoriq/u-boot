/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020 NXP
 * Copyright 2014 Freescale Semiconductor, Inc.
 */

#ifndef __VID_H_
#define __VID_H_

/* Declarations for the IR36021 */
#define IR36021_LOOP1_MANUAL_ID_OFFSET	0x6A
#define IR36021_LOOP1_VOUT_OFFSET	0x9A
#define IR36021_MFR_ID_OFFSET		0x92
#define IR36021_MFR_ID			0x43
#define IR36021_INTEL_MODE_OFFSET	0x14
#define IR36021_MODE_MASK		0x20
#define IR36021_INTEL_MODE		0x00
#define IR36021_AMD_MODE		0x20

/* step the IR regulator in 5mV increments */
#define IR_VDD_STEP_DOWN		5 /* mV */
#define IR_VDD_STEP_UP			5 /* mV */
#define IR_ADC_MIN_ACCURACY		8 /* mV */

/* Declarations for the LTC regulator */
#define LTC_PAGE		0x00
#define LTC_PAGE_PLUS_WRITE	0x05
#define LTC_PAGE_PLUS_READ	0x06
#define LTC_VOUT_COMMAND	0x21
#define LTC_VOUT_MAX		0x24
#define LTC_VOUT_MARGIN_HIGH	0x25
#define LTC_VOUT_MARGIN_LOW	0x26
#define LTC_VOUT_OV_FAULT_LIMIT	0x40
#define LTC_VOUT_OV_WARN_LIMIT	0x42
#define LTC_VOUT_UV_WARN_LIMIT	0x43
#define LTC_VOUT_UV_FAULT_LIMIT	0x44
#define LTC_READ_VOUT		0x8B
#define LTC_MFR_ID		0x99
#define LTC_MFR_MODEL		0x9A

/* step the LTC regulator in 1mV increments */
#define LTC_VDD_STEP_DOWN		1 /* mV */
#define LTC_VDD_STEP_UP			1 /* mV */
#define LTC_ADC_MIN_ACCURACY		1 /* mV */

/* This is a compatibility setting for existing board configs */
#ifdef PWM_CHANNEL0
#define LTC_VID_CHANNEL	PWM_CHANNEL0
#endif

#ifdef PWM_CHANNEL1
#define LTC_VID_CHANNEL2 PWM_CHANNEL1
#endif

int vid_set_mv_limits(int absmax,
		      int marginhigh, int marginlow,
		      int ovfault, int ovwarn,
		      int uvwarn, int uvfault);

int adjust_vdd(ulong vdd_override);
#endif  /* __VID_H_ */
