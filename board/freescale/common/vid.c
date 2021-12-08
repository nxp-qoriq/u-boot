// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 * Copyright 2020 NXP
 */

#include <common.h>
#include <command.h>
#include <env.h>
#include <i2c.h>
#include <irq_func.h>
#include <asm/io.h>
#ifdef CONFIG_FSL_LSCH2
#include <asm/arch/immap_lsch2.h>
#elif defined(CONFIG_FSL_LSCH3)
#include <asm/arch/immap_lsch3.h>
#else
#include <asm/immap_85xx.h>
#endif
#include "vid.h"
DECLARE_GLOBAL_DATA_PTR;

int __weak i2c_multiplexer_select_vid_channel(u8 channel)
{
	return 0;
}

/*
 * Compensate for a board specific voltage drop between regulator and SoC
 * return a value in mV
 */
int __weak board_vdd_drop_compensation(void)
{
	return 0;
}

/*
 * Board specific settings for specific voltage value
 */
int __weak board_adjust_vdd(int vdd)
{
	return 0;
}

/*
 * Get the i2c address configuration for the regulator chip
 *
 * There are some variance in the RDB HW regarding the I2C address configuration
 * for the IR regulator chip, which is likely a problem of external resistor
 * accuracy. So we just check each address in a hopefully non-intrusive mode
 * and use the first one that seems to work
 *
 * The IR chip can show up under the following addresses:
 * 0x08 (Verified on T1040RDB-PA,T4240RDB-PB,X-T4240RDB-16GPA)
 * 0x09 (Verified on T1040RDB-PA)
 * 0x38 (Verified on T2080QDS, T2081QDS, T4240RDB)
 *
 * For other types of regulator chips, we check the IDs before we
 * return the address to avoid making damaging mistakes
 */
static int find_vid_chip_on_i2c(void)
{
#if defined(CONFIG_VOL_MONITOR_IR36021_READ) || \
	defined(CONFIG_VOL_MONITOR_IR36021_SET)
	int i2caddress;
	int ret;
	u8 byte;
	int i;
	const int ir_i2c_addr[] = {0x38, 0x08, 0x09};
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	/* Check all the address */
	for (i = 0; i < (sizeof(ir_i2c_addr)/sizeof(ir_i2c_addr[0])); i++) {
		i2caddress = ir_i2c_addr[i];
#ifndef CONFIG_DM_I2C
		ret = i2c_read(i2caddress,
			       IR36021_MFR_ID_OFFSET, 1, (void *)&byte,
			       sizeof(byte));
#else
		ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
		if (!ret)
			ret = dm_i2c_read(dev, IR36021_MFR_ID_OFFSET,
					  (void *)&byte, sizeof(byte));
#endif
		if (!ret && byte == IR36021_MFR_ID)
			return i2caddress;
	}
#endif
#if defined(CONFIG_VOL_MONITOR_LTC3882_READ) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_READ) || \
	defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_SET)
	int i2caddress = I2C_VOL_MONITOR_ADDR;
	int ret;
	u8 buf[8];
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

#ifndef CONFIG_DM_I2C
	ret = i2c_read(i2caddress,
		       LTC_MFR_ID, 1, (void *)&buf[0],
		       4);
#else
	ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
	if (!ret)
		ret = dm_i2c_read(dev, LTC_MFR_ID,
				  (void *)&buf[0], 4);
#endif
	if (!ret && memcmp(buf, "\3LTC", 4) == 0) {
#ifndef CONFIG_DM_I2C
		ret = i2c_read(i2caddress,
			       LTC_MFR_MODEL, 1, (void *)&buf[0],
			       8);
#else
		ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
		if (!ret)
			ret = dm_i2c_read(dev, LTC_MFR_MODEL,
					  (void *)&buf[0], 8);
#endif
		if (!ret) {
#if defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC3882_READ)
			if (memcmp(buf, "\7LTC3882", 8) == 0)
#elif defined(CONFIG_VOL_MONITOR_LTC7132_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_READ)
			if (memcmp(buf, "\7LTC7132", 8) == 0)
#endif
				return i2caddress;
		}
	}
