/*
 * Copyright 2017 NXP.
 * Authors: Sriram Dash <sriram.dash@nxp.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <i2c.h>
#include <asm/io.h>
#ifdef CONFIG_FSL_LSCH2
#include <asm/arch/immap_lsch2.h>
#elif defined(CONFIG_FSL_LSCH3)
#include <asm/arch/immap_lsch3.h>
#else
#include <asm/immap_85xx.h>
#endif
#include "emc2305.h"

DECLARE_GLOBAL_DATA_PTR;

void configure_emc2305(void)
{
	u8 data;

	data = CONFIG_I2C_EMC2305_CMD;
	if (i2c_write(CONFIG_I2C_EMC2305_ADDR,
			I2C_EMC2305_CONF, 1, &data, 1) != 0){
		printf("Error: failed to configure I2C fan @%x\n",
			CONFIG_I2C_EMC2305_ADDR);
	}
}

void set_fan_speed(void)
{
	u8 data;

	data = CONFIG_I2C_EMC2305_PWM;
	if (i2c_write(CONFIG_I2C_EMC2305_ADDR,
			I2C_EMC2305_FAN1, 1, &data, 1) != 0){
		printf("Error: failed to change fan speed @%x\n",
			I2C_EMC2305_FAN1);
	}

	if (i2c_write(CONFIG_I2C_EMC2305_ADDR,
			I2C_EMC2305_FAN2, 1, &data, 1) != 0){
		printf("Error: failed to change fan speed @%x\n",
			I2C_EMC2305_FAN2);
	}

	if (i2c_write(CONFIG_I2C_EMC2305_ADDR,
			I2C_EMC2305_FAN3, 1, &data, 1) != 0){
		printf("Error: failed to change fan speed @%x\n",
			I2C_EMC2305_FAN3);
	}

	if (i2c_write(CONFIG_I2C_EMC2305_ADDR,
			I2C_EMC2305_FAN4, 1, &data, 1) != 0){
		printf("Error: failed to change fan speed @%x\n",
			I2C_EMC2305_FAN4);
	}

	if (i2c_write(CONFIG_I2C_EMC2305_ADDR,
			I2C_EMC2305_FAN5, 1, &data, 1) != 0){
		printf("Error: failed to change fan speed @%x\n",
			I2C_EMC2305_FAN5);
	}
}

void emc2305_init()
{
	configure_emc2305();
	set_fan_speed();
}
