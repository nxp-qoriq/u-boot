/*
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __EMC2305_H_
#define __EMC2305_H_

#define I2C_EMC2305_CONF		0x20
#define I2C_EMC2305_FAN1		0x30
#define I2C_EMC2305_FAN2		0x40
#define I2C_EMC2305_FAN3		0x50
#define I2C_EMC2305_FAN4		0x60
#define I2C_EMC2305_FAN5		0x70

void emc2305_init(void);

#endif  /* __EMC2305_H_ */