#endif
	return -1;
}

/* Maximum loop count waiting for new voltage to take effect */
#define MAX_LOOP_WAIT_NEW_VOL		255 /* ms due to udelay(1000) */
/* Maximum loop count waiting for the voltage to be stable */
#define MAX_LOOP_WAIT_VOL_STABLE	100 /* ms due to udelay(1000) */
/*
 * If an INA220 chip is available, we can use it to read back the voltage
 * as it may have a higher accuracy than the IR chip for the same purpose
 */
#ifdef CONFIG_VOL_MONITOR_INA220
#define NUM_READINGS    4       /* prefer to be power of 2 for efficiency */
#define WAIT_FOR_ADC	532	/* wait for 532 microseconds for ADC */
#endif
#ifdef CONFIG_VOL_MONITOR_IR36021_READ
#define NUM_READINGS    4       /* prefer to be power of 2 for efficiency */
#define WAIT_FOR_ADC	138	/* wait for 138 microseconds for ADC */
#endif
#ifdef CONFIG_VOL_MONITOR_IR36021_SET
#define ADC_MIN_ACCURACY	IR_ADC_MIN_ACCURACY
#define VDD_STEP_UP		IR_VDD_STEP_UP
#define VDD_STEP_DOWN		IR_VDD_STEP_DOWN
#endif
#if defined(CONFIG_VOL_MONITOR_LTC3882_READ) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_READ)
#define WAIT_FOR_ADC		0
#endif
#if defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_SET)
#define ADC_MIN_ACCURACY	LTC_ADC_MIN_ACCURACY
#define VDD_STEP_UP		LTC_VDD_STEP_UP
#define VDD_STEP_DOWN		LTC_VDD_STEP_DOWN
#endif
#if (defined(VDD_STEP_UP) && (VDD_STEP_UP < 1)) || \
	(defined(VDD_STEP_DOWN) && (VDD_STEP_DOWN < 1))
#error VDD_STEP values must be > 0!
#endif

#ifdef CONFIG_VOL_MONITOR_INA220
static int read_voltage_from_INA220(int i2caddress)
{
	int i, ret, voltage_read = 0;
	u16 vol_mon;
	u8 buf[2];
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	for (i = 0; i < NUM_READINGS; i++) {
#ifndef CONFIG_DM_I2C
		ret = i2c_read(i2caddress,
			       I2C_VOL_MONITOR_BUS_V_OFFSET, 1,
			       (void *)&buf[0], 2);
#else
		ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
		if (!ret)
			ret = dm_i2c_read(dev, I2C_VOL_MONITOR_BUS_V_OFFSET,
					  (void *)&buf[0], 2);
#endif
		if (ret) {
			printf("VID: failed to read core voltage\n");
			return ret;
		}
		vol_mon = (buf[0] << 8) | buf[1];
		if (vol_mon & I2C_VOL_MONITOR_BUS_V_OVF) {
			printf("VID: Core voltage sensor error\n");
			return -1;
		}
		debug("VID: bus voltage reads 0x%04x\n", vol_mon);
		/* LSB = 4mv */
		voltage_read += (vol_mon >> I2C_VOL_MONITOR_BUS_V_SHIFT) * 4;
		udelay(WAIT_FOR_ADC);
	}
	/* calculate the average */
	voltage_read /= NUM_READINGS;

	return voltage_read;
}
#endif

/* read voltage from IR */
#ifdef CONFIG_VOL_MONITOR_IR36021_READ
static int read_voltage_from_IR(int i2caddress)
{
	int i, ret, voltage_read = 0;
	u16 vol_mon;
	u8 buf;
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	for (i = 0; i < NUM_READINGS; i++) {
#ifndef CONFIG_DM_I2C
		ret = i2c_read(i2caddress,
			       IR36021_LOOP1_VOUT_OFFSET,
			       1, (void *)&buf, 1);
#else
		ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
		if (!ret)
			ret = dm_i2c_read(dev, IR36021_LOOP1_VOUT_OFFSET,
					  (void *)&buf, 1);
#endif
		if (ret) {
			printf("VID: failed to read vcpu\n");
			return ret;
		}
		vol_mon = buf;
		if (!vol_mon) {
			printf("VID: Core voltage sensor error\n");
			return -1;
		}
		debug("VID: bus voltage reads 0x%02x\n", vol_mon);
		/* Resolution is 1/128V. We scale up here to get 1/128mV
		 * and divide at the end
		 */
		voltage_read += vol_mon * 1000;
		udelay(WAIT_FOR_ADC);
	}
	/* Scale down to the real mV as IR resolution is 1/128V, rounding up */
	voltage_read = DIV_ROUND_UP(voltage_read, 128);

	/* calculate the average */
	voltage_read /= NUM_READINGS;

	/* Compensate for a board specific voltage drop between regulator and
	 * SoC before converting into a VID value
	 */
	voltage_read -= board_vdd_drop_compensation();

	return voltage_read;
}
#endif

#if defined(CONFIG_VOL_MONITOR_LTC3882_READ) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_READ)
/* read the current value of the LTC Regulator Voltage.
 * This will only read the first channel for dual channel setups
 */
static int read_voltage_from_LTC(int i2caddress)
{
	int  ret, vcode = 0;
	u8 chan = LTC_VID_CHANNEL;
	u8 buf[2];
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	/* select the PAGE 0 using PMBus commands PAGE for VDD*/
#ifndef CONFIG_DM_I2C
	ret = i2c_write(i2caddress,
			LTC_PAGE, 1, &chan, 1);
#else
	ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
	if (!ret)
		ret = dm_i2c_write(dev, LTC_PAGE, &chan, 1);
#endif
	if (ret) {
		printf("VID: failed to select VDD Page\n");
		return ret;
	}

	/*read the output voltage using PMBus command READ_VOUT*/
#ifndef CONFIG_DM_I2C
	ret = i2c_read(i2caddress,
		       LTC_READ_VOUT, 1, (void *)&buf[0], 2);
#else
	ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
	if (!ret)
		ret = dm_i2c_read(dev, LTC_READ_VOUT,
				  (void *)&buf[0], 2);
#endif
	if (ret) {
		printf("VID: failed to read the voltage\n");
		return ret;
	}
	vcode = (buf[1] << 8) | buf[0];

	/* Scale down to the real mV as LTC resolution is 1/4096V,rounding up */
	vcode = DIV_ROUND_UP(vcode * 1000, 4096);

	return vcode;
}
#endif

static int read_voltage(int i2caddress)
{
	int voltage_read;
#ifdef CONFIG_VOL_MONITOR_INA220
	voltage_read = read_voltage_from_INA220(I2C_VOL_MONITOR_ADDR);
#elif defined CONFIG_VOL_MONITOR_IR36021_READ
	voltage_read = read_voltage_from_IR(i2caddress);
#elif defined(CONFIG_VOL_MONITOR_LTC3882_READ) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_READ)
	voltage_read = read_voltage_from_LTC(i2caddress);
#else
	voltage_read = -1;
#endif
	if (voltage_read >= 0) {
		/* Compensate for a board specific voltage drop between
		 * regulator and SoC before converting into an IR VID value
		 */
		voltage_read -= board_vdd_drop_compensation();
	}

	return voltage_read;
}

/*
 * We need to calculate how long before the voltage stops to drop
 * or increase. It returns with the loop count. Each loop takes
 * several readings (WAIT_FOR_ADC)
 */
static int wait_for_new_voltage(int vdd, int i2caddress)
{
	int timeout, vdd_current;

	vdd_current = read_voltage(i2caddress);
	/* wait until voltage starts to reach the target. Voltage slew
	 * rates by typical regulators will always lead to stable readings
	 * within each fairly long ADC interval in comparison to the
	 * intended voltage delta change until the target voltage is
	 * reached. The fairly small voltage delta change to any target
	 * VID voltage also means that this function will always complete
	 * within few iterations. If the timeout was ever reached, it would
	 * point to a serious failure in the regulator system.
	 */
	for (timeout = 0;
	     abs(vdd - vdd_current) > ADC_MIN_ACCURACY &&
	     timeout < MAX_LOOP_WAIT_NEW_VOL; timeout++) {
		udelay(1000);
		vdd_current = read_voltage(i2caddress);
	}
	if (timeout >= MAX_LOOP_WAIT_NEW_VOL) {
		printf("VID: Voltage adjustment timeout\n");
		return -1;
	}
	return timeout;
}

/*
 * this function keeps reading the voltage until it is stable or until the
 * timeout expires
 */
static int wait_for_voltage_stable(int i2caddress)
{
	int timeout, vdd_current, vdd;

	vdd = read_voltage(i2caddress);
	udelay(1000);

	/* wait until voltage is stable */
	vdd_current = read_voltage(i2caddress);
	/* The maximum timeout is
	 * MAX_LOOP_WAIT_VOL_STABLE * (1000µs + NUM_READINGS * WAIT_FOR_ADC)
	 */
	for (timeout = MAX_LOOP_WAIT_VOL_STABLE;
	     abs(vdd - vdd_current) > ADC_MIN_ACCURACY &&
	     timeout > 0; timeout--) {
		vdd = vdd_current;
		udelay(1000);
		vdd_current = read_voltage(i2caddress);
	}
	if (timeout == 0)
		return -1;
	return vdd_current;
}

#ifdef CONFIG_VOL_MONITOR_IR36021_SET
/* Set the voltage to the IR chip */
static int set_voltage_to_IR(int i2caddress, int vdd)
{
	int wait, vdd_last;
	int ret;
	u8 vid;
	u8 buf;
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	/* check if IR chip works in Intel mode*/
#ifndef CONFIG_DM_I2C
	ret = i2c_read(i2caddress,
		       IR36021_INTEL_MODE_OFFSET,
		       1, (void *)&buf, 1);
#else
	ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
	if (!ret)
		ret = dm_i2c_read(dev, IR36021_INTEL_MODE_OFFSET,
				  (void *)&buf, 1);
#endif
	if (ret) {
		printf("VID: failed to read IR chip mode.\n");
		return -1;
	}
	if ((buf & IR36021_MODE_MASK) != IR36021_INTEL_MODE) {
		/* AMD SVI2 mode. */
		/* An increase by 5 in the VID value causes a decrease
		 * by 4/128V in the output rail, -6.25mV per step.
		 * The scale appears to be 0V based starting from 0xf8.
		 */
		vid = 248 - DIV_ROUND_UP(vdd * 4, 25);
	} else {
		/* Intel mode */
		vid = DIV_ROUND_UP(vdd - 245, 5);
	}

#ifndef CONFIG_DM_I2C
	ret = i2c_write(i2caddress, IR36021_LOOP1_MANUAL_ID_OFFSET,
			1, (void *)&vid, sizeof(vid));
#else
	ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
	if (!ret)
		ret = dm_i2c_write(dev, IR36021_LOOP1_MANUAL_ID_OFFSET,
				   (void *)&vid, sizeof(vid));

#endif
	if (ret) {
		printf("VID: failed to write VID\n");
		return -1;
	}
	wait = wait_for_new_voltage(vdd, i2caddress);
	if (wait < 0)
		return -1;
	debug("VID: Waited %d us\n",
	      wait * (1000 + NUM_READINGS * WAIT_FOR_ADC));

	vdd_last = wait_for_voltage_stable(i2caddress);
	if (vdd_last < 0)
		return -1;
	debug("VID: Current voltage is %d mV\n", vdd_last);
	return vdd_last;
}
#endif

#if defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_SET)
/* Helper function to write a mV value as LTC L16 into the chip,
 * returning a boolean for success
 */
static int write_l16_mV_LTC(int i2caddress, int cmd, int mv)
{
	int l16;
	int ret;
	u8 buf[5];
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	/* Scale mV to L16 */
	l16 = mv;
	l16 <<= 12;
	l16 /= 1000;
	debug("VID: cmd 0x%02x voltage write 0x%04x\n", cmd, l16);
	buf[0] = 4;
	buf[1] = LTC_VID_CHANNEL;
	buf[2] = cmd;
	buf[3] = (l16 & 0xff);
	buf[4] = (l16 >> 8);

	/* This code assumes that both channels run the very
	 * SAME voltage. This is likely true for LS2 style
	 * devices. For any other configuration, all hell will
	 * break loose!
	 */
#ifndef CONFIG_DM_I2C
	ret = i2c_write(i2caddress,
			LTC_PAGE_PLUS_WRITE, 1, (void *)&buf, 5);
#else
	ret = i2c_get_chip_for_busnum(0, i2caddress, 1, &dev);
	if (!ret)
		ret = dm_i2c_write(dev,
				   LTC_PAGE_PLUS_WRITE, (void *)&buf, 5);
#endif
	return ret;
}
#endif

#if defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_SET)
/* this function sets the VDD and returns the value set */
static int set_voltage_to_LTC(int i2caddress, int vdd)
{
	int wait, ret, vdd_last;

	/* Write the desired voltage code to the regulator */
	ret = write_l16_mV_LTC(i2caddress,
			       LTC_VOUT_COMMAND,
			       vdd);
	if (ret) {
		printf("VID: I2C failed to write to the voltage regulator\n");
		return -1;
	}

	/* Wait for the voltage to get to the desired value */
	wait = wait_for_new_voltage(vdd, i2caddress);
	if (wait < 0)
		return -1;

	vdd_last = wait_for_voltage_stable(i2caddress);
	if (vdd_last < 0)
		return -1;
	return vdd_last;
}
#endif

static int set_voltage(int i2caddress, int vdd)
{
	int vdd_last = -1;

	/* Compensate for a board specific voltage drop between regulator and
	 * SoC before converting into a VID value
	 */
	vdd += board_vdd_drop_compensation();
#ifdef CONFIG_VOL_MONITOR_IR36021_SET
	vdd_last = set_voltage_to_IR(i2caddress, vdd);
#elif defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_SET)
	vdd_last = set_voltage_to_LTC(i2caddress, vdd);
#else
	#error Specific voltage monitor must be defined
#endif
	return vdd_last;
}

#if defined(CONFIG_FSL_LSCH3)
static bool soc_has_lowbitsinfusesr(struct ccsr_gur *gur)
{
	u32 svr = in_le32(&gur->svr);

	/* LS2088A derivatives have different FUSESR */
	switch (SVR_SOC_VER(svr)) {
	case SVR_LS2088A:
	case SVR_LS2048A:
	case SVR_LS2084A:
	case SVR_LS2044A:
	case SVR_LX2160A:
	case SVR_LX2120A:
	case SVR_LX2080A:
	return true;
	}

	return false;
}
#endif /* CONFIG_FSL_LSCH3 */

int vid_set_mv_limits(int absmax,
		      int marginhigh, int marginlow,
		      int ovfault, int ovwarn,
		      int uvwarn, int uvfault)
{
	int ret;

#if defined(CONFIG_VOL_MONITOR_LTC3882_SET) || \
	defined(CONFIG_VOL_MONITOR_LTC7132_SET)
	int i2caddress;
	ret = i2c_multiplexer_select_vid_channel(I2C_MUX_CH_VOL_MONITOR);
	if (ret) {
		debug("VID: I2C failed to switch channel\n");
		return ret;
	}
	i2caddress = find_vid_chip_on_i2c();

	if (i2caddress >= 0) {
		/* We need to program the voltage limits
		 * properly, or the chip may freak out on
		 * VID changes.
		 */
		ret = write_l16_mV_LTC(i2caddress,
				       LTC_VOUT_MAX,
				       absmax);
		if (!ret) {
			ret = write_l16_mV_LTC(i2caddress,
					       LTC_VOUT_MARGIN_HIGH,
					       marginhigh);
		}
		if (!ret) {
			ret = write_l16_mV_LTC(i2caddress,
					       LTC_VOUT_MARGIN_LOW,
					       marginlow);
		}
		if (!ret) {
			ret = write_l16_mV_LTC(i2caddress,
					       LTC_VOUT_OV_FAULT_LIMIT,
					       ovfault);
		}
		if (!ret) {
			ret = write_l16_mV_LTC(i2caddress,
					       LTC_VOUT_OV_WARN_LIMIT,
					       ovwarn);
		}
		if (!ret) {
			ret = write_l16_mV_LTC(i2caddress,
					       LTC_VOUT_UV_WARN_LIMIT,
					       uvwarn);
		}
		if (!ret) {
			ret = write_l16_mV_LTC(i2caddress,
					       LTC_VOUT_UV_FAULT_LIMIT,
					       uvfault);
		}
	} else {
		ret = -1;
	}

	if (ret)
		printf("VID: Setting voltage limits failed! VID regulation may not be stable!\n");

	i2c_multiplexer_select_vid_channel(I2C_MUX_CH_DEFAULT);
#else /* CONFIG_VOL_MONITOR_LTC3882_SET || CONFIG_VOL_MONITOR_LTC7132_SET*/
	ret = 0;
#endif
	return ret;
}

int adjust_vdd(ulong vdd_override)
{
#if defined(CONFIG_FSL_LSCH2) || defined(CONFIG_FSL_LSCH3)
	struct ccsr_gur *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR);
#else
	ccsr_gur_t __iomem *gur =
		(void __iomem *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
#endif
	u32 fusesr;
	u8 vid;
	int vdd_target, vdd_current, vdd_last;
	int ret, i2caddress;
	unsigned long vdd_string_override;
	char *vdd_string;
	/* Note that these are all possible values for the FUSESR
	 * register. Specific SoCs will only ever reference their
	 * own subset. Manual overrides can be used for settings
	 * outside the specification and need to be used carefully.
	 */
#ifdef CONFIG_FSL_LSCH3
#ifdef CONFIG_ARCH_LX2160A
	static const u16 vdd[32] = {
		8250,
		7875,
		7750,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		8000,
		8125,
		8250,
		0,      /* reserved */
		8500,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
	};
#elif defined(CONFIG_ARCH_LS1088A)
	static const u16 vdd[32] = {
		10250,
		9875,
		9750,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		9000,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		10000,  /* 1.0000V */
		10125,
		10250,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
	};
#else
	static const u16 vdd[32] = {
		10500,
		0,      /* reserved */
		9750,
		0,      /* reserved */
		9500,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		9000,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		10000,  /* 1.0000V */
		0,      /* reserved */
		10250,
		0,      /* reserved */
		10500,
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
		0,      /* reserved */
	};
#endif
#else /* !CONFIG_FSL_LSCH3 */
	static const u16 vdd[32] = {
		0,      /* unused */
		9875,   /* 0.9875V */
		9750,
		9625,
		9500,
		9375,
		9250,
		9125,
		9000,
		8875,
		8750,
		8625,
		8500,
		8375,
		8250,
		8125,
		10000,  /* 1.0000V */
		10125,
		10250,
		10375,
		10500,
		10625,
		10750,
		10875,
		11000,
		0,      /* reserved */
	};
#endif
	struct vdd_drive {
		u8 vid;
		unsigned voltage;
	};

	ret = i2c_multiplexer_select_vid_channel(I2C_MUX_CH_VOL_MONITOR);
	if (ret) {
		debug("VID: I2C failed to switch channel\n");
		ret = -1;
		goto exit;
	}
	ret = find_vid_chip_on_i2c();
	if (ret < 0) {
		printf("VID: Could not find voltage regulator on I2C.\n");
		ret = -1;
		goto exit;
	} else {
		i2caddress = ret;
		debug("VID: Voltage regulator found on I2C address 0x%02x\n",
		      i2caddress);
	}

	/* get the voltage ID from fuse status register */
#ifdef CONFIG_FSL_LSCH3
	fusesr = in_le32(&gur->dcfg_fusesr);
#else
	fusesr = in_be32(&gur->dcfg_fusesr);
#endif
	/*
	 * VID is used according to the table below
	 *                ---------------------------------------
	 *                |                DA_V                 |
	 *                |-------------------------------------|
	 *                | 5b00000 | 5b00001-5b11110 | 5b11111 |
	 * ---------------+---------+-----------------+---------|
	 * | D | 5b00000  | NO VID  | VID = DA_V      | NO VID  |
	 * | A |----------+---------+-----------------+---------|
	 * | _ | 5b00001  |VID =    | VID =           |VID =    |
	 * | V |   ~      | DA_V_ALT|   DA_V_ALT      | DA_A_VLT|
	 * | _ | 5b11110  |         |                 |         |
	 * | A |----------+---------+-----------------+---------|
	 * | L | 5b11111  | No VID  | VID = DA_V      | NO VID  |
	 * | T |          |         |                 |         |
	 * ------------------------------------------------------
	 */
#ifdef CONFIG_FSL_LSCH2
	vid = (fusesr >> FSL_CHASSIS2_DCFG_FUSESR_ALTVID_SHIFT) &
		FSL_CHASSIS2_DCFG_FUSESR_ALTVID_MASK;
	if ((vid == 0) || (vid == FSL_CHASSIS2_DCFG_FUSESR_ALTVID_MASK)) {
		vid = (fusesr >> FSL_CHASSIS2_DCFG_FUSESR_VID_SHIFT) &
			FSL_CHASSIS2_DCFG_FUSESR_VID_MASK;
	}
#elif defined(CONFIG_FSL_LSCH3)
	if (soc_has_lowbitsinfusesr(gur)) {
		vid = (fusesr >> FSL_CHASSIS3_DCFG_FUSESR_LOW_ALTVID_SHIFT) &
			FSL_CHASSIS3_DCFG_FUSESR_LOW_ALTVID_MASK;
		if (vid == 0 ||
		    vid == FSL_CHASSIS3_DCFG_FUSESR_LOW_ALTVID_MASK) {
			vid = (fusesr >>
				FSL_CHASSIS3_DCFG_FUSESR_LOW_VID_SHIFT) &
				FSL_CHASSIS3_DCFG_FUSESR_LOW_VID_MASK;
		}
	} else {
		vid = (fusesr >> FSL_CHASSIS3_DCFG_FUSESR_ALTVID_SHIFT) &
			FSL_CHASSIS3_DCFG_FUSESR_ALTVID_MASK;
		if (vid == 0 ||
		    vid == FSL_CHASSIS3_DCFG_FUSESR_ALTVID_MASK) {
			vid = (fusesr >>
				FSL_CHASSIS3_DCFG_FUSESR_VID_SHIFT) &
				FSL_CHASSIS3_DCFG_FUSESR_VID_MASK;
		}
	}
#else
	vid = (fusesr >> FSL_CORENET_DCFG_FUSESR_ALTVID_SHIFT) &
		FSL_CORENET_DCFG_FUSESR_ALTVID_MASK;
	if ((vid == 0) || (vid == FSL_CORENET_DCFG_FUSESR_ALTVID_MASK)) {
		vid = (fusesr >> FSL_CORENET_DCFG_FUSESR_VID_SHIFT) &
			FSL_CORENET_DCFG_FUSESR_VID_MASK;
	}
#endif
	vdd_target = vdd[vid];

	/* check override variable for overriding VDD */
	vdd_string = env_get(CONFIG_VID_FLS_ENV);
	if (vdd_override == 0 && vdd_string &&
	    !strict_strtoul(vdd_string, 10, &vdd_string_override))
		vdd_override = vdd_string_override;
	if (vdd_override >= VDD_MV_MIN && vdd_override <= VDD_MV_MAX) {
		vdd_target = vdd_override * 10; /* convert to 1/10 mV */
		debug("VDD override is %lu\n", vdd_override);
	} else if (vdd_override != 0) {
		printf("VID: Invalid override value %lu mV.\n", vdd_override);
	}
	if (vdd_target == 0) {
		debug("VID: VID not used\n");
		ret = 0;
		goto exit;
	} else {
		/* divide and round up by 10 to get a value in mV */
		vdd_target = DIV_ROUND_UP(vdd_target, 10);
		printf("VID: SoC target voltage = %d mV\n", vdd_target);
	}

	/*
	 * Read voltage monitor to check real voltage.
	 */
	vdd_last = read_voltage(i2caddress);
	if (vdd_last < 0) {
		printf("VID: Couldn't read sensor abort VID adjustment\n");
		ret = -1;
		goto exit;
	}
	vdd_current = vdd_last;
	debug("VID: Core voltage is currently at %d mV\n", vdd_last);
	/*
	  * Adjust voltage to at or one step above target.
	  * As measurements are less precise than setting the values
	  * we may run through dummy steps that cancel each other
	  * when stepping up and then down.
	  */
#if defined(VDD_STEP_UP) && defined(VDD_STEP_DOWN)
	while (vdd_last > 0 &&
	       vdd_last < vdd_target &&
	       vdd_current < vdd_target) {
		vdd_current += min(VDD_STEP_UP,
				 vdd_target - vdd_current + ADC_MIN_ACCURACY);
		vdd_last = set_voltage(i2caddress, vdd_current);
	}
	while (vdd_last > 0 &&
	       vdd_last >= vdd_target + ADC_MIN_ACCURACY &&
	       vdd_current >= vdd_target + ADC_MIN_ACCURACY) {
		vdd_current -= min(VDD_STEP_DOWN, vdd_current - vdd_target);
		vdd_last = set_voltage(i2caddress, vdd_current);
	}
#endif

	if (vdd_last > 0) {
		printf("VID: Core voltage after adjustment is at %d mV\n",
		       vdd_last);
		ret = 0;
	} else {
		ret = -1;
	}
exit:
	i2c_multiplexer_select_vid_channel(I2C_MUX_CH_DEFAULT);

	return ret;
}

static int print_vdd(void)
{
	int vdd_last, ret, i2caddress;

	ret = i2c_multiplexer_select_vid_channel(I2C_MUX_CH_VOL_MONITOR);
	if (ret) {
		debug("VID : I2C failed to switch channel\n");
		return -1;
	}
	ret = find_vid_chip_on_i2c();
	if (ret < 0) {
		printf("VID: Could not find voltage regulator on I2C.\n");
		goto exit;
	} else {
		i2caddress = ret;
		debug("VID: Chip found on I2C address 0x%02x\n", i2caddress);
	}

	/*
	 * Read voltage monitor to check real voltage.
	 */
	vdd_last = read_voltage(i2caddress);
	if (vdd_last < 0) {
		printf("VID: Couldn't read sensor abort VID adjustment\n");
		goto exit;
	}
	printf("VID: Core voltage is at %d mV\n", vdd_last);
exit:
	i2c_multiplexer_select_vid_channel(I2C_MUX_CH_DEFAULT);

	return ret < 0 ? -1 : 0;

}

static int do_vdd_override(cmd_tbl_t *cmdtp,
			   int flag, int argc,
			   char * const argv[])
{
	ulong override;

	if (argc < 2)
		return CMD_RET_USAGE;

	if (!strict_strtoul(argv[1], 10, &override))
		adjust_vdd(override);   /* the value is checked by callee */
	else
		return CMD_RET_USAGE;
	return 0;
}

static int do_vdd_read(cmd_tbl_t *cmdtp,
			 int flag, int argc,
			 char * const argv[])
{
	if (argc < 1)
		return CMD_RET_USAGE;
	print_vdd();

	return 0;
}

U_BOOT_CMD(
	vdd_override, 2, 0, do_vdd_override,
	"override VDD",
	" - override with the voltage specified in mV, eg. 1050"
);

U_BOOT_CMD(
	vdd_read, 1, 0, do_vdd_read,
	"read VDD",
	" - Read the voltage specified in mV"
)
