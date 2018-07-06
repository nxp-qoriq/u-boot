
#ifndef __CS_CS4224_API__
#define __CS_CS4224_API__



#ifdef __cplusplus
extern "C" {
#endif

#include "cs_rtos.h"


/** @file cs4224_kran_types.h
 ****************************************************************************
 *
 * @brief
 *    KR-AN management types.
 *
 ******************************************************************************
 *  * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 *****************************************************************************/
#ifndef __CS4224_KRAN_TYPES_H__
#define __CS4224_KRAN_TYPES_H__

/**
 * KR-AN data rate negotiation complete
 */
typedef enum 
{
    /** KR AN not completed */
    CS4224_KRAN_AN_NOT_DONE,

    /** KR AN completed */
    CS4224_KRAN_AN_DONE,

} e_cs4224_kran_an_status_t; 

#pragma pack(push, 1)
/**
 * Storage for the user advertised configuration options
 * related to KR AN 
 */
typedef struct 
{
    /** (A4) (4 x 10G) 40G Faceplate/Cable */
    cs_uint16 tech_cr4;
    
    /** (A3) (4 x 10G) 40G Backplane */
    cs_uint16 tech_kr4;
    
    /** (A2) 10G Backplane */
    cs_uint16 tech_kr;
    
    /** (A0) 1G Backplane */
    cs_uint16 tech_kx;
    
    /** (A1) (4 x 2.5G) 10G Backplane */
    cs_uint16 tech_kx4;
    
    /** (F0) FEC Ability */
    cs_uint16 f0;
    
    /** (F1) FEC Requested */
    cs_uint16 f1;
    
    /** (C0) Link Partner Pause Ability */
    cs_uint16 pause_0;
    
    /** (C1) Link Partner Pause Ability */
    cs_uint16 pause_1;
    
    /** (D13) Remote Fault */
    cs_uint16 rf;

} cs4224_kran_advertised_config_t_s;
#pragma pack(pop)

#pragma pack(push, 1)
/**
 * Storage for the advanced configuration options
 * related to KR AN 
 */
typedef struct 
{
    /** Enable the line side fixed-phase override */
    cs_boolean phase_or;

    /** Enable the host side fixed-phase override */
    cs_boolean hphase_or;

    /** Enable the AN Ability Detect Timeout */
    cs_boolean en_adt;

    /** Enable the TP Max Wait Timer */
    cs_boolean en_mwt;

    /** Skip Auto-Negotiation */
    cs_boolean skip_an;

    /** Skip Training */
    cs_boolean skip_tp;

    /** Skip line side Phase Calibration */
    cs_boolean skip_phsc;

    /** Enable Link Fail Inhibit Timer */
    cs_boolean en_lfit;                                                                                                    

    /** Enable AN Retries */
    cs_boolean en_retry;

    /** Disable AN start */
    cs_boolean dis_start;

    /** Enable DFE Power Savings */
    cs_boolean en_pwrsv;

    /** Enable DFE */
    cs_boolean en_dfe;

    /** Count Frames with Invalid Markers */
    cs_boolean bad_mrkrs;

    /** Disable PCS Sync Checks in Mission Mode */
    cs_boolean dis_sync;                                                                                                   

    /** Enable the transmitter squelch control in the egress (host->line) path */
    cs_boolean sqlch_egr;

    /** Enable the transmitter squelch control in the ingress (line->host) path */
    cs_boolean sqlch_ing;

    /** Enables the use of the ring oscillator VCO */
    cs_boolean ring;

    /** Line side Fixed-phase value, used only when phase_or is TRUE */
    cs_uint16  phase_ov;

    /** Host side fixed-phase value, used only when hphase_or is TRUE */
    cs_uint16  hphase_ov;

    /** Number of Additional Frames to Send at the Start of SEND_DATA */
    cs_uint16  xtra_f;

    /** Iterations to Wait for PCS to Sync (multiply by 2^16) */
    cs_uint16  pcs_itr;                                                                                                    

    /** 10G LFI Timeout */
    cs_uint16  lfi_10g;

    /** TP_TX maximum wait */
    cs_uint16  max_wait;

    /** LFI Threshold */
    cs_uint16  lfi_thres;

    /** 1G LFI timeout */
    cs_uint16  lfi_1g;

    /** Override the LINK_READY wait_timer */
    cs_uint16  wait_tmr;

    /** True = Main-cursor incremented to maximum, False=Main-cursor untouched */
    cs_boolean  tpm0;

    /** True = Post-cursor adaptive calibration, False=Post-cursor incremented to maximum */ 
    cs_boolean  tpm1;

    /** True = Send preset command at start */
    cs_boolean  tpm2;

    /** True = Send init command at start */
    cs_boolean  tpm3;

    /** True = Force local_rx_ready to True immediately */
    cs_boolean  tpm4;

    /** Always set to 'b000 */
    cs_uint16   tpm567;

    /** True = Enable pre/main/post Cursor Preset Values */
    cs_boolean  preset;

    /** True = Skip Rx data corruption */
    cs_boolean  tpm30;

    /** True = Invert Pre and Post cursor orientation (Leeds emulation mode) */
    cs_boolean  tpm31;

    /** True = calibrate if we lose lock */
    cs_boolean  jmp2cal;

    /** True = reference clock frequency is 106.25Mhz, set according to rules->ref_clk_rate */
    cs_boolean  ref106;

    /** True = two-pass mode for optical links enabled */
    cs_uint16 two_pass_mode;

    /** True = disable FEC even when FEC is advertized and negotiated */
    cs_uint16 fec_bypass;

    /** Time to EDC converge in DFE mode, line side, range 0...0xffff, default 0x0180 */
    cs_uint16 line_t_to_conv_dfe;

    /** Time to EDC converge in DFE mode, host side, range 0...0xffff, default 0x0240 */
    cs_uint16 host_t_to_conv_dfe;

} cs4224_kran_advanced_config_t_s;

#pragma pack(pop)

#pragma pack(push, 1)

/**
 * Storage for the configuration options
 * related to KR AN 
 */
typedef struct 
{
    /** advertised config */
    cs4224_kran_advertised_config_t_s advertised;
    
    /** advanced config */
    cs4224_kran_advanced_config_t_s   advanced;

} cs4224_kran_config_t_s;

#pragma pack(pop)

#pragma pack(push, 1)
/**
 * Struct to store the final results of an AN sequence.
 */
typedef struct 
{
  /** Parallel detect */
  cs_boolean  bp_par_detect:1;

  /** (4 x 10G) 40G CR4 capability detect */
  cs_boolean  bp_40gcr4:1;
  
  /** (4 x 10G) 40G KR4 capability detect */
  cs_boolean  bp_40gkr4:1;

  /** 10G KR capability detect */
  cs_boolean  bp_10gkr:1;

  /** 1G KX capability detect */
  cs_boolean  bp_1000kx:1;

  /** (4 x 2.5G) 10G KX4 capability detect */
  cs_boolean  bp_10gkx4:1;

  /** FEC capability detect */
  cs_boolean  bp_fec:1;

  /** Remote Fault */
  cs_boolean  bp_rf:1;

  /** Pause ability */
  cs_uint8    bp_pause:3;

} cs4224_kran_results_t_s;  
#pragma pack(pop)

#endif
/** @file cs4224.h
 ****************************************************************************
 *
 * @brief
 *     This module describes the high level API methods provided by the
 *     CS4224 API.
 *
 ****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ***************************************************************************/
#ifndef __CS4224_H__
#define __CS4224_H__
#ifdef __cplusplus
extern "C" {
#endif

#if defined(CS_HAS_LINUX_KERNEL)
#    include <linux/kernel.h>
#endif

extern cs_uint32 cs4224_max_num_ports;

/* Query the number of supported ports. This method is deprecated
 * in favor of CS4224_MAX_NUM_SLICES() */
cs_uint32 CS4224_MAX_NUM_PORTS(void);

/* Query the number of slices on this device */
cs_uint32 CS4224_MAX_NUM_SLICES(cs_uint32 slice);

/* Query the number of dies on this device */
cs_uint32 CS4224_MAX_NUM_DIES(cs_uint32 slice);

/** apply this numerical value to a rule for it to be disabled */
#define CS4224_RULE_DISABLED 0xFFFF

/** The number of available ports or slices in the device   */
#define CS4224_MAX_NUM_CS4223_PORTS   4  /*  4 port duplex  */
#define CS4224_MAX_NUM_CS4224_PORTS  16  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4343_PORTS   8  /*  8 port duplex  */
#define CS4224_MAX_NUM_CS4221_PORTS  10  /* 10 port simplex */
#define CS4224_MAX_NUM_CS4227_PORTS   2  /*  2 port duplex  */
#define CS4224_MAX_NUM_CS4210_PORTS  16  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4341_PORTS   8  /*  8 port duplex */

/** The number of available dies in the device   */
#define CS4224_MAX_NUM_CS4223_DIES    1  /*  4 port duplex  */
#define CS4224_MAX_NUM_CS4224_DIES    2  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4343_DIES    2  /*  8 port duplex  */
#define CS4224_MAX_NUM_CS4221_DIES    2  /* 10 port simplex */
#define CS4224_MAX_NUM_CS4227_DIES    1  /*  2 port duplex  */
#define CS4224_MAX_NUM_CS4210_DIES    2  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4341_DIES    2  /*  8 port duplex */

/** The difference between line and host PP registers */
#define CS4224_LINE_TO_HOST_OFFSET    0x0800

#define CS4224_REF_CLK_156p25 156250 /* kHz */
#define CS4224_REF_CLK_155p52 155520
#define CS4224_REF_CLK_106p25 106250

/** The preferred GLOBAL_MDIO_CONFIG value */
#define CS4224_GLOBAL_MDIO_CONFIG_pref 0x78

/** Required or loaded ucode image bit map */
typedef enum
{
    /** microsequencer contains NO ucode */
    CS4224_UCODE_IMAGE_NONE = 0x0,

    /** Application requires or microsequencer contains the KR ucode */
    CS4224_UCODE_IMAGE_KR   = 0x1,

    /** Application requires or microsequencer contains the FC ucode */
    CS4224_UCODE_IMAGE_FC   = 0x2,

    /** Application requires ANY ucode */
    CS4224_UCODE_IMAGE_ANY  = 0x3,

}e_cs4224_ucode_image;

/** Used to identify a line side PCS monitoring device */
typedef enum
{
    /** No PCS monitor */
    CS4224_PCS_MONITOR_NONE  = 0x0,

    /** GEPCS monitor */
    CS4224_PCS_MONITOR_GEPCS = 0x1,

    /** EGPCS monitor */
    CS4224_PCS_MONITOR_EGPCS = 0x2,

    /** XGPCS monitor */
    CS4224_PCS_MONITOR_XGPCS = 0x3,

}e_cs4224_pcs_monitor_device;

/** Hardware ID numbers for the ASIC as defined in Efuse SKU */
typedef enum
{
    /** Undefined */
    CS4224_HW_UNDEF  = 0,

    /** 4 port duplex */
    CS4224_HW_CS4223 = 0x0010,

    /** 16 port simplex */
    CS4224_HW_CS4224 = 0x0011,

    /** 8 port duplex */
    CS4224_HW_CS4343 = 0x0012,
    
    /** 10 port simplex */
    CS4224_HW_CS4221 = 0x0013,

    /** 2 port duplex */
    CS4224_HW_CS4227 = 0x0014,

    /** 16 port simplex, 15G only */
    CS4224_HW_CS4210 = 0x0015,

    /** 8 port duplex, 10G only */
    CS4224_HW_CS4341 = 0x0016

}e_cs4224_hardware_id;

/** Pre-defined target applications used
 * with the cs4224_rules_set_default() method
 * to setup default configurations */
typedef enum
{

    /** Configure the device for 156.25MHz ref clock, 1.25GHz operation */
    CS4224_TARGET_APPLICATION_1G = 0,

    /** Configure the device for 156.25MHz ref clock, 5GHz operation */
    CS4224_TARGET_APPLICATION_5G,

    /** Configure the device for 156.25MHz ref clock, 7.5GHz operation */
    CS4224_TARGET_APPLICATION_7p5G,

    /** Configure the device for 156.25MHz ref clock, 8GHz operation (not verified yet) */
    CS4224_TARGET_APPLICATION_8G,

    /** Configure the device for 156.25MHz ref clock, 8.5GHz operation (not verified yet)*/
    CS4224_TARGET_APPLICATION_8p5G,

    /** Configure the device for 156.25MHz ref clock, 10.3125GHz operation */
    CS4224_TARGET_APPLICATION_10G,

    /** Configure the device for 156.25MHz ref clock, 15GHz operation */
    CS4224_TARGET_APPLICATION_15G,

    /** Configure the device for 10GE KR operation, deprecated, use CS4224_TARGET_APPLICATION_KRAN instead  */
    CS4224_TARGET_APPLICATION_10G_KR,

    /** Configure the device for 40GE KR operation, deprecated, use CS4224_TARGET_APPLICATION_KRAN instead  */
    CS4224_TARGET_APPLICATION_40G_KR,

    /** Configure the device for KR-AN operation */
    CS4224_TARGET_APPLICATION_KRAN,

    /** Configure the device for FC-AN operation */
    CS4224_TARGET_APPLICATION_FCAN,

    /** Configure the device for 106.25MHz ref clock, 14.025GHz FC operation */
    CS4224_TARGET_APPLICATION_16G_FC,

    /** Configure the device for 106.25MHz ref clock, 10.51875GHz FC operation */
    CS4224_TARGET_APPLICATION_10G_FC,

    /** Configure the device for 106.25MHz ref clock, 8.5GHz FC operation */
    CS4224_TARGET_APPLICATION_8G_FC, 

    /** Configure the device for 106.25MHz ref clock, 4.25GHz FC operation */
    CS4224_TARGET_APPLICATION_4G_FC,

    /** Configure the device for 106.25MHz ref clock, 2.125GHz FC operation */
    CS4224_TARGET_APPLICATION_2G_FC,

    /** Configure the device for 106.25MHz ref clock, 1.0625GHz FC operation */
    CS4224_TARGET_APPLICATION_1G_FC,
    
    /** Configure the device for 156.25MHz ref clock, 622.08MHz OC12 operation */
    CS4224_TARGET_APPLICATION_OC12

}e_cs4224_target_application;

/**
 * Data-path direction and side selector
 */
typedef enum
{
    /** Select the Line to Host path (Ingress) */
    CS4224_LINE_RX_TO_HOST_TX_DIR = 1,

    /** Configure the line side */
    CS4224_CFG_LINE_SIDE          = 1,

    /** Select the Host to Line path (Egress) */
    CS4224_HOST_RX_TO_LINE_TX_DIR = 2,

    /** Configure the host side */
    CS4224_CFG_HOST_SIDE          = 2,

    /** The channel is simplex so the direction is picked automatically by the API */
    CS4224_SIMPLEX_DIR = 3,

    /** Configure a simplex slice */
    CS4224_CFG_SIMPLEX = 3,

} e_cs4224_datapath_dir_t, e_cs4224_cfg_sides_t;

/**
 * The supply voltage for the voltage monitor 
 */
typedef enum
{
    /** 0.9V supply TX */
    CS4224_VLT_SUPPLY_0p9V = 0,

    /** 1.8V supply */
    CS4224_VLT_SUPPLY_1p8V = 1,

}e_cs4224_mon_vlt_supply;

/**
 * The EDC modes supported by the microcode
 */
typedef enum
{

    /** 
     * Disable the microcode. Does not disable the EDC.
     *
     * @{warning,
     *    This mode is not recommended and should not be used unless
     *    explicitly told to do so by Cortina
     * }
     */
    CS_HSIO_EDC_MODE_DISABLED = 0x0001,

    /**
     * DFE mode meant for CX1 or direct attach compatible cables.
     * @{table,
     * -h Supported Data Rates
     * -s Min     ! Max       ! Note
     * - 9.8Gbps  ! <10.6Gbps ! API Optimized range
     * - 10.6Gbps ! <11.3Gbps ! Optional tuning could be applied to improve performance.
     * - 11.3Gbps ! <12.5Gbps ! Optional tuning could be applied to improve performance.
     * }
     */
    CS_HSIO_EDC_MODE_CX1 = 0x0002,

    /**
     * Non-DFE mode meant to support low channel loss applications.
     *
     * Short for 'short reach'. This mode is suitable for connecting to SR modules, 
     * very short on-board connections, or anything involving slow data rates. 
     * 
     * @{note,
     * Although it's a fully adaptive mode, unlike DFE modes receiver tuning is 
     * required to maximize performance. See these rules:
     *
     * - rules.rx_if.dplx_line_eq (Duplex devices)
     * - rules.rx_if.dplx_host_eq (Duplex devices)
     * - rules.rx_if.splx_eq      (Simplex devices)
     * }
     * 
     * @{table,
     * -h Supported Data Rates
     * -s Min    ! Max     ! Note
     * - 622Mbps ! 15Gbps  ! API Optimized range
     * }
     *
     */
    CS_HSIO_EDC_MODE_SR = 0x0004,

    /**
     * DFE mode meant to support ZR modules.
     * 
     * @{table,
     * -h Supported Data Rates
     * -s Min     ! Max       ! Note
     * - 9.8Gbps  ! <10.6Gbps ! API Optimized range
     * - 10.6Gbps ! <11.3Gbps ! Optional tuning could be applied to improve performance.
     * - 11.3Gbps ! <12.5Gbps ! Optional tuning could be applied to improve performance.
     * }
     */
    CS_HSIO_EDC_MODE_ZR = 0x0008,

    /**
     * DFE mode meant to support DWDM modules.
     *
     * @{table,
     * -h Supported Data Rates
     * -s Min     ! Max       ! Note
     * - 9.8Gbps  ! <10.6Gbps ! API Optimized range
     * - 10.6Gbps ! <11.3Gbps ! Optional tuning could be applied to improve performance.
     * - 11.3Gbps ! <12.5Gbps ! Optional tuning could be applied to improve performance.
     * }
     */
    CS_HSIO_EDC_MODE_DWDM = 0x0010,

    /**
     * DFE mode meant to support 9.8 to 12.5G data rates over a backplane.
     *
     * @{note,
     * KR-AN DFE mode is supported through CS_HSIO_EDC_MODE_10G_BP, see KR-AN programming
     * examples for more info.
     * }
     * 
     * @{table,
     * -h Supported Data Rates
     * -s Min     ! Max       ! Note
     * - 9.8Gbps  ! <10.6Gbps ! API Optimized range
     * - 10.6Gbps ! <11.3Gbps ! Optional tuning could be applied to improve performance.
     * - 11.3Gbps ! <12.5Gbps ! Optional tuning could be applied to improve performance.
     * }
     */
    CS_HSIO_EDC_MODE_10G_BP = 0x0020,

    /**
     * DFE mode meant to support 12.5 to 15G data rates over a backplane.
     *
     * @{table,
     * -h Supported Data Rates
     * -s Min     ! Max     ! Note
     * - 12.5Gbps ! <14Gbps ! Optional tuning could be applied to improve performance.
     * - 14Gbps   ! 15Gbps  ! API optimized range
     * }
     */
    CS_HSIO_EDC_MODE_15G_BP = 0x0040,

    /**
     * DFE mode meant to support 5 to 7.5G data rates over a backplane.
     * 
     * @{note,
     * CS_HSIO_EDC_MODE_5G_BP and CS_HSIO_EDC_MODE_7p5G_BP are functionally equivalent
     * }
     * 
     * @{table,
     * -h Supported Data Rates
     * -s Min    ! Max     ! Note
     * - 5.0Gbps ! 7.5Gbps ! API optimized range
     * }
     */
    CS_HSIO_EDC_MODE_5G_BP = 0x0080,

    /**
     * DFE mode meant to support 5 to 7.5G data rates over a backplane.
     * 
     * @{note,
     * CS_HSIO_EDC_MODE_5G_BP and CS_HSIO_EDC_MODE_7p5G_BP are functionally equivalent
     * }
     * 
     * @{table,
     * -h Supported Data Rates
     * -s Min    ! Max     ! Note
     * - 5.0Gbps ! 7.5Gbps ! API optimized range
     * }
     */
    CS_HSIO_EDC_MODE_7p5G_BP = 0x0100,

    /**
     * DFE mode meant to support 8 to 8.6G data rates over a backplane.
     *
     * @{table,
     * -h Supported Data Rates
     * -s Min    ! Max     ! Note
     * - 8.0Gbps ! 8.6Gbps ! API optimized range
     * }
     */
    CS_HSIO_EDC_MODE_8p5G_BP = 0x0200,

    /**
     * DFE mode for the line side on FC-AN connections.
     * 
     * @{note,
     * FC-AN SR/DFE mode on the line side is supported through CS_HSIO_EDC_MODE_FCAN, 
     * see FC-AN programming examples for more info.
     * }
     *
     * @{table,
     * -h Supported Data Rates
     * -s Fibre Channel Rates ! Note
     * - FC1, FC2, FC4, FC8, FC10, FC16 ! API optimized range
     * }
     */
    CS_HSIO_EDC_MODE_FCAN = 0x0400,

    /**
     * DFE mode meant for very specific 15G backplane applications.
     * 
     * @{warning,
     * Do not use unless advised to do so by a Cortina AE.
     * }
     */
    CS_HSIO_EDC_MODE_15G_BP_27dB = 0x0800,

    /**
     * DFE mode meant for Single Mode Fibre with a Long Reach Module (SMLRM).
     * @{table,
     * -h Supported Data Rates
     * -s Min     ! Max       ! Note
     * - 9.8Gbps  ! <10.6Gbps ! API Optimized range
     * - 10.6Gbps ! <11.3Gbps ! Optional tuning could be applied to improve performance.
     * - 11.3Gbps ! <12.5Gbps ! Optional tuning could be applied to improve performance.
     * }
     */
    CS_HSIO_EDC_MODE_SMLRM = 0x1000

}e_cs4224_edc_mode;

/**
 * EDC modes, able to hold multiple modes by OR'ing edc_mode
 * enums together.
 * 
 * @example
 * 
 * cs4224_edc_modes valid_modes = CS4224_HSIO_EDC_MODE_SR  |
 *                                CS4224_HSIO_EDC_MODE_CX1 |
 *                                CS4224_HSIO_EDC_MODE_DWDM;
 */
typedef cs_uint16 cs4224_edc_modes;

/**
 * Settings used to compensate for trace loss on the board
 */
typedef enum
{
    /** For trace loss where trace loss <= 1dB */
    CS_HSIO_TRACE_LOSS_0dB,
    
    /** For trace loss where trace loss <= 2dB */
    CS_HSIO_TRACE_LOSS_1dB,

    /** For trace loss where 2dB < trace loss <= 3dB */
    CS_HSIO_TRACE_LOSS_2dB,
    
    /** For trace loss where 3dB < trace loss < 4dB */
    CS_HSIO_TRACE_LOSS_3dB,

    /** For trace loss where 4dB < trace loss <= 5dB */
    CS_HSIO_TRACE_LOSS_4dB,

    /** For trace loss where 5dB < trace loss <= 5.5dB */
    CS_HSIO_TRACE_LOSS_5dB,

    /** For trace loss where trace loss > 5.5dB */
    CS_HSIO_TRACE_LOSS_6dB,

    /** For trace loss where trace loss = 15dB */
    CS_HSIO_TRACE_LOSS_15dB,

    /** For trace loss where trace loss = 27dB */
    CS_HSIO_TRACE_LOSS_27dB,

    /** For CR4 cables */
    CS_HSIO_TRACE_LOSS_CR4,

    /** Special case for no-loss coax cables, for use in the lab */
    CS_HSIO_TRACE_LOSS_LAB,
    
}e_cs4224_trace_loss;

/**
 * Polarity Inversion bit map
 */
typedef enum
{
  /** No polarity inversion */ 
  CS4224_POLARITY_INV_NONE          = 0,

  /** Duplex host Tx driver polarity inversion */ 
  CS4224_POLARITY_INV_DPLX_HOST_TX  = 1 << 0,

  /** Duplex line Tx driver polarity inversion */ 
  CS4224_POLARITY_INV_DPLX_LINE_TX  = 1 << 1,

  /** Simplex Tx driver polarity inversion */ 
  CS4224_POLARITY_INV_SPLX_TX       = 1 << 2,

  /** Duplex host Rx (digital, pre-FEC) polarity inversion */ 
  CS4224_POLARITY_INV_DPLX_HOST_RX  = 1 << 3,

  /** Duplex line Rx (digital, pre-FEC) polarity inversion */ 
  CS4224_POLARITY_INV_DPLX_LINE_RX  = 1 << 4,

  /** Simplex Rx (digital, pre-FEC) polarity inversion */ 
  CS4224_POLARITY_INV_SPLX_RX       = 1 << 5,

} e_cs4224_polarity_inv_t;

/**
 * Low-level polarity inversion bit map
 * Describes each inversion point in the path
 * @private
 */
typedef enum
{
  /** No polarity inversion */ 
  CS4224_LL_INV_NONE          = 0,

  /** A: Digital line Rx */
  CS4224_LL_INV_DIGITAL_LINE_RX = 1 << 0,
  
  /** B: Digital host Tx */
  CS4224_LL_INV_DIGITAL_HOST_TX = 1 << 1,
  
  /** C: Analog host Tx */
  CS4224_LL_INV_ANALOG_HOST_TX = 1 << 2,
  
  /** D: Digital host Rx */
  CS4224_LL_INV_DIGITAL_HOST_RX = 1 << 3,
  
  /** E: Digital line Tx */
  CS4224_LL_INV_DIGITAL_LINE_TX = 1 << 4,
  
  /** F: Analog line Tx */
  CS4224_LL_INV_ANALOG_LINE_TX = 1 << 5,

} e_cs4224_ll_inv_t;

/**
 * KR-AN data rate advertise abilities bit map
 */
typedef enum
{
  /** 1000BASE-KX (A0) */
  CS4224_KRAN_DATA_RATE_1G  = 0x1,
  CS4224_KRAN_DATA_RATE_KX  = 0x1,

  /** 10GBASE-KR (A2) */
  CS4224_KRAN_DATA_RATE_10G = 0x2,
  CS4224_KRAN_DATA_RATE_KR = 0x2,

  /** 40GBASE-KR4 (4 x 10G) Backplane (A3) */
  CS4224_KRAN_DATA_RATE_BP_40G = 0x4,
  CS4224_KRAN_DATA_RATE_KR4 = 0x4,

  /** 40GBASE-CR4 (4 x 10G) Copper Cable Faceplate (A4) */
  CS4224_KRAN_DATA_RATE_FP_40G = 0x8,
  CS4224_KRAN_DATA_RATE_CR4 = 0x8,

  /** 10GBASE-KX4 (4 x 2.5G) Backplane (A1) */
  CS4224_KRAN_DATA_RATE_BP_KX4  = 0x10,
  CS4224_KRAN_DATA_RATE_KX4  = 0x10,

} e_cs4224_kran_data_rates_t;

/**
 * Determines which microsequencer to stall/un-stall
 */
typedef enum
{
    /** The simplex (line or host) microsequencer */
    CS4224_SPLX_MSEQ      = 0,

    /** The duplex (line) microsequencer */
    CS4224_DPLX_LINE_MSEQ = 1,

    /** The duplex (host) microsequencer */
    CS4224_DPLX_HOST_MSEQ = 2,

}e_cs4224_mseq_id;

/** Protection switch specific actions, note that for digital broadcast switching, 
 *  the 0 refers to the lower slice (even) of the protection pair, slice 1 the higher (odd).
 */
typedef enum
{
    /** disable switch */
    CS4224_SWITCH_DISABLE                  = 0,

    /** Duplex 2x2 switch from slices 0 to 1 and 1 to 0 bi-directionally */
    CS4224_SWITCH_DIGITAL_SWITCH_2x2       = 1,
    /** Duplex 2x2 switch from slices 0 to 1 and 1 to 0 bi-directionally */
    CS4224_SWITCH_DUPLEX_SWITCH_2x2        = 1,

    /** Digital broadcast, Rx/Tx on slice 0 to 0 with line Tx only on 1 (Egress dir only) */
    CS4224_SWITCH_DIGITAL_BROADCAST_0_to_0 = 2,
    /** Duplex broadcast, Rx/Tx on slice 0 to 0 with line Tx only on 1 (Egress dir only) */
    CS4224_SWITCH_DUPLEX_BROADCAST_0_to_0  = 2,

    /** Digital broadcast, Rx/Tx on slice 1 to 1 with line Tx only on 0 (Egress dir only) */
    CS4224_SWITCH_DIGITAL_BROADCAST_1_to_1 = 3,
    CS4224_SWITCH_DUPLEX_BROADCAST_1_to_1 = 3,

    /** Digital broadcast, Rx/Tx on slice 0 to 1 with line Tx only on 0 (Egress dir only) */
    CS4224_SWITCH_DIGITAL_BROADCAST_0_to_1 = 4,
    CS4224_SWITCH_DUPLEX_BROADCAST_0_to_1 = 4,

    /** Digital broadcast, Rx/Tx on slice 1 to 0 with line Tx only on 1 (Egress dir only) */
    CS4224_SWITCH_DIGITAL_BROADCAST_1_to_0 = 5,
    CS4224_SWITCH_DUPLEX_BROADCAST_1_to_0 = 5,

    /** Simplex 2x2 switch of slice with mate */
    CS4224_SWITCH_SIMPLEX_SWITCH_2x2        = 6,
    /** Analog 2x2 switch of slice with mate */
    CS4224_SWITCH_ANALOG_SWITCH_2x2        = 6,

    /** Simplex broadcast, transmits on slice and its mate */
    CS4224_SWITCH_SIMPLEX_BROADCAST         = 7,
    /** Analog broadcast, transmits on slice and its mate */
    CS4224_SWITCH_ANALOG_BROADCAST         = 7,

}e_cs4224_switch_action_t;

/**
 * The CLKDIV_CTRL, SRX_DDIV_SEL dividers
 * NOTE: There are accidentally two structures like this, the other is below
 */
typedef enum
{
    /** Rx serial clock div by 1 */
    CS4224_CLKDIV_DDIV_BY_1   = 0,

    /** Rx serial clock div by 2 */
    CS4224_CLKDIV_DDIV_BY_2   = 1,

    /** Rx serial clock div by 4 */
    CS4224_CLKDIV_DDIV_BY_4   = 2,

    /** Rx serial clock div by 8 */
    CS4224_CLKDIV_DDIV_BY_8   = 3,

    /** Rx serial clock div by 16 */
    CS4224_CLKDIV_DDIV_BY_16  = 4,

    /** Rx serial clock div by 32 */
    CS4224_CLKDIV_DDIV_BY_32  = 5,

    /** Rx serial clock div by 64 */
    CS4224_CLKDIV_DDIV_BY_64  = 6,

    /** Rx serial clock div by 128 */
    CS4224_CLKDIV_DDIV_BY_128 = 7,

}e_cs4224_clkdiv_ddiv_t;

/**
 * Divider for the clock rate for receive
 * data path.
 */
typedef enum
{
    /** Divide by 1 */
    CS4224_DDIV_DIV1 = 0,

    /** Divide by 2 */
    CS4224_DDIV_DIV2 = 1,

    /** Divide by 4 */
    CS4224_DDIV_DIV4 = 2,

    /** Divide by 8 */
    CS4224_DDIV_DIV8 = 3,

    /** Divide by 16 */
    CS4224_DDIV_DIV16 = 4,

    /** Divide by 32 */
    CS4224_DDIV_DIV32 = 5,
    
    /** Divide by 64 */
    CS4224_DDIV_DIV64 = 6,

    /** Divide by 128 */
    CS4224_DDIV_DIV128 = 7
}e_cs4224_ddiv_divide;

/**
 * The CLKDIV_CTRL, SRX_RDIV_SEL dividers
 * NOTE: There are accidentally two structures like this, the other is below
 */
typedef enum
{
    /** Rx serial clock div by 8 */
    CS4224_CLKDIV_RDIV_BY_8   = 0,

    /** Rx serial clock div by 16 */
    CS4224_CLKDIV_RDIV_BY_16  = 1,

    /** Rx serial clock div by 32 */
    CS4224_CLKDIV_RDIV_BY_32  = 2,

    /** Rx serial clock div by 40 */
    CS4224_CLKDIV_RDIV_BY_40  = 3,

    /** Rx serial clock div by 64 */
    CS4224_CLKDIV_RDIV_BY_64  = 4,

    /** Rx serial clock div by 66 */
    CS4224_CLKDIV_RDIV_BY_66  = 5,

    /** Rx serial clock div by 80 */
    CS4224_CLKDIV_RDIV_BY_80  = 6,

    /** Rx serial clock div by 100 */
    CS4224_CLKDIV_RDIV_BY_100 = 7,

    /** Rx serial clock div by 128 */
    CS4224_CLKDIV_RDIV_BY_128= 8,

}e_cs4224_clkdiv_rdiv_t;

/**
 * The serial clock divider to receive PFD
 * for comparison to the clock reference.
 */
typedef enum
{
    /** Rx serial clock div by 8 */
    CS4224_RDIV_DIV8   = 0,

    /** Rx serial clock div by 16 */
    CS4224_RDIV_DIV16  = 1,

    /** Rx serial clock div by 32 */
    CS4224_RDIV_DIV32  = 2,

    /** Rx serial clock div by 40 */
    CS4224_RDIV_DIV40  = 3,

    /** Rx serial clock div by 64 */
    CS4224_RDIV_DIV64  = 4,

    /** Rx serial clock div by 66 */
    CS4224_RDIV_DIV66  = 5,

    /** Rx serial clock div by 80 */
    CS4224_RDIV_DIV80  = 6,

    /** Rx serial clock div by 100 */
    CS4224_RDIV_DIV100 = 7,

    /** Rx serial clock div by 128 */
    CS4224_RDIV_DIV128= 8,

}e_cs4224_rdiv_divide;

/**
 * Accumulator width options for the Fractional-N divider.
 *
 * @private
 */
typedef enum
{
    /** 8 bit accumulator */
    CS4224_FRACDIV_ACCUM_WIDTH_8BIT = 0,

    /** 16 bit accumulator */
    CS4224_FRACDIV_ACCUM_WIDTH_16BIT = 1,

    /** 24 bit accumulator */
    CS4224_FRACDIV_ACCUM_WIDTH_24BIT = 2,

    /** 32 bit accumulator */
    CS4224_FRACDIV_ACCUM_WIDTH_32BIT = 3,

}e_cs4224_fracdiv_accumulator_width;

/**
 * The clock monitor clock source divider
 */
typedef enum
{
    /** Divide by 1 */
    CS4224_CLK_MON_DIV1  = 0,

    /** Divide by 4 */
    CS4224_CLK_MON_DIV4  = 1,

    /** Divide by 8 */
    CS4224_CLK_MON_DIV8  = 2,

    /** Divide by 16 */
    CS4224_CLK_MON_DIV16 = 3,

}e_cs4224_clk_mon_clksel_div;

/**
 * The per-port (pp) clock monitor clock source.
 */
typedef enum
{
    /** Clock source is SRX_CLK */
    CS4224_CLK_MON_PP_SRX    = 0x00 << 2,

    /** Clock source is STX_CLK */
    CS4224_CLK_MON_PP_STX    = 0x01 << 2,

    /** Clock source is SRX_CLK_LD */
    CS4224_CLK_MON_PP_CLK_LD = 0x02 << 2,

    /** Clock source is SRX_REF_LD */
    CS4224_CLK_MON_PP_REF_LD = 0x03 << 2,  

    /** Clock source is SRX_FAST_CLK */
    CS4224_CLK_MON_PP_FAST   = 0x04 << 2,   

    /** Clock source is SRX_DIV32_OUT */
    CS4224_CLK_MON_PP_DIV32  = 0x05 << 2,

    /** Clock source is SRX_DIV80_OUT */
    CS4224_CLK_MON_PP_DIV80  = 0x06 << 2,

    /** Clock source is SRX_DIV64_OUT */
    CS4224_CLK_MON_PP_DIV64  = 0x07 << 2,

    /** Clock source is SRX_MON_CLK */
    CS4224_CLK_MON_PP_MON    = 0x08 << 2,

    /** Clock source is no clock */
    CS4224_CLK_MON_PP_NO_CLK = 0x10 << 2,

    /** Clock source is reset dividers, output idle */
    CS4224_CLK_MON_PP_RESET  = 0x3f,

}e_cs4224_pp_clk_mon_clksel_src;

/**
 * The global (gbl) clock monitor clock source.
 */
typedef enum
{
    /** Clock source is process monitor */
    CS4224_CLK_MON_GBL_PMON   = 0x00 << 2,

    /** Clock source is process monitor divided by 2 */
    CS4224_CLK_MON_GBL_PMOND2 = 0x01 << 2,

    /** Clock source is process monitor divided by 4 */
    CS4224_CLK_MON_GBL_PMOND4 = 0x02 << 2,

    /** Clock source is reference clock */
    CS4224_CLK_MON_GBL_REFCLK = 0x03 << 2,  

    /** Clock source is clockless VCO clock */
    CS4224_CLK_MON_GBL_VCO    = 0x04 << 2,   

    /** Clock source is GPIO1 */
    CS4224_CLK_MON_GBL_GPIO1  = 0x06 << 2,

    /** Clock source is GPIO2 */
    CS4224_CLK_MON_GBL_GPIO2  = 0x07 << 2,

    /** Clock source is port pair 0 egress selected clock */
    CS4224_CLK_MON_GBL_PP0EGR = 0x08 << 2,

    /** Clock source is port pair 0 ingress selected clock */
    CS4224_CLK_MON_GBL_PP0ING = 0x09 << 2,

    /** Clock source is port pair 1 egress selected clock */
    CS4224_CLK_MON_GBL_PP1EGR = 0x0a << 2,

    /** Clock source is port pair 1 ingress selected clock */
    CS4224_CLK_MON_GBL_PP1ING = 0x0b << 2,

    /** Clock source is port pair 2 egress selected clock */
    CS4224_CLK_MON_GBL_PP2EGR = 0x0c << 2,

    /** Clock source is port pair 2 ingress selected clock */
    CS4224_CLK_MON_GBL_PP2ING = 0x0d << 2,

    /** Clock source is port pair 3 egress selected clock */
    CS4224_CLK_MON_GBL_PP3EGR = 0x0e << 2,

    /** Clock source is port pair 3 ingress selected clock */
    CS4224_CLK_MON_GBL_PP3ING = 0x0f << 2,

    /** Clock source is no clock */
    CS4224_CLK_MON_GBL_NO_CLK = 0x10 << 2,

    /** Clock source is reset dividers, output idle */
    CS4224_CLK_MON_GBL_RESET  = 0x7f,

}e_cs4224_gbl_clk_mon_clksel_src;

/**
* Used to select the driver being configured
*/
typedef enum
{
    /** The host side transmitter */
    CS4224_TX_HOST_INTERFACE  = 0,

    /** The line side transmitter */
    CS4224_TX_LINE_INTERFACE  = 1,

    /** If in simplex mode this automatically
     * determines the appropriate transmitter for the channel */
    CS4224_TX_SIMPLEX_INTERFACE = 2,

}e_cs4224_tx_driver_interface;

/**
 * PCS Monitor Status
 */
typedef struct cs4224_pcs_monitor_status_s
{
    /** PCS monitor reveiver state machine sync status */
    cs_boolean sync;

    /** PCS monitor is in a high bit error state (XGPCS only) */
    cs_uint32 high_ber;

}cs4224_pcs_monitor_status_t;

/**
 * FEC Statistics
 */
typedef struct cs4224_fec_stats_s
{
    /** Number of FEC blocks transmitted (encoded) by the line side Tx module */
    cs_uint32 tx_blk_total;

    /** Number of FEC blocks received (decoded) by the line side Rx module */
    cs_uint32 rx_blk_total;

    /** Number of FEC blocks successfully corrected by the Rx module */
    cs_uint32 rx_blk_corr;

    /** Number of FEC blocks unable to be corrected by the Rx module */
    cs_uint32 rx_blk_uncorr;

    /** Number of zero bit errors detected by the Rx module */
    cs_uint32 rx_zero_errs;

    /** Number of one bit errors detected by the Rx module */
    cs_uint32 rx_one_errs;

}cs4224_fec_stats_t;

/**
 * Configuration for the per-port (pp) clock monitor
 * 
 * Use cs4224_pp_clk_mon_cfg_init() to initialize
 */
typedef struct cs4224_pp_clk_mon_cfg_s
{
    /** Select the clock source divider */
    e_cs4224_clk_mon_clksel_div clksel_divider;

    /** Select the clock source */
    e_cs4224_pp_clk_mon_clksel_src clksel_src;

    /** Start counting flag */
    cs_boolean go;

    /** Enable counter to free run  */
    cs_boolean free_run;

    /** Time duration to count */
    cs_uint16  duration;

    /** minimum count threshold */
    cs_uint32  min_thresh;

    /** maximum count threshold */
    cs_uint32  max_thresh;

}cs4224_pp_clk_mon_cfg_t;

/**
 * Configuration for the global (gbl) clock monitor
 */
typedef struct cs4224_gbl_clk_mon_cfg_s
{
    /** Select the clock source divider */
    e_cs4224_clk_mon_clksel_div clksel_divider;

    /** Select the clock source */
    e_cs4224_gbl_clk_mon_clksel_src clksel_src;

    /** Start counting flag */
    cs_boolean go;

    /** Enable counter to free run  */
    cs_boolean free_run;

    /** Time duration to count */
    cs_uint16  duration;

    /** minimum count threshold, 32 bits */
    cs_uint16  min_thresh_0;
    cs_uint16  min_thresh_1;

    /** maximum count threshold, 32 bits */
    cs_uint16  max_thresh_0;
    cs_uint16  max_thresh_1;

}cs4224_gbl_clk_mon_cfg_t;

/**
 * This structure is used to store state information
 * that is used when enabling a 2x2 or broadcast
 * switch.  
 */
typedef struct
{
    /** The state of the line SRX0_RX_CLKOUT_CTRL register */
    cs_uint16 line_srx0_rx_clkout_ctrl;

    /** The state of the host SRX0_RX_CLKOUT_CTRL register */
    cs_uint16 host_srx0_rx_clkout_ctrl;

    /** The state of the line COMMON_STX0_MISC register */
    cs_uint16 line_stx0_misc;

    /** The state of the host COMMON_STX0_MISC register */
    cs_uint16 host_stx0_misc;

    /** The state of the line DSP_MSEQ_POWER_DOWN_LSB register */
    cs_uint16 line_mseq_power_down;

    /** The state of the host DSP_MSEQ_POWER_DOWN_LSB register */
    cs_uint16 host_mseq_power_down;

    /** The state of the line STX0_DRIVER_CONFIG register */
    cs_uint16 line_stx0_driver_config;

    /** The state of the host STX0_DRIVER_CONFIG register */
    cs_uint16 host_stx0_driver_config;

    /** The state of the line DSP_MSEQ_MAIL_SEL register */
    cs_uint16 line_dsp_mseq_mail_sel;

    /** The state of the host DSP_MSEQ_MAIL_SEL register */
    cs_uint16 host_dsp_mseq_mail_sel;

    /** The state of the host SRX0_VCO_CONFIG register */
    cs_uint16 host_srx0_vco_config;

    /** The state of the host SRX0_AGC_CONFIG1 register */
    cs_uint16 host_srx0_agc_config1;

    /** The state of the host SRX0_DFE_CONFIG register */
    cs_uint16 host_srx0_dfe_config;

    /** The state of the line SRX0_RX_CONFIG register */
    cs_uint16 line_srx0_rx_config;

    /** The state of the host SRX0_RX_CONFIG register */
    cs_uint16 host_srx0_rx_config;

    /** The state of the line MSEQ_SPARE12_LSB register */
    cs_uint16 line_spare12_lsb;

    /** The state of the host MSEQ_SPARE12_LSB register */
    cs_uint16 host_spare12_lsb;

    /** The state of the line MSEQ_SPARE26_LSB register */
    cs_uint16 line_spare26_lsb;

}cs4224_switch_state_t;

/**
 * This structure is used to maintain device state when
 * managing the duplex 2x2 protection switching. It must
 * be initialized when the switch is first initialized and
 * then gets passed to consecutive API calls to manage
 * the h/w state.
 */
typedef struct
{
    /**
     * This flag is used to determine whether or not the
     * switch state has been initialized. Before activating the
     * switch for the first time this variable should be set to
     * FALSE to ensure that the API knows that the state
     * should be stored.
     */
    cs_boolean initialized;

    /**
     * This flag is managed by the user to setup the switch in low
     * latency mode. This bypasses the digital path through
     * the chip and achieves the lowest possible latency. However
     * it cannot be used when KR-AN or FC-AN are enabled
     */
    cs_boolean low_latency_switching;

    /**
     * State space used to manage the two slices of
     * the switch pair. This is managed automatically
     * by the API.
     */
    cs4224_switch_state_t slices[2];
    
    /**
     * For duplex switching on dual-die devices, write the switch config to both
     * dies at the same time.
     * 
     * Ignored for simplex switching. Only supported in superfast methods for now.
     * Only relevant for the 8-port duplex devices.
     * 
     * For which pairs will be set at the same time:
     * 
     * @{table,
     * -h Slice pairs that will be set when broadcast is enabled
     * -s Die 1 ! Die 0
     * - 0/1 ! 6/7
     * - 2/3 ! 4/5
     * }
     */
    cs_boolean broadcast;

}cs4224_switch_pair_state_t;

/**
 * Configuration for the SyncE application
 */
typedef struct cs4224_synce_config_s
{
    /** Enable SyncE */
    cs_boolean enable;

    /** The core fracN integer divisor */
    cs_uint16 divisor;

    /** The core fracN numerator */
    cs_uint32 numerator;

    /** The clock divider */
    e_cs4224_clk_mon_clksel_div clk_divider;

    /** the GPIO to ouput the SyncE signal, range GPIO1 thru GPIO4 and GPIO_INTERR */
    cs_uint8  gpio;

    /** the direction, line Rx or host Rx */
    e_cs4224_datapath_dir_t dir;

}cs4224_rules_synce_config_t;

/**
 * Configuration for the CDR frac-N's in the
 * Rx and Tx interfaces
 */
typedef struct cs4224_fracdiv_config_s
{
    /** Enable the fracN block */
    cs_boolean enable;

    /** The integer divisor, 8 bits wide */
    cs_uint16 divisor;

    /** The numerator, 24bits wide */
    cs_uint32 numerator;

}cs4224_rules_fracdiv_config_t;

/** Configuration rules for trace loss on the
 *  Rx or Tx interfaces */
typedef struct
{
    /** Trace loss setting */
    e_cs4224_trace_loss traceloss;

}cs4224_rules_intf_t;

/** Advanced rules for diagnostics and debug features.
 * 
 * Note: Leave these as their defaults unless instructed to by a Cortina AE
 * 
 * @private
 */
typedef struct
{
    cs_boolean phsel_bypass;
    
    cs_uint16 phsel_bypass_value;

}cs4224_advanced_rules_t;

/** Rx interface rules for device initialization */
typedef struct
{
    /** The EDC mode for the Rx interface (simplex only) */
    e_cs4224_edc_mode splx_edc_mode;
    
    /** The line EDC mode for the Rx interface (duplex only) */
    e_cs4224_edc_mode dplx_line_edc_mode;

    /** The host EDC mode for the Rx interface (duplex only) */
    e_cs4224_edc_mode dplx_host_edc_mode;

    /** Rx equalization rules (simplex only) */
    cs4224_rules_intf_t splx_eq;

    /** Rx line equalization rules (duplex only) */
    cs4224_rules_intf_t dplx_line_eq;

    /** Rx host equalization rules (duplex only) */
    cs4224_rules_intf_t dplx_host_eq;

}cs4224_rules_rx_if_t;

/** Tx interface rules for device initialization */
typedef struct
{ 
    /** Disable the transmitter (simplex only) */
    cs_boolean splx_disable;

    /** Disable the line transmitter (duplex only) */
    cs_boolean dplx_line_disable;

    /** Disable the host transmitter (duplex only) */
    cs_boolean dplx_host_disable;

    /** Tx driver initialization rules (simplex only) */
    cs4224_rules_intf_t splx_driver;

    /** Tx line driver initialization rules (duplex only) */
    cs4224_rules_intf_t dplx_line_driver;

    /** Tx host driver initialization rules (duplex only) */
    cs4224_rules_intf_t dplx_host_driver;

    /** power-down flag */
    cs_boolean power_down;

}cs4224_rules_tx_if_t;

/** Divider values for controlling CLKDIV_CTRL register */
typedef struct
{
    /** Enable the manual CLKDIV_CTRL register updates */
    cs_boolean enable;

    /** The SRX_RDIV_SEL divider */
    cs_uint16 rdiv;

    /** The SRX_DDIV_SEL divider */
    cs_uint16 ddiv;

    /** The SRX_FASTDIV_SEL divider */
    cs_uint16 fastdiv;

}cs4224_rules_div_config_t;

/**
 * This structure is used to manage callback functions
 * required during the initialization process.
 */
typedef struct
{
    /** Pointer to user data passed back to the callback */
    void* user_data;

    /** Pointer to the callback method */
    void* fcn_ptr;

}cs4224_callback_t;

/** KR-AN specific rules */
typedef struct
{
    /** The advertised supported data-rates, see e_cs4224_kran_data_rates_t */
    cs_uint32 data_rates;

    /** FEC ability flag (F0) */
    cs_boolean fec_ability_f0;

    /** FEC requested flag (F1) */
    cs_boolean fec_requested_f1;

    /** Pause ability flag (C0) */
    cs_boolean pause_ability_c0;

    /** Pause ability flag (C1) */
    cs_boolean pause_ability_c1;

    /** Remote Fault flag (D13) */
    cs_boolean remote_fault_d13;

    /** Training enable flag */
    cs_boolean allow_training;

    /** Causes enter_operational_state methods to wait for KR-AN to complete
     *  before returning
     */
    cs_boolean wait_for_an_done;

    /** A callback method */
    cs4224_callback_t poll_kran_callback;

    /** Internal advanced settings */
    cs4224_kran_advanced_config_t_s advanced;
    
}cs4224_rules_kran_t;

/**
 * FC-AN data rate capabilities
 * 
 * Uses single bits, so they can be OR'd together
 */
typedef enum
{

  /** 1 GE */
  CS4224_FCAN_DATA_RATE_1G = 0x1,

  /** 2 GE */
  CS4224_FCAN_DATA_RATE_2G = 0X2,

  /** 4 GE */
  CS4224_FCAN_DATA_RATE_4G = 0x4,

  /** 8 GE */
  CS4224_FCAN_DATA_RATE_8G = 0x8,

  /** 16 GE */
  CS4224_FCAN_DATA_RATE_16G = 0x10,

  /**
   * Disabled - the default value for the negotiated_rate
   * variable to prevent compilation problems */
  CS4224_FCAN_DATA_RATE_DISABLED = 0,

} e_cs4224_fcan_data_rate_t;

/**
 * FC-AN data rates, able to hold multiple rates by OR'ing fcan_data_rate_t
 * enums together.
 * 
 * @example
 * 
 * cs4224_fcan_data_rates_t rates = CS4224_FCAN_DATA_RATE_1G |
 *                                  CS4224_FCAN_DATA_RATE_2G |
 *                                  CS4224_FCAN_DATA_RATE_4G;
 */
typedef cs_uint16 cs4224_fcan_data_rates_t;

/**
 * FC-AN data rate negotiation complete
 */
typedef enum
{
  /** FC-AN negotiated a data rate */
  CS4224_FCAN_AN_DONE = 0x1,

  /** FC-AN could not negotiate a data rate */
  CS4224_FCAN_AN_NOT_DONE = 0x2,

} e_cs4224_fcan_an_status_t;

/**
 * FC-AN VCO mode
 * 
 * Note: 16G needs to run in PD mode, all others can run in PFD
 */
typedef enum
{
    /** VCO in PD mode (through-timing) */
    CS4224_FCAN_VCO_PD_MODE = 0,

    /** VCO in PFD mode (local-timing) */
    CS4224_FCAN_VCO_PFD_MODE,

} e_cs4224_fcan_vco_mode_t;

/**
 * FC-AN advanced rules for debugging
 * 
 * Note: Do not touch these unless instructed to do so by a Cortina AE
 * 
 * @private
 */
typedef struct
{
    /** Set Main tap to Maximum */
    cs_boolean enable_tp_main_tap_to_max;

    /** Turn on adaptive post */
    cs_boolean enable_tp_adaptive_post;

    /** Send Preset at the begining of training */
    cs_boolean enable_tp_send_preset;

    /** Send Init at the begining of training */
    cs_boolean enable_tp_send_init;

    /** Force local_rx_ready initial condition */
    cs_boolean enable_tp_frc_lcl_rx_ready;

    /** lp_status inital condition */
    cs_boolean enable_tp_lp_status;

    /** Send hold inital condition */
    cs_boolean enable_tp_send_hold;

    /** SWC in limits initial condition */
    cs_boolean enable_tp_swc_in_limits;

    /** Jump to cal.asm during train_taps if LOL */
    cs_boolean enable_jmp_to_caldotasm;

    /** Disable FCAN power-savings */
    cs_boolean disable_fc_power_savings;

    /** When disabled, PCS sync not monitored on line side, for testing only */
    cs_boolean disable_pcs_checks;

    /** Enable training to work in the lab with no loss cables */
    cs_boolean enable_no_loss;

    /** Start AN immediately after pre-AN config, default is TRUE */
    cs_boolean start_an;

}cs4224_fcan_advanced_t;

/** 
 * FC-AN specific rules
 */
typedef struct
{
    /** The advertised supported data-rates */
    cs4224_fcan_data_rates_t data_rates;

    /** A callback method to poll for FC-AN complete status */
    cs4224_callback_t poll_fcan_callback;

    /** 
     * Wait for FC-AN to complete flag
     * When set, enter_op_state will wait until FC-AN is complete
     */
    cs_boolean wait_for_an_done;

    /** Rate negotiated on the link after wait_for_an_done is completed */
    e_cs4224_fcan_data_rate_t negotiated_rate;

    /** Advertise Speed Negotiation support */
    cs_boolean speed_negotiation_support;

    /** Advertise Training Protocol support */
    cs_boolean training_protocol_support;

    /** Advertise we are FEC capable */
    cs_boolean fec_capable;

    /** Advertise that we are requesting FEC */
    cs_boolean fec_request;

    /** Advertise that we will not change Tx settings during training */
    cs_boolean transmitter_fixed;

    /** Advanced setup rules */
    cs4224_fcan_advanced_t advanced;

}cs4224_rules_fcan_t;

/**
 * This structure defines the rules used to initialize the
 * device. These rules serve as the high-level configuration
 * switches that a user can adjust to control the behavior
 * of the device(s).
 */
typedef struct cs4224_rules_s
{
    /** Application */
    e_cs4224_target_application application;

    /** Rx rule settings */
    cs4224_rules_rx_if_t rx_if;

    /** Tx rule settings */
    cs4224_rules_tx_if_t tx_if;

    /** FC-AN rule settings */
    cs4224_rules_fcan_t fcan;

    /** KR-AN rule settings */
    cs4224_rules_kran_t kran;

    /** The frequency of the reference clock in kHz */
    int ref_clk_rate;

    /** The configuration for the rdiv/ddiv dividers */
    cs4224_rules_div_config_t clkdiv;

    /** The configuration for the fractional divider */
    cs4224_rules_fracdiv_config_t fracdiv;

    /** The configuration for the SyncE application */
    cs4224_rules_synce_config_t synce;

    /** Flag to un-squelch the driver */
    cs_boolean unsquelch_driver;

    /** Enable or disable TX auto squelch */
    cs_boolean tx_auto_squelch;
 
    /** Flag to enable the internal AC decoupling caps */
    cs_boolean enable_ac_decoupling_caps;

    /** Show debugging info when initializing the device */
    cs_boolean show_debug_info;

    /** Flag to enable power savings mode */
    cs_boolean enable_power_savings;

    /** Flag to enable the FEC, duplex only */
    cs_boolean enable_fec;

    /** Flag to enable polarity inversion */
    e_cs4224_polarity_inv_t polarity_inv;

    /** Flag to enable ucode downloading to both dies at once */
    cs_boolean enable_die_broadcast;

#if defined(CS_HAS_FILESYSTEM)
    /**
     * This rule can be used to load the KR microcode from a file
     * instead of inlining it. This is useful on systems where
     * there is not enough memory to inline the microcode but
     * a file system is available. This rule will be defaulted
     * to NULL by cs4224_rules_set_default and the inlined image
     * will be used.
     */ 
    const char* ucode_kr_path;

    /**
     * This rule can be used to load the FC microcode from a file
     * instead of inlining it. This is useful on systems where
     * there is not enough memory to inline the microcode but
     * a file system is available. This rule will be defaulted
     * to NULL by cs4224_rules_set_default and the inlined image
     * will be used.
     */ 
    const char* ucode_fc_path;
#endif /* CS_HAS_FILESYSTEM */

    /** Advanced rules for debug/diagnostics */
    cs4224_advanced_rules_t advanced;
    
    /** Flag to allow the microsequencer to perform a dynamic reconfig */
    cs_boolean mseq_dyn_reconfig;

}cs4224_rules_t;

/** VCO lock status */
typedef struct
{
    /** The lock status of the line Rx VCO */
    cs_boolean rx_line_lock;

    /** The lock status of the host Rx VCO */
    cs_boolean rx_host_lock;

}cs4224_vco_lock_status_t;

/** Struct to define a interface, used for methods that operate on multiple
 *   interfaces at a time (cs4224_wait_for_links_ready, etc) */
typedef struct
{
    /** slice of the device */
    cs_uint32 slice;
    
    /** mseq interface of the device
     *  Simplex: CS4224_SPLX_MSEQ
     *  Duplex: CS4224_DPLX_LINE_MSEQ or CS4224_DPLX_HOST_MSEQ
     */
    e_cs4224_mseq_id  mseq_id;

}cs4224_interface_t;

/**
 * This structure is used to report the status of
 * an RX link.
 */
typedef struct
{
    /** The slice of the device to query the link status */
    cs_uint32 slice;

    /** The microsequencer associated with the interface */
    e_cs4224_mseq_id mseq_id;

    /** TRUE if the link is up, FALSE if it is down */
    cs_boolean link_up;

    /** TRUE if the SERDES is locked, FALSE if it is down */
    cs_boolean rxlock;

    /** TRUE if the force lock bit is asserted, FALSE otherwise */
    cs_boolean force_lock;
    
    /** TRUE if the microsequencer is reporting EDC convergence, FALSE otherwise */ 
    cs_boolean edc_converged; 

    /** TRUE if the 10G PCS is enabled (only on the line interface) */
    cs_boolean xgpcs_enabled;

    /** TRUE if the 8G PCS is enabled (only on the line interface) */
    cs_boolean egpcs_enabled;

    /** TRUE if the 1G PCS is enabled (only on the line interface) */
    cs_boolean gigepcs_enabled;

    /** TRUE if the 10G PCS is synced (only on the line interface) */
    cs_boolean xgpcs_synced;

    /** TRUE if the 10G PCS is detecting high bit errors */
    cs_boolean xgpcs_high_ber;

    /** TRUE if the 8G PCS is synced (only on the line interface) */
    cs_boolean egpcs_synced;

    /** TRUE if the 1G PCS is synced (only on the line interface) */
    cs_boolean gigepcs_synced;

}cs4224_interface_status_t;

/*============================================================
 *
 * API Callback Methods
 *
 *============================================================*/
typedef cs_status (*cs4224_callback_lock)(cs_uint32 slice);
typedef cs_status (*cs4224_callback_unlock)(cs_uint32 slice);

/* Callback methods for implementing h/w locking */
void cs4224_set_callback_for_lock(cs4224_callback_lock callback);
void cs4224_set_callback_for_unlock(cs4224_callback_unlock callback);

/* Callback type definitions for KRAN and FCAN */
                                                        /*  slice  ,        results           , user data */
typedef e_cs4224_fcan_an_status_t (*CS4224_FCAN_CALLBACK)(cs_uint32,e_cs4224_fcan_data_rate_t*,void*);
typedef e_cs4224_kran_an_status_t (*CS4224_KRAN_CALLBACK)(cs_uint32,cs4224_kran_results_t_s*  ,void*);

/*============================================================
 *
 * API Utility Methods
 *
 *============================================================*/

/* Reset the entire static state of the API (deprecated) */
void cs4224_reset_static_state(void);

/* Reset the entire static state of the API (by device) */
void cs4224_reset_die_static_state(cs_uint32 die);

/* This method returns the die number associated with the slice */
cs_uint32 cs4224_get_die_from_slice(
    cs_uint32 slice);

/**
 * This method provides access a particular channel or port of
 * the device (simplex or duplex). This method should be used
 * instead of underlying cs4224_reg_get() method as it automatically handles
 * mapping of a channel/slice to the appropriate register in
 * the address map.
 *
 * *Port Pair Registers:*
 * ======================
 * When accessing a register in the port pair space such as
 * PP_LINE_SDS_COMMON_RX0_Config this method automatically
 * selects the appropriate die and port-pair instance of the register
 * based on the channel/slice.
 *
 * *Global Registers:*
 * ===================
 * When accessing a global register it automatically selects the
 * appropriate die of the package that the channel belongs to.
 *
 * @param slice [I] - The slice of the device being accessed. The
 *                    least significant bits represent the port 
 *                    or channel being accessed (simplex or duplex).
 *                    The upper bits are passed through to the lower
 *                    level APIs to access multiple devices.
 * @param addr  [I] - The address of the device to access.
 * @param data  [O] - The data returned from the register read.
 *
 * @return CS_OK on success, CS_ERROR on failure.
 *
 * @example
 *
 * cs_uint16 reg_data = 0;
 *
 * // Read the RX0_Config register for slice/channel 2. Internally
 * // the API will select the correct port pair instance on the
 * // correct die of the package for this channel.
 * cs4224_reg_get_channel(2, CS4224_PP_LINE_SDS_COMMON_RX0_Config, &reg_data);
 *
 * // When reading a global register from slice/channel 2 this method
 * // automatically selects the appropriate die of the package.
 * cs4224_reg_get_channel(2, CS4224_GLOBAL_CHIP_ID_LSB, &reg_data);
 *
 */
cs_status cs4224_reg_get_channel(
    cs_uint32 slice,
    cs_uint32 addr,
    cs_uint16* data);

/* Wrapper around cs4224_reg_get_channel to ignore status and return register value */
cs_uint16 cs4224_reg_read(
    cs_uint32 slice,
    cs_uint32 addr);
    
/* Set a register value for the particular port/channel/slice */
cs_status cs4224_reg_set_channel(
    cs_uint32 channel, 
    cs_uint32 addr, 
    cs_uint16 data);

/* Set a register value and check the register contents */
cs_status cs4224_reg_set_chk_channel(
    cs_uint32 channel, 
    cs_uint32 addr, 
    cs_uint16 data);

/* Read from a register */
cs_status cs4224_reg_get(
    cs_uint32  die, 
    cs_uint32  addr, 
    cs_uint16* data);

/* Write to a register */
cs_status cs4224_reg_set(
    cs_uint32 die, 
    cs_uint32 addr, 
    cs_uint16 data);

/* Lock the device - for threaded programming */
cs_status cs4224_lock(
    cs_uint32 slice);

/* Unlock the device - for threaded programming */
cs_status cs4224_unlock(
    cs_uint32 slice);

/* Returns the HW_ID or SKU of the device */
e_cs4224_hardware_id cs4224_hw_id(
    cs_uint32 slice);

/* Returns True is ASIC is cs4224 simplex device, False otherwise */
cs_boolean cs4224_is_hw_simplex(cs_uint32 slice);

/* Returns True is ASIC is cs4343 duplex device, False otherwise */
cs_boolean cs4224_is_hw_duplex(cs_uint32 slice);

/* Reads the current fixed-point value of the temperature monitor (fixed-point math)  */
cs_status cs4224_mon_temp_read_fixp(
    cs_uint32  slice,
    cs_uint32* temp);

/* Reads the current fixed-point value of the voltage monitor (fixed-point math) */
cs_status cs4224_mon_volt_read_fixp(
    cs_uint32               slice,
    e_cs4224_mon_vlt_supply volt_source,
    cs_uint32*              voltage);

/* This method is called to adjust the register address to point
 * to the correct port pair (pp) register instance. */
cs_uint32 cs4224_adj_pp(
    cs_uint32 slice, 
    cs_uint16 addr);

/* This method is called to configure the global timer (fixed-point math)*/
cs_status cs4224_init_global_timer_fixp(
    cs_uint32 slice, 
    cs_uint32 ref_clk_freq);

/* This method is called to adjust the register address to point
 * to the correct MSEQ program store register instance. */
cs_uint32 cs4224_adj_mseq(
    cs_uint32 slice, 
    cs_uint16 addr);

/* Simplex mate slice */
cs_status cs4224_simplex_mate_slice(
    cs_uint32 slice,
    cs_uint32 *mate);

/* This method is used to identify the direction of the simplex data-path. */
cs_boolean cs4224_line_rx_to_host_tx_dir(
    cs_uint32 slice);

/* Output the version info */
cs_status cs4224_version(
    char* buffer, 
    cs_uint32 buffer_len);

/* Re-sync the elastic store */
cs_status cs4224_resync_elsto(
    cs_uint32 slice, 
    cs_uint16 elsto_reg);

/* re-sync the Rx VCOs */
cs_status cs4224_init_vco(
    cs_uint32 slice, 
    cs_uint32 addr);

/* change the rate of a specific interface (don't use this unless told to) */
cs_status cs4224_slice_change_rate_intf(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t intf,
    cs4224_rules_t* rules);

/* Setup the default rules for an application type */
cs_status cs4224_rules_set_default(
    e_cs4224_target_application application,
    cs4224_rules_t*             rules);

/* Put the device into kran operational state */
cs_status cs4224_slice_enter_operational_state_kran(
    cs_uint32 base_slice, 
    cs4224_rules_t* rules);

/* Put the device into kran 40G operational state */
cs_status cs4224_slice_enter_operational_state_kran_40g(
    cs_uint32 base_slice, 
    cs4224_rules_t* rules);

/* Put the device into operational state */
cs_status cs4224_slice_enter_operational_state(
    cs_uint32 slice, 
    cs4224_rules_t* rules);

/* Get the mseq id */
cs_status cs4224_get_mseq_id(
    cs_uint32         slice, 
    e_cs4224_mseq_id*  mseq_id);

/* Determine the address offset for the mseq registers */
cs_uint32 cs4224_mseq_get_addr_offset(
    cs_uint32 slice,
    e_cs4224_mseq_id mseq);

/* Get the cfg side */
cs_status cs4224_get_cfg_side(
    cs_uint32              slice,
    e_cs4224_cfg_sides_t*  side);

/* Fine tune the delay required when stalling the microsequencer */
void cs4224_mseq_stall_set_delay(int us);
int cs4224_mseq_stall_get_delay(void);

/* Stall/un-stall the microsequencer */
cs_status cs4224_mseq_stall(
    cs_uint32         slice, 
    e_cs4224_mseq_id  mseq_id,
    cs_boolean        stall);

/* Stall/un-stall multiple microsequencers */
cs_status cs4224_mseqs_stall(
    cs4224_interface_t interfaces[],
    cs_uint16          length,
    cs_boolean         stall);

/* Enable/disable power savings on the microsequencer */
cs_status cs4224_mseq_enable_power_savings(
    cs_uint32         slice, 
    e_cs4224_mseq_id  mseq_id,
    cs_boolean        enable);

/* Configures the Tx driver settings */
cs_status cs4224_init_driver_settings(
    cs_uint32                    slice,
    e_cs4224_tx_driver_interface intf,
    cs_uint16                    ctrla,
    cs_uint16                    ctrlb);

/* Low-level polarity inversion API, use cs4224_enable_polarity_inv instead */
cs_status cs4224_config_polarity_inv_points(
    cs_uint32 slice,
    cs_uint16 inv_points);

/* Configure polarity inversion */
cs_status cs4224_enable_polarity_inv(
    cs_uint32 slice,
    cs4224_rules_t* rules);

/* Get polarity inversion configuration */   
cs_status cs4224_query_polarity_inv(
    cs_uint32 slice,
    cs_uint16 *inv_points);
 
/* Dump polarity inversion configuration */
cs_status cs4224_debug_dump_polarity_inv(
    cs_uint32 slice);

/* Force init switch state */
cs_status cs4224_switch_duplex_force_init_state(
    cs_uint32                   slice,
    cs4224_switch_pair_state_t* state);

/* Reset the analog elastic stores of a switch pair */
cs_status cs4224_switch_reset_analog_elsto(
    cs_uint32 slice);

/* Configure the duplex switch */
cs_status cs4224_switch_duplex(
    cs_uint32                   slice,
    e_cs4224_switch_action_t    switch_action,
    cs4224_switch_pair_state_t* switch_state);

/* Faster switch with many more caveats */
cs_status cs4224_switch_duplex_superfast(
    cs_uint32                   slice,
    e_cs4224_switch_action_t    switch_action,
    cs4224_switch_pair_state_t* switch_state,
    cs_boolean                  die_broadcast);

/* Used for pre-initializing a switch pair before running cs4224_switch_duplex_superfast */
cs_status cs4224_switch_duplex_superfast_init(
    cs_uint32                   slice,
    cs4224_switch_pair_state_t* switch_state,
    cs_boolean                  die_broadcast);

/* Query the current switch configuration */
cs_status cs4224_switch_query_mode(
    cs_uint32                 slice,
    e_cs4224_switch_action_t* switch_mode,
    cs_boolean*               low_latency_mode);

/* Configure the simplex protection switch */
cs_status cs4224_switch_simplex(
    cs_uint32                   slice,
    e_cs4224_switch_action_t    switch_action,
    cs4224_switch_pair_state_t* switch_state);

/* Configure the digital protection switch */
cs_status cs4224_switch_digital(
    cs_uint32 slice,
    e_cs4224_switch_action_t switch_action);

/* Configure the analog protection switch */
cs_status cs4224_switch_analog(
    cs_uint32 slice,
    e_cs4224_switch_action_t switch_action);

/* Initialize a cs4224_pp_clk_mon_cfg_t struct */
void cs4224_pp_clk_mon_cfg_init(
    cs4224_pp_clk_mon_cfg_t *clk_mon_cfg);

/* Configure the per-port (pp) clock monitor */
cs_status cs4224_pp_clock_monitor(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t side,
    cs4224_pp_clk_mon_cfg_t *clk_mon_cfg);

/* Get the frequency of a desired pp clock (fixed-point math)*/
cs_status cs4224_pp_clock_monitor_freq_fixp(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t side,
    cs4224_pp_clk_mon_cfg_t *pp_clk_mon_cfg,
    cs_uint32 ref_clk_rate,
    cs_uint16 *freq);

/* Configure the global (gbl) clock monitor */
cs_status cs4224_gbl_clock_monitor(
    cs_uint32 slice,
    cs4224_gbl_clk_mon_cfg_t *clk_mon_cfg);

/* Configure the Core Fractional Divider */
cs_status cs4224_fracdiv_core_init(
    cs_uint32               slice,
    e_cs4224_datapath_dir_t dir,    
    cs_uint16               divisor,
    cs_uint32               numerator);

/* Configure the CDR Fractional Divider */
cs_status cs4224_fracdiv_cdr_init(
    cs_uint32               slice,
    e_cs4224_datapath_dir_t dir,    
    cs_uint16               divisor,
    cs_uint32               numerator);

/* Get the FEC stats */
cs_status cs4224_get_fec_stats(
    cs_uint32 slice, 
    cs4224_fec_stats_t* fec_stats);

/* Soft reset a slice and apply per-port workarounds */
cs_status cs4224_slice_soft_reset(
    cs_uint32 slice);

cs_status cs4224_slice_soft_reset_intf(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t intf);

/* The method controls the automatic squelching of a Tx driver */
cs_status cs4224_mseq_squelch_ctrl(
    cs_uint32 slice,
    e_cs4224_datapath_dir_t dir, 
    cs_boolean enable);

/* This method sends a squelch/un-squelch request while using the
 * automatic squelch feature */
cs_status cs4224_send_squelch_request(
    cs_uint32 slice,
    e_cs4224_datapath_dir_t dir,
    cs_boolean squelch);

/* This method squelches/un-squelches a Tx driver */
cs_status cs4224_squelch_driver(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t side, 
    cs_boolean squelch);

/**
 * This method performs a hard reset of a single die. Normally it
 * is preferable to use the cs4224_hard_reset() method to reset
 * the entire ASIC.
 * 
 * Note: this method resets the cached API state via cs4224_reset_die_static_state
 * 
 * @param die [I] - The die of the device to reset.
 *
 * @return CS_OK on success, CS_ERROR on failure.
 *
 * @see cs4224_hard_reset()
 */
cs_status cs4224_hard_reset_die(
    cs_uint32 die);

/**
 * This method performs a hard reset on the K2 package. If there
 * are two dies in a package, both dies are hard reset. This method
 * accepts a slice parameter but it can be any slice on the ASIC. Only
 * the upper 24 bits of the slice parameter are really used in order
 * to address the appropriate ASIC in the scenario where multiple are
 * present in the same system.
 *
 * This method should not be called for every slice as it will reset
 * the ASIC each time. It should only be called once when the ASIC is
 * first powered on. It is required to properly reset all registers since
 * certain blocks may not be powered on when the external reset is
 * applied in order to conserve power.
 *
 * @param slice [I] - The slice of the device to reset.
 *
 * @return CS_OK on success, CS_ERROR on failure.
 *
 */
cs_status cs4224_hard_reset(
    cs_uint32 slice);

/* Print the microcode version info */
void cs4224_debug_ucode_show_version(
    cs_uint32 slice);

/* Power-up/power-down the mux */
cs_status cs4224_mux_enable(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t side, 
    cs_boolean enable);

/* Power-up/power-down the demux */
cs_status cs4224_demux_enable(
    cs_uint32 slice,
    e_cs4224_cfg_sides_t side, 
    cs_boolean enable);

cs_status cs4224_init_edc_mode_set(
    cs_uint32       slice,
    cs4224_rules_t* rules);

cs_status cs4224_init_edc_mode_intf(
    cs_uint32       slice,
    cs4224_rules_t* rules,
    e_cs4224_mseq_id intf);

cs_boolean cs4224_is_eeprom_finished(
    cs_uint32 die);

cs_status cs4224_wait_for_eeprom_finished(
    cs_uint32 die,
    cs_uint32 max_iterations,
    cs_uint32 ms_delay_between_iterations);

cs_status cs4224_restore_powered_down_regs(
    cs_uint32 slice); 

cs_status cs4224_enable_pcs_monitor(
    cs_uint32 slice, 
    e_cs4224_pcs_monitor_device pcs_device,
    cs_boolean enable);

cs_status cs4224_query_pcs_monitor_status(
    cs_uint32 slice, 
    e_cs4224_pcs_monitor_device pcs_device, 
    cs4224_pcs_monitor_status_t *pcs_status);

cs_status cs4224_dump_pcs_monitor_status(
    cs_uint32 slice, 
    e_cs4224_pcs_monitor_device pcs_device);

/* Power down an individual slice */
cs_status cs4224_slice_power_down(
    cs_uint32 slice);

/* Save the currently active EDC mode */
cs_status cs4224_save_edc_mode(
    cs_uint32          slice,
    cs_uint16          reg_offset,
    e_cs4224_edc_mode  edc_mode);

/* Query the currently active EDC mode */
cs_status cs4224_query_edc_mode(
    cs_uint32          slice,
    e_cs4224_mseq_id   mseq_id,
    e_cs4224_edc_mode* edc_mode);

/* Check to see if the microsequencer is stalled or not */
cs_status cs4224_query_mseq_is_stalled(
    cs_uint32         slice, 
    e_cs4224_mseq_id  mseq_id,
    cs_boolean*       stalled);

/* Check to see if we're in power savings mode or not */
cs_status cs4224_query_mseq_power_savings(
    cs_uint32         slice, 
    e_cs4224_mseq_id  mseq_id,
    cs_boolean*       enabled);

/* Check to see if the interface link is up and ready to pass traffic */
cs_status cs4224_query_link_ready(
    cs_uint32         slice,
    e_cs4224_mseq_id  mseq_id,
    cs_uint32         check_time,
    cs_boolean*       link_ready);

/* Wait at least timeout ms for link to come up */
cs_status cs4224_wait_for_link_ready(
    cs_uint32         slice,
    e_cs4224_mseq_id  mseq_id,
    cs_uint32         check_time,
    cs_uint32         timeout);

/* Similar to query_link_ready, but works on multiple slices at once */
cs_status cs4224_query_links_ready(
    cs4224_interface_t interfaces[],
    cs_uint16          length,
    cs_uint32          check_time,
    cs_uint16*         interface_fault,
    cs_boolean*        links_ready);

/* Similar to wait_for_link_ready, but works on multiple slices at once */
cs_status cs4224_wait_for_links_ready(
    cs4224_interface_t interfaces[],
    cs_uint16          length,
    cs_uint32          check_time,
    cs_uint32          timeout);

/* Similar to query_links_ready, but reports the status of all links */
cs_status cs4224_query_links_status(
    cs4224_interface_t        interfaces[],
    cs_uint16                 length,
    cs_uint32                 check_time,
    cs4224_interface_status_t links_status[]);

/* Translate the specified EDC mode */
const char* cs4224_translate_edc_mode(e_cs4224_edc_mode edc_mode);

/* Translate the specified application mode */
const char* cs4224_translate_app_mode(e_cs4224_target_application app_mode);

/* Translate the switch state */
const char* cs4224_switch_translate_state(e_cs4224_switch_action_t state);

/* Translate the cfg side */
const char* cs4224_translate_cfg_side(e_cs4224_cfg_sides_t side);

/* Check the validity of the rules struct */
cs_status cs4224_check_rules(cs_uint32 slice, cs4224_rules_t *rules);

/* Returns the approx data rate (only use for debug info) */
cs_uint32 cs4224_query_data_rate(
    const cs4224_rules_t* rules);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* __CS4224_H_ */

/** @file cs4224_api.h
 ******************************************************************************
 *
 * @brief
 *    This module contains the register definitions for the device.
 *
 ******************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 *****************************************************************************/
#ifndef __CS4224_REGISTERS_H__
#define __CS4224_REGISTERS_H__

#define CS4224_GLOBAL_CHIP_ID_LSB                 0x0
#define CS4224_GLOBAL_CHIP_ID_LSB_dft             0x3E5
#define CS4224_GLOBAL_CHIP_ID_MSB                 0x1
#define CS4224_GLOBAL_CHIP_ID_MSB_dft             0x7003
#define CS4224_GLOBAL_SCRATCH                     0x2
#define CS4224_GLOBAL_SCRATCH_dft                 0x0
#define CS4224_GLOBAL_UCODE_VERSION_SR            0x3
#define CS4224_GLOBAL_UCODE_VERSION_SR_dft        0x0
#define CS4224_GLOBAL_UCODE_VERSION_CX1           0x4
#define CS4224_GLOBAL_UCODE_VERSION_CX1_dft       0x0
#define CS4224_GLOBAL_UCODE_VERSION_KR            0x5
#define CS4224_GLOBAL_UCODE_VERSION_KR_dft        0x0
#define CS4224_GLOBAL_UCODE_VERSION_ZR            0x6
#define CS4224_GLOBAL_UCODE_VERSION_ZR_dft        0x0
#define CS4224_GLOBAL_UCODE_VERSION_FC            0x7
#define CS4224_GLOBAL_UCODE_VERSION_FC_dft        0x0
#define CS4224_GLOBAL_UCODE_TIMESTAMP0            0x8
#define CS4224_GLOBAL_UCODE_TIMESTAMP0_dft        0x0
#define CS4224_GLOBAL_UCODE_TIMESTAMP1            0x9
#define CS4224_GLOBAL_UCODE_TIMESTAMP1_dft        0x0
#define CS4224_GLOBAL_UCODE_TIMESTAMP2            0xA
#define CS4224_GLOBAL_UCODE_TIMESTAMP2_dft        0x0
#define CS4224_GLOBAL_MPIF_SOFT_RESET             0xD
#define CS4224_GLOBAL_MPIF_SOFT_RESET_dft         0x0
#define CS4224_GLOBAL_MDIO_CONFIG                 0xF
#define CS4224_GLOBAL_MDIO_CONFIG_dft             0x50
#define CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL         0x11
#define CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL_dft     0x0
#define CS4224_GLOBAL_DWNLD_CHECKSUM_STATUS       0x12
#define CS4224_GLOBAL_DWNLD_CHECKSUM_STATUS_dft   0x0
#define CS4224_GLOBAL_DWNLD_CHECKSUM_HW           0x13
#define CS4224_GLOBAL_DWNLD_CHECKSUM_HW_dft       0xFFFF
#define CS4224_GLOBAL_DWNLD_CHECKSUM_SW           0x14
#define CS4224_GLOBAL_DWNLD_CHECKSUM_SW_dft       0x0
#define CS4224_GLOBAL_PIN_STATUS                  0x16
#define CS4224_GLOBAL_PIN_STATUS_dft              0x0
#define CS4224_GLOBAL_BROADCAST                   0x17
#define CS4224_GLOBAL_BROADCAST_dft               0x0
#define CS4224_GLOBAL_GT_10KHZ_REF_CLK_CNT1       0x18
#define CS4224_GLOBAL_GT_10KHZ_REF_CLK_CNT1_dft   0x0
#define CS4224_GLOBAL_GT_10KHZ_REF_CLK_CNT0       0x19
#define CS4224_GLOBAL_GT_10KHZ_REF_CLK_CNT0_dft   0x3CF0
#define CS4224_GLOBAL_SCRATCH0                    0x1D
#define CS4224_GLOBAL_SCRATCH0_dft                0x0
#define CS4224_GLOBAL_SCRATCH6                    0x23
#define CS4224_GLOBAL_SCRATCH6_dft                0x0
#define CS4224_GLOBAL_SCRATCH7                    0x24
#define CS4224_GLOBAL_SCRATCH7_dft                0x0
#define CS4224_GLOBAL_GLOBAL_INTERRUPT            0x26
#define CS4224_GLOBAL_GLOBAL_INTERRUPT_dft        0x0
#define CS4224_GLOBAL_GLOBAL_INTE                 0x27
#define CS4224_GLOBAL_GLOBAL_INTE_dft             0x0
#define CS4224_GLOBAL_DATAPATH_INTERRUPT          0x28
#define CS4224_GLOBAL_DATAPATH_INTERRUPT_dft      0x0
#define CS4224_GLOBAL_DATAPATH_INTE               0x29
#define CS4224_GLOBAL_DATAPATH_INTE_dft           0x0
#define CS4224_GPIO_INTERR                        0x100
#define CS4224_GPIO_INTERR_dft                    0x0
#define CS4224_GPIO_GPIO1                         0x106
#define CS4224_GPIO_GPIO1_dft                     0x0
#define CS4224_GPIO_GPIO2                         0x10C
#define CS4224_GPIO_GPIO2_dft                     0x0
#define CS4224_GPIO_GPIO3                         0x112
#define CS4224_GPIO_GPIO3_dft                     0x0
#define CS4224_GPIO_GPIO4                         0x118
#define CS4224_GPIO_GPIO4_dft                     0x0
#define CS4224_GPIO_GPIO_INPUT0                   0x11E
#define CS4224_GPIO_GPIO_INPUT0_dft               0x0
#define CS4224_GPIO_GPIO_INPUT_INT                0x124
#define CS4224_GPIO_GPIO_INPUT_INT_dft            0x0
#define CS4224_GPIO_GPIO_INPUT_INTE               0x125
#define CS4224_GPIO_GPIO_INPUT_INTE_dft           0x0
#define CS4224_GPIO_GPIO_INPUT_INTS               0x126
#define CS4224_GPIO_GPIO_INPUT_INTS_dft           0x0
#define CS4224_GPIO_GPIO_INPUT_INTZ               0x127
#define CS4224_GPIO_GPIO_INPUT_INTZ_dft           0x0
#define CS4224_EFUSE_GENERAL_STATUS               0x181
#define CS4224_EFUSE_GENERAL_STATUS_dft           0x0
#define CS4224_EFUSE_PDF_SKU                      0x19F
#define CS4224_EFUSE_PDF_SKU_dft                  0x0
#define CS4224_EFUSE_PDF_MON_CAL_DATA             0x1A4
#define CS4224_EFUSE_PDF_MON_CAL_DATA_dft         0x0
#define CS4224_EFUSE_PDF_MON_GAIN_DATA            0x1A5
#define CS4224_EFUSE_PDF_MON_GAIN_DATA_dft        0x0
#define CS4224_EFUSE_PDF_MON_LUT15                0x1A6
#define CS4224_EFUSE_PDF_MON_LUT15_dft            0x0
#define CS4224_EFUSE_PDF_POLY_RES_CAL_DATA2       0x1B7
#define CS4224_EFUSE_PDF_POLY_RES_CAL_DATA2_dft   0x0
#define CS4224_EFUSE_PDF_POLY_RES_CAL_DATA1       0x1B8
#define CS4224_EFUSE_PDF_POLY_RES_CAL_DATA1_dft   0x0
#define CS4224_EFUSE_PDF_POLY_RES_CAL_DATA0       0x1B9
#define CS4224_EFUSE_PDF_POLY_RES_CAL_DATA0_dft   0x0
#define CS4224_MONITOR_CONTROL0                   0x200
#define CS4224_MONITOR_CONTROL0_dft               0x630C
#define CS4224_MONITOR_CONFIG_MASK                0x204
#define CS4224_MONITOR_CONFIG_MASK_dft            0x0
#define CS4224_MONITOR_LUT_SELECT                 0x20B
#define CS4224_MONITOR_LUT_SELECT_dft             0x0
#define CS4224_MONITOR_LUT_LOCAL_SELECT           0x20C
#define CS4224_MONITOR_LUT_LOCAL_SELECT_dft       0x0
#define CS4224_MONITOR_LUT_APPLY                  0x20D
#define CS4224_MONITOR_LUT_APPLY_dft              0x0
#define CS4224_MONITOR_CAL_CONST_OVERRIDE_ENA     0x20E
#define CS4224_MONITOR_CAL_CONST_OVERRIDE_ENA_dft 0x0
#define CS4224_MONITOR_CAL_OVERRIDE               0x20F
#define CS4224_MONITOR_CAL_OVERRIDE_dft           0x0
#define CS4224_MONITOR_CAL_CONST_APPLY            0x210
#define CS4224_MONITOR_CAL_CONST_APPLY_dft        0x0
#define CS4224_MONITOR_STATUS_FINAL0              0x25A
#define CS4224_MONITOR_STATUS_FINAL0_dft          0x0
#define CS4224_MONITOR_STATUS_FINAL2              0x25C
#define CS4224_MONITOR_STATUS_FINAL2_dft          0x0
#define CS4224_MONITOR_STATUS_FINAL6              0x260
#define CS4224_MONITOR_STATUS_FINAL6_dft          0x0
#define CS4224_MONITOR_LUT_RANGE0                 0x27B
#define CS4224_MONITOR_LUT_RANGE0_dft             0x0
#define CS4224_MONITOR_LUT_VALUE0                 0x28B
#define CS4224_MONITOR_LUT_VALUE0_dft             0x0
#define CS4224_CLKMON_GBL_CLKSEL                  0x2E0
#define CS4224_CLKMON_GBL_CLKSEL_dft              0x7F
#define CS4224_CLKMON_GBL_CTRL                    0x2E1
#define CS4224_CLKMON_GBL_CTRL_dft                0x110
#define CS4224_CLKMON_GBL_DURATION                0x2E2
#define CS4224_CLKMON_GBL_DURATION_dft            0x0
#define CS4224_CLKMON_GBL_MINTHRESH1              0x2E6
#define CS4224_CLKMON_GBL_MINTHRESH1_dft          0x0
#define CS4224_CLKMON_GBL_MINTHRESH0              0x2E7
#define CS4224_CLKMON_GBL_MINTHRESH0_dft          0x0
#define CS4224_CLKMON_GBL_MAXTHRESH1              0x2E8
#define CS4224_CLKMON_GBL_MAXTHRESH1_dft          0x0
#define CS4224_CLKMON_GBL_MAXTHRESH0              0x2E9
#define CS4224_CLKMON_GBL_MAXTHRESH0_dft          0x0
#define CS4224_MSEQ_PS_RAM_CONTROL                0x301
#define CS4224_MSEQ_PS_RAM_CONTROL_dft            0x0
#define CS4224_MSEQ_PS_RAM_DATA1                  0x302
#define CS4224_MSEQ_PS_RAM_DATA1_dft              0x0
#define CS4224_MSEQ_PS_RAM_DATA0                  0x303
#define CS4224_MSEQ_PS_RAM_DATA0_dft              0x0
#define CS4224_MSEQ_PS_INT                        0x306
#define CS4224_MSEQ_PS_INT_dft                    0x0
#define CS4224_MSEQ_PS_INTE                       0x307
#define CS4224_MSEQ_PS_INTE_dft                   0x0
#define CS4224_MSEQ_PS_INTO                       0x308
#define CS4224_MSEQ_PS_INTO_dft                   0x0
#define CS4224_MSEQ_PS_INTZ                       0x309
#define CS4224_MSEQ_PS_INTZ_dft                   0x0
#define CS4224_MSEQ_PS_MBIST_CTRL                 0x30A
#define CS4224_MSEQ_PS_MBIST_CTRL_dft             0x0
#define CS4224_MSEQ_PS_MBIST_STATUS               0x30B
#define CS4224_MSEQ_PS_MBIST_STATUS_dft           0x0
#define CS4224_PP_LINE_LINEMISC_SOFT_RESET        0x1000
#define CS4224_PP_LINE_LINEMISC_SOFT_RESET_dft    0x2
#define CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG 0x1001
#define CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG_dft 0x7
#define CS4224_PP_LINE_LINEMISC_GIGEPCS_SOFT_RESET 0x1002
#define CS4224_PP_LINE_LINEMISC_GIGEPCS_SOFT_RESET_dft 0x3
#define CS4224_PP_LINE_LINEMISC_FUNCEN            0x1003
#define CS4224_PP_LINE_LINEMISC_FUNCEN_dft        0x0
#define CS4224_PP_LINE_LINEMISC_CLKEN             0x1004
#define CS4224_PP_LINE_LINEMISC_CLKEN_dft         0x0
#define CS4224_PP_LINE_LINEMISC_MSEQCLKCTRL       0x1007
#define CS4224_PP_LINE_LINEMISC_MSEQCLKCTRL_dft   0x4
#define CS4224_PP_LINE_LINEMISC_DATAPATH_CTRL     0x100B
#define CS4224_PP_LINE_LINEMISC_DATAPATH_CTRL_dft 0xF180
#define CS4224_PP_LINE_LINEMISC_KR_AN_PORT_ADDR   0x100C
#define CS4224_PP_LINE_LINEMISC_KR_AN_PORT_ADDR_dft 0x0
#define CS4224_PP_LINE_LINEMISC_AN_MODE_SEL       0x100D
#define CS4224_PP_LINE_LINEMISC_AN_MODE_SEL_dft   0x0
#define CS4224_PP_LINE_LINEMISC_OVERRIDE_EN       0x1010
#define CS4224_PP_LINE_LINEMISC_OVERRIDE_EN_dft   0x0
#define CS4224_PP_LINE_LINEMISC_INTERRUPT         0x1011
#define CS4224_PP_LINE_LINEMISC_INTERRUPT_dft     0x0
#define CS4224_PP_LINE_LINEMISC_INTENABLE         0x1012
#define CS4224_PP_LINE_LINEMISC_INTENABLE_dft     0x0
#define CS4224_PP_LINE_SDS_COMMON_RX0_Config      0x1020
#define CS4224_PP_LINE_SDS_COMMON_RX0_Config_dft  0x18
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CONFIG  0x1021
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CONFIG_dft 0x4
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CLKDIV_CTRL 0x1023
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CLKDIV_CTRL_dft 0x3005
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CLKOUT_CTRL 0x1024
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CLKOUT_CTRL_dft 0x2FF
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CPA     0x1027
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CPA_dft 0x77
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_IBIAS_TUNE 0x1029
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_IBIAS_TUNE_dft 0x4
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_RBIAS_TUNE 0x102A
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_RBIAS_TUNE_dft 0x44
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_MISC    0x102D
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_MISC_dft 0x300
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_SPARE   0x102F
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_SPARE_dft 0xE0F0
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CONFIG_EYEMON 0x1030
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CONFIG_EYEMON_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_VCOMAX   0x1031
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_VCOMAX_dft 0xFFF
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_CONTROL  0x1033
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_CONTROL_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_ALTCT_LIMIT_HI 0x1035
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_ALTCT_LIMIT_HI_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_ALTCT_STATUS 0x1037
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_ALTCT_STATUS_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL3  0x1039
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL3_dft 0xFF
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL2  0x103A
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL2_dft 0xFF
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL1  0x103B
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL1_dft 0xFF
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL0  0x103C
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPLVL0_dft 0xFF
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES04 0x103D
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES04_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES03 0x103E
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES03_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES02 0x103F
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES02_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES01 0x1040
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES01_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES00 0x1041
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES00_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES14 0x1042
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES14_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES13 0x1043
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES13_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES12 0x1044
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES12_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES11 0x1045
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES11_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES10 0x1046
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_TMPTHRES10_dft 0x19
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL00 0x1047
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL00_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL01 0x1048
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL01_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL02 0x1049
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL02_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL03 0x104A
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL03_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL10 0x104B
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL10_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL11 0x104C
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL11_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL12 0x104D
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL12_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL13 0x104E
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL13_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_VCOBIAS 0x104F
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_VCOBIAS_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_STATUS   0x1050
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_STATUS_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_RESOLUTION 0x1057
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_RESOLUTION_dft 0x3
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_CONTROL 0x1059
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_CONTROL_dft 0x2098
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPT 0x105B
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPT_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTSTATUS 0x105C
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTSTATUS_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTENABLE 0x105D
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTENABLE_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPTZ 0x105E
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPTZ_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RXELST0_Control 0x106E
#define CS4224_PP_LINE_SDS_COMMON_RXELST0_Control_dft 0x2
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Cfg    0x106F
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Cfg_dft 0x2000
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Ctrl   0x1070
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Ctrl_dft 0xC
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Count1 0x1071
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Count1_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Count0 0x1072
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Count0_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_INTERRUPT 0x1073
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_INTERRUPT_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_INTSTATUS 0x1074
#define CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_INTSTATUS_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_RESET     0x1077
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_RESET_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_EN        0x1078
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_EN_dft    0x0
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_NUMERATOR0 0x1079
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_NUMERATOR0_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_NUMERATOR1 0x107A
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_NUMERATOR1_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_NUMERATOR0 0x107D
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_NUMERATOR0_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_NUMERATOR1 0x107E
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_NUMERATOR1_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_WIDTH     0x1081
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_WIDTH_dft 0xA
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_INTDIV    0x1082
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_INTDIV_dft 0x1919
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_STAGE_EN  0x1083
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_STAGE_EN_dft 0x408
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD0 0x1084
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD0_dft 0xCCCC
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD1 0x1085
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD1_dft 0xCC
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE2PRELOAD0 0x1086
#define CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE2PRELOAD0_dft 0xCCCC
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE1PRELOAD0 0x1088
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE1PRELOAD0_dft 0xCCCC
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE1PRELOAD1 0x1089
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE1PRELOAD1_dft 0xCC
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE2PRELOAD0 0x108A
#define CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE2PRELOAD0_dft 0xCCCC
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_DITHER    0x108D
#define CS4224_PP_LINE_SDS_COMMON_FRAC0_DITHER_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_TX0_Config      0x108E
#define CS4224_PP_LINE_SDS_COMMON_TX0_Config_dft  0x18
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA 0x108F
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA_dft 0x814
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB 0x1090
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB_dft 0x4
#define CS4224_PP_LINE_SDS_COMMON_STX0_SQUELCH    0x1091
#define CS4224_PP_LINE_SDS_COMMON_STX0_SQUELCH_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_STX0_MISC       0x1092
#define CS4224_PP_LINE_SDS_COMMON_STX0_MISC_dft   0x33
#define CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Cfg    0x1096
#define CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Cfg_dft 0x2000
#define CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Ctrl   0x1097
#define CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Ctrl_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Fixed0_Pattern1 0x1098
#define CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Fixed0_Pattern1_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_TXELST0_Control 0x109D
#define CS4224_PP_LINE_SDS_COMMON_TXELST0_Control_dft 0x2
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG1 0x10A1
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG1_dft 0x4
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG2 0x10A2
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG2_dft 0x40
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG4 0x10A4
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG4_dft 0x81F0
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_RTUNE  0x10A5
#define CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_RTUNE_dft 0x4
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DAC_ENB_MSB 0x10A6
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DAC_ENB_MSB_dft 0xD
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DAC_ENB_LSB 0x10A7
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DAC_ENB_LSB_dft 0xDFFF
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_DLY_CTRL1 0x10A8
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_DLY_CTRL1_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_DLY_CTRL2 0x10A9
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_DLY_CTRL2_dft 0x0
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_BIAS1  0x10AA
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_BIAS1_dft 0x5555
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_CONFIG 0x10AE
#define CS4224_PP_LINE_SDS_COMMON_SRX0_DFE_CONFIG_dft 0x441
#define CS4224_PP_LINE_SDS_COMMON_SRX0_MONCTRL    0x10B1
#define CS4224_PP_LINE_SDS_COMMON_SRX0_MONCTRL_dft 0x2000
#define CS4224_PP_LINE_SDS_COMMON_SRX0_VCO_CONFIG 0x10B2
#define CS4224_PP_LINE_SDS_COMMON_SRX0_VCO_CONFIG_dft 0x607
#define CS4224_PP_LINE_SDS_COMMON_STX0_DRIVER_CONFIG 0x10B4
#define CS4224_PP_LINE_SDS_COMMON_STX0_DRIVER_CONFIG_dft 0x10
#define CS4224_PP_LINE_SDS_COMMON_STX0_DRIVER_TUNE 0x10B5
#define CS4224_PP_LINE_SDS_COMMON_STX0_DRIVER_TUNE_dft 0x404
#define CS4224_PP_LINE_SDS_COMMON_STX0_DRIVERCML_TUNE 0x10B6
#define CS4224_PP_LINE_SDS_COMMON_STX0_DRIVERCML_TUNE_dft 0x404
#define CS4224_PP_LINE_SDS_COMMON_Int             0x10BA
#define CS4224_PP_LINE_SDS_COMMON_Int_dft         0x0
#define CS4224_PP_LINE_SDS_COMMON_IntEn           0x10BB
#define CS4224_PP_LINE_SDS_COMMON_IntEn_dft       0x0
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKDi        0x10C7
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKDi_dft    0x0
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKDe        0x10C8
#define CS4224_PP_LINE_SDS_COMMON_RXLOCKDe_dft    0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_CR       0x1220
#define CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_CR_dft   0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_D1       0x1221
#define CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_D1_dft   0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_D0       0x1222
#define CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_D0_dft   0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_PC_SHADOW     0x1223
#define CS4224_PP_LINE_SDS_DSP_MSEQ_PC_SHADOW_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_SHADOW 0x1224
#define CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_SHADOW_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS       0x1225
#define CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_dft   0x8
#define CS4224_PP_LINE_SDS_DSP_MSEQ_PC            0x1228
#define CS4224_PP_LINE_SDS_DSP_MSEQ_PC_dft        0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_IX            0x1229
#define CS4224_PP_LINE_SDS_DSP_MSEQ_IX_dft        0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_BASE3_INST    0x1233
#define CS4224_PP_LINE_SDS_DSP_MSEQ_BASE3_INST_dft 0x3
#define CS4224_PP_LINE_SDS_DSP_MSEQ_ENABLE        0x1234
#define CS4224_PP_LINE_SDS_DSP_MSEQ_ENABLE_dft    0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_LSB 0x1235
#define CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_LSB_dft 0x1E0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_MSB 0x1236
#define CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_STATUS        0x1237
#define CS4224_PP_LINE_SDS_DSP_MSEQ_STATUS_dft    0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_BANK_SELECTOR 0x1238
#define CS4224_PP_LINE_SDS_DSP_MSEQ_BANK_SELECTOR_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_SEL      0x1239
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_SEL_dft  0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_IN_LSB   0x123A
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_IN_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_IN_MSB   0x123B
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_IN_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_OUT_LSB  0x123C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_OUT_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_OUT_MSB  0x123D
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MAIL_OUT_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MU_VALUE      0x1245
#define CS4224_PP_LINE_SDS_DSP_MSEQ_MU_VALUE_dft  0xF0F
#define CS4224_PP_LINE_SDS_DSP_MSEQ_LEAK_INTVL_AGC 0x1247
#define CS4224_PP_LINE_SDS_DSP_MSEQ_LEAK_INTVL_AGC_dft 0x40
#define CS4224_PP_LINE_SDS_DSP_MSEQ_LEAK_INTVL_DFE 0x1248
#define CS4224_PP_LINE_SDS_DSP_MSEQ_LEAK_INTVL_DFE_dft 0x40
#define CS4224_PP_LINE_SDS_DSP_MSEQ_COEF_DSP_INIT_SEL 0x1249
#define CS4224_PP_LINE_SDS_DSP_MSEQ_COEF_DSP_INIT_SEL_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_COEF_DSP_FLOAT 0x124C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_COEF_DSP_FLOAT_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_EQADJ1 0x125E
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_EQADJ1_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_EQADJ2 0x125F
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_EQADJ2_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_DFE_GAIN0 0x1262
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_DFE_GAIN0_dft 0x1FF
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_DFE_GAIN1 0x1263
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_DFE_GAIN1_dft 0x1FF
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_DFE_GAIN2 0x1264
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_DFE_GAIN2_dft 0x1FF
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_PHSEL  0x1265
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_PHSEL_dft 0x20
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_SLICER 0x1266
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_SLICER_dft 0x80
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_AGC_MISC 0x1268
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_AGC_MISC_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_AGC_GAIN 0x1269
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_AGC_GAIN_dft 0x7
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SNR_CTRL      0x126C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SNR_CTRL_dft  0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SNR_ZEROS_LSB 0x126D
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SNR_ZEROS_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_RESET_COUNT_LSB 0x1273
#define CS4224_PP_LINE_SDS_DSP_MSEQ_RESET_COUNT_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_RESET_COUNT_MSB 0x1274
#define CS4224_PP_LINE_SDS_DSP_MSEQ_RESET_COUNT_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SERDES        0x1275
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SERDES_dft    0x30
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE0_LSB    0x1280
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE0_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE0_MSB    0x1281
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE0_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE1_LSB    0x1282
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE1_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE1_MSB    0x1283
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE1_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE2_LSB    0x1284
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE2_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE2_MSB    0x1285
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE2_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE3_LSB    0x1286
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE3_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE3_MSB    0x1287
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE3_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE4_LSB    0x1288
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE4_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE4_MSB    0x1289
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE4_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE5_LSB    0x128A
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE5_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE5_MSB    0x128B
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE5_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE6_LSB    0x128C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE6_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE6_MSB    0x128D
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE6_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE7_LSB    0x128E
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE7_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE7_MSB    0x128F
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE7_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE8_LSB    0x1290
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE8_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE8_MSB    0x1291
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE8_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE9_LSB    0x1292
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE9_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE9_MSB    0x1293
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE9_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE10_LSB   0x1294
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE10_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE10_MSB   0x1295
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE10_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE11_LSB   0x1296
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE11_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE11_MSB   0x1297
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE11_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE12_LSB   0x1298
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE12_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE12_MSB   0x1299
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE12_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE13_LSB   0x129A
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE13_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE13_MSB   0x129B
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE13_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE14_LSB   0x129C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE14_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE15_MSB   0x129F
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE15_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE16_LSB   0x12A0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE16_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE18_LSB   0x12A4
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE18_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE18_MSB   0x12A5
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE18_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE19_LSB   0x12A6
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE19_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE20_LSB   0x12A8
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE20_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE20_MSB   0x12A9
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE20_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE21_LSB   0x12AA
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE21_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE21_MSB   0x12AB
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE21_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB   0x12AC
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB   0x12AD
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE23_LSB   0x12AE
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE23_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE23_MSB   0x12AF
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE23_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB   0x12B0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_MSB   0x12B1
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE25_LSB   0x12B2
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE25_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE25_MSB   0x12B3
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE25_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_LSB   0x12B4
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_MSB   0x12B5
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE27_LSB   0x12B6
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE27_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE27_MSB   0x12B7
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE27_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE28_LSB   0x12B8
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE28_LSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE28_MSB   0x12B9
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE28_MSB_dft 0x0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE0_0          0x1320
#define CS4224_PP_LINE_SDS_DSP_IC_DFE0_0_dft      0xA0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE0_1          0x1321
#define CS4224_PP_LINE_SDS_DSP_IC_DFE0_1_dft      0xF0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE1_0          0x1324
#define CS4224_PP_LINE_SDS_DSP_IC_DFE1_0_dft      0xE0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE1_1          0x1325
#define CS4224_PP_LINE_SDS_DSP_IC_DFE1_1_dft      0xF8
#define CS4224_PP_LINE_SDS_DSP_IC_DFE2_0          0x1328
#define CS4224_PP_LINE_SDS_DSP_IC_DFE2_0_dft      0xF0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE2_1          0x1329
#define CS4224_PP_LINE_SDS_DSP_IC_DFE2_1_dft      0x0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE3_0          0x132C
#define CS4224_PP_LINE_SDS_DSP_IC_DFE3_0_dft      0x0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE3_1          0x132D
#define CS4224_PP_LINE_SDS_DSP_IC_DFE3_1_dft      0x0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE4_0          0x132E
#define CS4224_PP_LINE_SDS_DSP_IC_DFE4_0_dft      0x0
#define CS4224_PP_LINE_SDS_DSP_IC_DFE4_1          0x132F
#define CS4224_PP_LINE_SDS_DSP_IC_DFE4_1_dft      0x0
#define CS4224_PP_LINE_SDS_DSP_ENABLE             0x1343
#define CS4224_PP_LINE_SDS_DSP_ENABLE_dft         0x4
#define CS4224_PP_LINE_SDS_DSP_COEF_SATURATED_INT 0x1354
#define CS4224_PP_LINE_SDS_DSP_COEF_SATURATED_INT_dft 0x0
#define CS4224_PP_LINE_GIGEPCS_LINE_CONTROL       0x1460
#define CS4224_PP_LINE_GIGEPCS_LINE_CONTROL_dft   0x140
#define CS4224_PP_LINE_GIGEPCS_LINE_STATUS        0x1461
#define CS4224_PP_LINE_GIGEPCS_LINE_STATUS_dft    0x9
#define CS4224_PP_LINE_GIGEPCS_INT_LINE_PCS1GE_INTERRUPT 0x1480
#define CS4224_PP_LINE_GIGEPCS_INT_LINE_PCS1GE_INTERRUPT_dft 0x0
#define CS4224_PP_LINE_GIGEPCS_INT_LINE_PCS1GE_INTSTATUS 0x1482
#define CS4224_PP_LINE_GIGEPCS_INT_LINE_PCS1GE_INTSTATUS_dft 0x0
#define CS4224_PP_LINE_EGPCS_RX_MODE              0x14A0
#define CS4224_PP_LINE_EGPCS_RX_MODE_dft          0x8003
#define CS4224_PP_LINE_EGPCS_RX_INTERRUPT         0x14A4
#define CS4224_PP_LINE_EGPCS_RX_INTERRUPT_dft     0x0
#define CS4224_PP_LINE_EGPCS_RX_INTSTATUS         0x14A6
#define CS4224_PP_LINE_EGPCS_RX_INTSTATUS_dft     0x0
#define CS4224_PP_LINE_EGPCS_TX_MODE              0x14B0
#define CS4224_PP_LINE_EGPCS_TX_MODE_dft          0x8001
#define CS4224_PP_LINE_EGPCS_TX_INTERRUPT         0x14B5
#define CS4224_PP_LINE_EGPCS_TX_INTERRUPT_dft     0x0
#define CS4224_PP_LINE_XGPCS_TX_TXCNTRL           0x14C0
#define CS4224_PP_LINE_XGPCS_TX_TXCNTRL_dft       0x8002
#define CS4224_PP_LINE_XGPCS_RX_RXCNTRL           0x14E0
#define CS4224_PP_LINE_XGPCS_RX_RXCNTRL_dft       0xD000
#define CS4224_PP_LINE_XGPCS_RX_RXSTATUS          0x14E1
#define CS4224_PP_LINE_XGPCS_RX_RXSTATUS_dft      0x0
#define CS4224_PP_LINE_XGPCS_RX_RXINT             0x14E2
#define CS4224_PP_LINE_XGPCS_RX_RXINT_dft         0x0
#define CS4224_PP_LINE_AN_TX_ENABLE               0x1500
#define CS4224_PP_LINE_AN_TX_ENABLE_dft           0x0
#define CS4224_PP_LINE_AN_TX_COMPLETE_STATUS      0x1505
#define CS4224_PP_LINE_AN_TX_COMPLETE_STATUS_dft  0x0
#define CS4224_PP_LINE_AN_TX_AN_COMPLETE_STATUS_INT 0x1506
#define CS4224_PP_LINE_AN_TX_AN_COMPLETE_STATUS_INT_dft 0x0
#define CS4224_PP_LINE_AN_TX_ADV_ABILITY          0x150C
#define CS4224_PP_LINE_AN_TX_ADV_ABILITY_dft      0x0
#define CS4224_PP_LINE_AN_TX_TECH_ABILITY_1       0x150D
#define CS4224_PP_LINE_AN_TX_TECH_ABILITY_1_dft   0x0
#define CS4224_PP_LINE_AN_TX_BP_STATUS            0x151B
#define CS4224_PP_LINE_AN_TX_BP_STATUS_dft        0x1
#define CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMEOUT0   0x1523
#define CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMEOUT0_dft 0x17C8
#define CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMER_THRES1 0x1524
#define CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMER_THRES1_dft 0x3B9
#define CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMER_THRES0 0x1525
#define CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMER_THRES0_dft 0xACA0
#define CS4224_PP_LINE_AN_TX_ABILITY_DETECT_TIMER_ENABLE 0x152A
#define CS4224_PP_LINE_AN_TX_ABILITY_DETECT_TIMER_ENABLE_dft 0x0
#define CS4224_PP_LINE_AN_TX_MAIN_INT             0x152D
#define CS4224_PP_LINE_AN_TX_MAIN_INT_dft         0x0
#define CS4224_PP_LINE_AN_TX_MAIN_INTS            0x152F
#define CS4224_PP_LINE_AN_TX_MAIN_INTS_dft        0x0
#define CS4224_PP_LINE_AN_TX_TX_AFIFO_INT         0x1531
#define CS4224_PP_LINE_AN_TX_TX_AFIFO_INT_dft     0x0
#define CS4224_PP_LINE_AN_TX_ARBITRATOR_STATE     0x1536
#define CS4224_PP_LINE_AN_TX_ARBITRATOR_STATE_dft 0x0
#define CS4224_PP_LINE_AN_RX_PAGE_RECEIVED        0x1545
#define CS4224_PP_LINE_AN_RX_PAGE_RECEIVED_dft    0x0
#define CS4224_PP_LINE_AN_RX_LP_ABILITY_1         0x1546
#define CS4224_PP_LINE_AN_RX_LP_ABILITY_1_dft     0x0
#define CS4224_PP_LINE_AN_RX_LP_ABILITY_2         0x1547
#define CS4224_PP_LINE_AN_RX_LP_ABILITY_2_dft     0x0
#define CS4224_PP_LINE_AN_RX_MAIN_INT             0x155A
#define CS4224_PP_LINE_AN_RX_MAIN_INT_dft         0x0
#define CS4224_PP_LINE_AN_RX_RX_AFIFO_INT         0x155E
#define CS4224_PP_LINE_AN_RX_RX_AFIFO_INT_dft     0x0
#define CS4224_PP_LINE_TP_TX_ENABLE               0x1580
#define CS4224_PP_LINE_TP_TX_ENABLE_dft           0x0
#define CS4224_PP_LINE_TP_TX_TX_RX_RESET          0x1581
#define CS4224_PP_LINE_TP_TX_TX_RX_RESET_dft      0x3
#define CS4224_PP_LINE_TP_TX_BITSWAP              0x1583
#define CS4224_PP_LINE_TP_TX_BITSWAP_dft          0x1
#define CS4224_PP_LINE_TP_TX_TRAINING             0x1586
#define CS4224_PP_LINE_TP_TX_TRAINING_dft         0x0
#define CS4224_PP_LINE_TP_TX_TRAINING_INT         0x1588
#define CS4224_PP_LINE_TP_TX_TRAINING_INT_dft     0x0
#define CS4224_PP_LINE_TP_TX_TRAINING_INTS        0x158A
#define CS4224_PP_LINE_TP_TX_TRAINING_INTS_dft    0x0
#define CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMEOUT1    0x158C
#define CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMEOUT1_dft 0x950
#define CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMEOUT0    0x158D
#define CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMEOUT0_dft 0x2F90
#define CS4224_PP_LINE_TP_TX_WAIT_TIMEOUT         0x158E
#define CS4224_PP_LINE_TP_TX_WAIT_TIMEOUT_dft     0x64
#define CS4224_PP_LINE_TP_TX_ADDITIONAL_FRAMES    0x158F
#define CS4224_PP_LINE_TP_TX_ADDITIONAL_FRAMES_dft 0x2
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_MAX_LIMIT  0x1590
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_MAX_LIMIT_dft 0x30
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_MAX_LIMIT 0x1591
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_MAX_LIMIT_dft 0x30
#define CS4224_PP_LINE_TP_TX_POST_COEFF_MAX_LIMIT 0x1592
#define CS4224_PP_LINE_TP_TX_POST_COEFF_MAX_LIMIT_dft 0x30
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_MIN_LIMIT  0x1593
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_MIN_LIMIT_dft 0x0
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_MIN_LIMIT 0x1594
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_MIN_LIMIT_dft 0x0
#define CS4224_PP_LINE_TP_TX_POST_COEFF_MIN_LIMIT 0x1595
#define CS4224_PP_LINE_TP_TX_POST_COEFF_MIN_LIMIT_dft 0x0
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_INIT       0x1596
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_INIT_dft   0x0
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_INIT      0x1597
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_INIT_dft  0x0
#define CS4224_PP_LINE_TP_TX_POST_COEFF_INIT      0x1598
#define CS4224_PP_LINE_TP_TX_POST_COEFF_INIT_dft  0x0
#define CS4224_PP_LINE_TP_TX_PRESET_COEFF_OVERRIDE 0x1599
#define CS4224_PP_LINE_TP_TX_PRESET_COEFF_OVERRIDE_dft 0x0
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_PRESET     0x159A
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_PRESET_dft 0x0
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_PRESET    0x159B
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_PRESET_dft 0x0
#define CS4224_PP_LINE_TP_TX_POST_COEFF_PRESET    0x159C
#define CS4224_PP_LINE_TP_TX_POST_COEFF_PRESET_dft 0x0
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_START      0x159D
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_START_dft  0x5
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_START     0x159E
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_START_dft 0x23
#define CS4224_PP_LINE_TP_TX_POST_COEFF_START     0x159F
#define CS4224_PP_LINE_TP_TX_POST_COEFF_START_dft 0x24
#define CS4224_PP_LINE_TP_TX_PRE_COEFF            0x15A0
#define CS4224_PP_LINE_TP_TX_PRE_COEFF_dft        0x0
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF           0x15A1
#define CS4224_PP_LINE_TP_TX_MAIN_COEFF_dft       0x0
#define CS4224_PP_LINE_TP_TX_POST_COEFF           0x15A2
#define CS4224_PP_LINE_TP_TX_POST_COEFF_dft       0x0
#define CS4224_PP_LINE_TP_TX_COEFF_STEP_VALUE     0x15A3
#define CS4224_PP_LINE_TP_TX_COEFF_STEP_VALUE_dft 0x111
#define CS4224_PP_LINE_TP_TX_LD_COEFF_UPDATE_CTRL 0x15A4
#define CS4224_PP_LINE_TP_TX_LD_COEFF_UPDATE_CTRL_dft 0x0
#define CS4224_PP_LINE_TP_TX_FC_OPTIONS           0x15A8
#define CS4224_PP_LINE_TP_TX_FC_OPTIONS_dft       0x0
#define CS4224_PP_LINE_TP_TX_ENCODER_STATE        0x15A9
#define CS4224_PP_LINE_TP_TX_ENCODER_STATE_dft    0x0
#define CS4224_PP_LINE_TP_TX_TRAINING_STATE       0x15AA
#define CS4224_PP_LINE_TP_TX_TRAINING_STATE_dft   0x0
#define CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMER_EN    0x15AF
#define CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMER_EN_dft 0x1
#define CS4224_PP_LINE_TP_RX_BITSWAP              0x15C0
#define CS4224_PP_LINE_TP_RX_BITSWAP_dft          0x1
#define CS4224_PP_LINE_TP_RX_REMOTE_RX_READY      0x15C1
#define CS4224_PP_LINE_TP_RX_REMOTE_RX_READY_dft  0x113
#define CS4224_PP_LINE_TP_RX_FRAME_LOCK_INT       0x15C2
#define CS4224_PP_LINE_TP_RX_FRAME_LOCK_INT_dft   0x0
#define CS4224_PP_LINE_TP_RX_FRAME_LOCK_INTS      0x15C4
#define CS4224_PP_LINE_TP_RX_FRAME_LOCK_INTS_dft  0x0
#define CS4224_PP_LINE_TP_RX_FM_DETECT_STATE      0x15C9
#define CS4224_PP_LINE_TP_RX_FM_DETECT_STATE_dft  0x0
#define CS4224_PP_LINE_TP_RX_FRAME_LOCK_STATE     0x15CA
#define CS4224_PP_LINE_TP_RX_FRAME_LOCK_STATE_dft 0x0
#define CS4224_PP_LINE_KR_FEC_TX_RESET            0x1600
#define CS4224_PP_LINE_KR_FEC_TX_RESET_dft        0x1
#define CS4224_PP_LINE_KR_FEC_TX_STATUS           0x1602
#define CS4224_PP_LINE_KR_FEC_TX_STATUS_dft       0x0
#define CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL    0x160A
#define CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL_dft 0x0
#define CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL1 0x160B
#define CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL1_dft 0x0
#define CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL0 0x160C
#define CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL0_dft 0x0
#define CS4224_PP_LINE_KR_FEC_TX_INT              0x160D
#define CS4224_PP_LINE_KR_FEC_TX_INT_dft          0x0
#define CS4224_PP_LINE_KR_FEC_RX_RESET            0x1640
#define CS4224_PP_LINE_KR_FEC_RX_RESET_dft        0x1
#define CS4224_PP_LINE_KR_FEC_RX_STATUS           0x1642
#define CS4224_PP_LINE_KR_FEC_RX_STATUS_dft       0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL    0x1644
#define CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL1 0x1645
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL1_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL0 0x1646
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL0_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_CORR1 0x1647
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_CORR1_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_CORR0 0x1648
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_CORR0_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_UNCORR1 0x1649
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_UNCORR1_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_UNCORR0 0x164A
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_UNCORR0_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ZERO_ERRS1 0x164B
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ZERO_ERRS1_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ZERO_ERRS0 0x164C
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ZERO_ERRS0_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ONE_ERRS1 0x164D
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ONE_ERRS1_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ONE_ERRS0 0x164E
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ONE_ERRS0_dft 0x0
#define CS4224_PP_LINE_KR_FEC_RX_INT              0x165F
#define CS4224_PP_LINE_KR_FEC_RX_INT_dft          0x0
#define CS4224_PP_LINE_MCAN_RESET                 0x1680
#define CS4224_PP_LINE_MCAN_RESET_dft             0x1
#define CS4224_PP_HOST_HOSTMISC_SOFT_RESET        0x1800
#define CS4224_PP_HOST_HOSTMISC_SOFT_RESET_dft    0x2
#define CS4224_PP_HOST_HOSTMISC_MPIF_RESET_DOTREG 0x1801
#define CS4224_PP_HOST_HOSTMISC_MPIF_RESET_DOTREG_dft 0x3
#define CS4224_PP_HOST_HOSTMISC_GIGEPCS_SOFT_RESET 0x1802
#define CS4224_PP_HOST_HOSTMISC_GIGEPCS_SOFT_RESET_dft 0x1
#define CS4224_PP_HOST_HOSTMISC_FUNCEN            0x1803
#define CS4224_PP_HOST_HOSTMISC_FUNCEN_dft        0x0
#define CS4224_PP_HOST_HOSTMISC_CLKEN             0x1804
#define CS4224_PP_HOST_HOSTMISC_CLKEN_dft         0x0
#define CS4224_PP_HOST_HOSTMISC_MATE_SELECT       0x1806
#define CS4224_PP_HOST_HOSTMISC_MATE_SELECT_dft   0x0
#define CS4224_PP_HOST_HOSTMISC_MSEQCLKCTRL       0x1807
#define CS4224_PP_HOST_HOSTMISC_MSEQCLKCTRL_dft   0x4
#define CS4224_PP_HOST_HOSTMISC_OVERRIDE_EN       0x1810
#define CS4224_PP_HOST_HOSTMISC_OVERRIDE_EN_dft   0x0
#define CS4224_PP_HOST_HOSTMISC_INTERRUPT         0x1811
#define CS4224_PP_HOST_HOSTMISC_INTERRUPT_dft     0x0
#define CS4224_PP_HOST_HOSTMISC_INTENABLE         0x1812
#define CS4224_PP_HOST_HOSTMISC_INTENABLE_dft     0x0
#define CS4224_PP_HOST_SDS_COMMON_RX0_Config      0x1820
#define CS4224_PP_HOST_SDS_COMMON_RX0_Config_dft  0x18
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CONFIG  0x1821
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CONFIG_dft 0x4
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CLKDIV_CTRL 0x1823
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CLKDIV_CTRL_dft 0x3005
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CLKOUT_CTRL 0x1824
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CLKOUT_CTRL_dft 0x2FF
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CPA     0x1827
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CPA_dft 0x77
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_IBIAS_TUNE 0x1829
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_IBIAS_TUNE_dft 0x4
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_RBIAS_TUNE 0x182A
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_RBIAS_TUNE_dft 0x44
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_SPARE   0x182F
#define CS4224_PP_HOST_SDS_COMMON_SRX0_RX_SPARE_dft 0xE0F0
#define CS4224_PP_HOST_SDS_COMMON_RXVCO0_CONTROL  0x1833
#define CS4224_PP_HOST_SDS_COMMON_RXVCO0_CONTROL_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RXVCO0_ALTCT_STATUS 0x1837
#define CS4224_PP_HOST_SDS_COMMON_RXVCO0_ALTCT_STATUS_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_CONTROL 0x1859
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_CONTROL_dft 0x2098
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPT 0x185B
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPT_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTSTATUS 0x185C
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTSTATUS_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTENABLE 0x185D
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTENABLE_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPTZ 0x185E
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPTZ_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RXELST0_Control 0x186E
#define CS4224_PP_HOST_SDS_COMMON_RXELST0_Control_dft 0x2
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Cfg    0x186F
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Cfg_dft 0x2000
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Ctrl   0x1870
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Ctrl_dft 0xC
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Count1 0x1871
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Count1_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Count0 0x1872
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Count0_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_INTERRUPT 0x1873
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_INTERRUPT_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_INTSTATUS 0x1874
#define CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_INTSTATUS_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_RESET     0x1877
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_RESET_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_EN        0x1878
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_EN_dft    0x0
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_NUMERATOR0 0x1879
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_NUMERATOR0_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_NUMERATOR1 0x187A
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_NUMERATOR1_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_NUMERATOR0 0x187D
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_NUMERATOR0_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_NUMERATOR1 0x187E
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_NUMERATOR1_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_WIDTH     0x1881
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_WIDTH_dft 0xA
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_INTDIV    0x1882
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_INTDIV_dft 0x1919
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_STAGE_EN  0x1883
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_STAGE_EN_dft 0x408
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD0 0x1884
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD0_dft 0xCCCC
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD1 0x1885
#define CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD1_dft 0xCC
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE1PRELOAD0 0x1888
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE1PRELOAD0_dft 0xCCCC
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE1PRELOAD1 0x1889
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE1PRELOAD1_dft 0xCC
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE2PRELOAD0 0x188A
#define CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE2PRELOAD0_dft 0xCCCC
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_DITHER    0x188D
#define CS4224_PP_HOST_SDS_COMMON_FRAC0_DITHER_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_TX0_Config      0x188E
#define CS4224_PP_HOST_SDS_COMMON_TX0_Config_dft  0x18
#define CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLA 0x188F
#define CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLA_dft 0x814
#define CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLB 0x1890
#define CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLB_dft 0x4
#define CS4224_PP_HOST_SDS_COMMON_STX0_SQUELCH    0x1891
#define CS4224_PP_HOST_SDS_COMMON_STX0_SQUELCH_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_STX0_MISC       0x1892
#define CS4224_PP_HOST_SDS_COMMON_STX0_MISC_dft   0x33
#define CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Cfg    0x1896
#define CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Cfg_dft 0x2000
#define CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Ctrl   0x1897
#define CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Ctrl_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Fixed0_Pattern1 0x1898
#define CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Fixed0_Pattern1_dft 0x0
#define CS4224_PP_HOST_SDS_COMMON_TXELST0_Control 0x189D
#define CS4224_PP_HOST_SDS_COMMON_TXELST0_Control_dft 0x2
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG1 0x18A1
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG1_dft 0x4
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG2 0x18A2
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG2_dft 0x40
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG4 0x18A4
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG4_dft 0x81F0
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_RTUNE  0x18A5
#define CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_RTUNE_dft 0x4
#define CS4224_PP_HOST_SDS_COMMON_SRX0_DAC_ENB_MSB 0x18A6
#define CS4224_PP_HOST_SDS_COMMON_SRX0_DAC_ENB_MSB_dft 0xD
#define CS4224_PP_HOST_SDS_COMMON_SRX0_DAC_ENB_LSB 0x18A7
#define CS4224_PP_HOST_SDS_COMMON_SRX0_DAC_ENB_LSB_dft 0xDFFF
#define CS4224_PP_HOST_SDS_COMMON_SRX0_DFE_CONFIG 0x18AE
#define CS4224_PP_HOST_SDS_COMMON_SRX0_DFE_CONFIG_dft 0x441
#define CS4224_PP_HOST_SDS_COMMON_SRX0_VCO_CONFIG 0x18B2
#define CS4224_PP_HOST_SDS_COMMON_SRX0_VCO_CONFIG_dft 0x607
#define CS4224_PP_HOST_SDS_COMMON_STX0_DRIVER_CONFIG 0x18B4
#define CS4224_PP_HOST_SDS_COMMON_STX0_DRIVER_CONFIG_dft 0x10
#define CS4224_PP_HOST_SDS_COMMON_STX0_DRIVER_TUNE 0x18B5
#define CS4224_PP_HOST_SDS_COMMON_STX0_DRIVER_TUNE_dft 0x404
#define CS4224_PP_HOST_SDS_COMMON_STX0_DRIVERCML_TUNE 0x18B6
#define CS4224_PP_HOST_SDS_COMMON_STX0_DRIVERCML_TUNE_dft 0x404
#define CS4224_PP_HOST_SDS_COMMON_Int             0x18BA
#define CS4224_PP_HOST_SDS_COMMON_Int_dft         0x0
#define CS4224_PP_HOST_SDS_COMMON_IntEn           0x18BB
#define CS4224_PP_HOST_SDS_COMMON_IntEn_dft       0x0
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKDi        0x18C7
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKDi_dft    0x0
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKDe        0x18C8
#define CS4224_PP_HOST_SDS_COMMON_RXLOCKDe_dft    0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_CR       0x1A20
#define CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_CR_dft   0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_D1       0x1A21
#define CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_D1_dft   0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_D0       0x1A22
#define CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_D0_dft   0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_PC_SHADOW     0x1A23
#define CS4224_PP_HOST_SDS_DSP_MSEQ_PC_SHADOW_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS_SHADOW 0x1A24
#define CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS_SHADOW_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS       0x1A25
#define CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS_dft   0x8
#define CS4224_PP_HOST_SDS_DSP_MSEQ_PC            0x1A28
#define CS4224_PP_HOST_SDS_DSP_MSEQ_PC_dft        0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_IX            0x1A29
#define CS4224_PP_HOST_SDS_DSP_MSEQ_IX_dft        0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_BASE3_INST    0x1A33
#define CS4224_PP_HOST_SDS_DSP_MSEQ_BASE3_INST_dft 0x3
#define CS4224_PP_HOST_SDS_DSP_MSEQ_ENABLE        0x1A34
#define CS4224_PP_HOST_SDS_DSP_MSEQ_ENABLE_dft    0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_POWER_DOWN_LSB 0x1A35
#define CS4224_PP_HOST_SDS_DSP_MSEQ_POWER_DOWN_LSB_dft 0x1E0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_STATUS        0x1A37
#define CS4224_PP_HOST_SDS_DSP_MSEQ_STATUS_dft    0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_BANK_SELECTOR 0x1A38
#define CS4224_PP_HOST_SDS_DSP_MSEQ_BANK_SELECTOR_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_SEL      0x1A39
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_SEL_dft  0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_IN_LSB   0x1A3A
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_IN_LSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_IN_MSB   0x1A3B
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_IN_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_OUT_LSB  0x1A3C
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_OUT_LSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_OUT_MSB  0x1A3D
#define CS4224_PP_HOST_SDS_DSP_MSEQ_MAIL_OUT_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_CAL_RX_PHSEL  0x1A65
#define CS4224_PP_HOST_SDS_DSP_MSEQ_CAL_RX_PHSEL_dft 0x20
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SERDES        0x1A75
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SERDES_dft    0x30
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE0_LSB    0x1A80
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE0_LSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE6_MSB    0x1A8D
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE6_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE12_LSB   0x1A98
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE12_LSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE12_MSB   0x1A99
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE12_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE14_LSB   0x1A9C
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE14_LSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE15_MSB   0x1A9F
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE15_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE20_MSB   0x1AA9
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE20_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_MSB   0x1AB1
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE25_LSB   0x1AB2
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE25_LSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE25_MSB   0x1AB3
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE25_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE26_MSB   0x1AB5
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE26_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE28_MSB   0x1AB9
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE28_MSB_dft 0x0
#define CS4224_PP_HOST_SDS_DSP_COEF_SATURATED_INT 0x1B54
#define CS4224_PP_HOST_SDS_DSP_COEF_SATURATED_INT_dft 0x0
#define CS4224_PP_HOST_GIGEPCS_INT_HOST_PCS1GE_INTERRUPT 0x1C80
#define CS4224_PP_HOST_GIGEPCS_INT_HOST_PCS1GE_INTERRUPT_dft 0x0
#define CS4224_PP_HOST_GIGEPCS_INT_HOST_PCS1GE_INTSTATUS 0x1C82
#define CS4224_PP_HOST_GIGEPCS_INT_HOST_PCS1GE_INTSTATUS_dft 0x0
#define CS4224_PP_CLKMON_ING_CLKSEL               0x1D00
#define CS4224_PP_CLKMON_ING_CLKSEL_dft           0x7F
#define CS4224_PP_CLKMON_ING_CTRL                 0x1D01
#define CS4224_PP_CLKMON_ING_CTRL_dft             0x110
#define CS4224_PP_CLKMON_ING_DURATION             0x1D02
#define CS4224_PP_CLKMON_ING_DURATION_dft         0x0
#define CS4224_PP_CLKMON_ING_STATUS               0x1D03
#define CS4224_PP_CLKMON_ING_STATUS_dft           0x0
#define CS4224_PP_CLKMON_ING_COUNT1               0x1D04
#define CS4224_PP_CLKMON_ING_COUNT1_dft           0x0
#define CS4224_PP_CLKMON_ING_COUNT0               0x1D05
#define CS4224_PP_CLKMON_ING_COUNT0_dft           0x0
#define CS4224_PP_CLKMON_ING_MINTHRESH1           0x1D06
#define CS4224_PP_CLKMON_ING_MINTHRESH1_dft       0x0
#define CS4224_PP_CLKMON_ING_MINTHRESH0           0x1D07
#define CS4224_PP_CLKMON_ING_MINTHRESH0_dft       0x0
#define CS4224_PP_CLKMON_ING_MAXTHRESH1           0x1D08
#define CS4224_PP_CLKMON_ING_MAXTHRESH1_dft       0x0
#define CS4224_PP_CLKMON_ING_MAXTHRESH0           0x1D09
#define CS4224_PP_CLKMON_ING_MAXTHRESH0_dft       0x0
#define CS4224_PP_CLKMON_EGR_CLKSEL               0x1D10
#define CS4224_PP_CLKMON_EGR_CLKSEL_dft           0x7F
#define CS4224_PP_CLKMON_EGR_CTRL                 0x1D11
#define CS4224_PP_CLKMON_EGR_CTRL_dft             0x110
#define CS4224_PP_CLKMON_EGR_DURATION             0x1D12
#define CS4224_PP_CLKMON_EGR_DURATION_dft         0x0
#define CS4224_PP_CLKMON_EGR_STATUS               0x1D13
#define CS4224_PP_CLKMON_EGR_STATUS_dft           0x0
#define CS4224_PP_CLKMON_EGR_COUNT1               0x1D14
#define CS4224_PP_CLKMON_EGR_COUNT1_dft           0x0
#define CS4224_PP_CLKMON_EGR_COUNT0               0x1D15
#define CS4224_PP_CLKMON_EGR_COUNT0_dft           0x0
#define CS4224_PP_CLKMON_EGR_MINTHRESH1           0x1D16
#define CS4224_PP_CLKMON_EGR_MINTHRESH1_dft       0x0
#define CS4224_PP_CLKMON_EGR_MINTHRESH0           0x1D17
#define CS4224_PP_CLKMON_EGR_MINTHRESH0_dft       0x0
#define CS4224_PP_CLKMON_EGR_MAXTHRESH1           0x1D18
#define CS4224_PP_CLKMON_EGR_MAXTHRESH1_dft       0x0
#define CS4224_PP_CLKMON_EGR_MAXTHRESH0           0x1D19
#define CS4224_PP_CLKMON_EGR_MAXTHRESH0_dft       0x0
#define CS4224_EEPROM_LOADER_STATUS               0x5001
#define CS4224_EEPROM_LOADER_STATUS_dft           0x0
#define CS4224_EEPROM_MICRO_ACCESS_CONTROL        0x5005
#define CS4224_EEPROM_MICRO_ACCESS_CONTROL_dft    0x0
#define CS4224_EEPROM_MICRO_ACCESS_STATUS         0x5006
#define CS4224_EEPROM_MICRO_ACCESS_STATUS_dft     0x0

#if !defined(CS_HAS_PYTHON)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 id_lsb         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 id_lsb         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_CHIP_ID_LSB_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 id_msb         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 id_msb         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_CHIP_ID_MSB_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 bit            :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit            :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_SCRATCH_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 version        :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 version        :5;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_VERSION_SR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 version        :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 version        :5;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_VERSION_CX1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 version        :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 version        :5;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_VERSION_KR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 version        :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 version        :5;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_VERSION_ZR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 version        :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 version        :5;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_VERSION_FC_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 month          :8;
    cs_uint16 day            :8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 day            :8;
    cs_uint16 month          :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_TIMESTAMP0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 year           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 year           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_TIMESTAMP1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 hour           :8;
    cs_uint16 min            :8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 min            :8;
    cs_uint16 hour           :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_UCODE_TIMESTAMP2_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 checksum_error :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 checksum_error :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_DWNLD_CHECKSUM_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 checksum       :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 checksum       :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_DWNLD_CHECKSUM_HW_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 checksum       :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 checksum       :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_DWNLD_CHECKSUM_SW_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :1;
    cs_uint16 ROM_PRSNT      :1;
    cs_uint16 REFDIV2_ENB    :1;
    cs_uint16 LOCALREF_EN    :1;
    cs_uint16 TDI            :1;
    cs_uint16 EPSDA          :1;
    cs_uint16 EPSCL          :1;
    cs_uint16 IO_VDD_SEL     :1;
    cs_uint16 MICROSEL       :2;
    cs_uint16 MADDR          :6;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MADDR          :6;
    cs_uint16 MICROSEL       :2;
    cs_uint16 IO_VDD_SEL     :1;
    cs_uint16 EPSCL          :1;
    cs_uint16 EPSDA          :1;
    cs_uint16 TDI            :1;
    cs_uint16 LOCALREF_EN    :1;
    cs_uint16 REFDIV2_ENB    :1;
    cs_uint16 ROM_PRSNT      :1;
    cs_uint16 rsrvd1         :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_PIN_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 broadcast      :1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 maddr_mask     :6;
    cs_uint16 broadcast_port :1;
    cs_uint16 broadcast_side :1;
    cs_uint16 maddr_value    :6;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 maddr_value    :6;
    cs_uint16 broadcast_side :1;
    cs_uint16 broadcast_port :1;
    cs_uint16 maddr_mask     :6;
    cs_uint16 rsrvd1         :1;
    cs_uint16 broadcast      :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_BROADCAST_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 bit            :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit            :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_SCRATCH0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 bit            :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit            :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_SCRATCH6_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 bit            :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit            :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_SCRATCH7_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :10;
    cs_uint16 mseq_ps1I      :1;
    cs_uint16 mseq_ps0I      :1;
    cs_uint16 monitorI       :1;
    cs_uint16 gpio_inputI    :1;
    cs_uint16 gpioI          :1;
    cs_uint16 datapathI      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 datapathI      :1;
    cs_uint16 gpioI          :1;
    cs_uint16 gpio_inputI    :1;
    cs_uint16 monitorI       :1;
    cs_uint16 mseq_ps0I      :1;
    cs_uint16 mseq_ps1I      :1;
    cs_uint16 rsrvd1         :10;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_GLOBAL_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :10;
    cs_uint16 mseq_ps1E      :1;
    cs_uint16 mseq_ps0E      :1;
    cs_uint16 monitorE       :1;
    cs_uint16 gpio_inputE    :1;
    cs_uint16 gpioE          :1;
    cs_uint16 datapathE      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 datapathE      :1;
    cs_uint16 gpioE          :1;
    cs_uint16 gpio_inputE    :1;
    cs_uint16 monitorE       :1;
    cs_uint16 mseq_ps0E      :1;
    cs_uint16 mseq_ps1E      :1;
    cs_uint16 rsrvd1         :10;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_GLOBAL_INTE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 host3I         :1;
    cs_uint16 host2I         :1;
    cs_uint16 host1I         :1;
    cs_uint16 host0I         :1;
    cs_uint16 line3I         :1;
    cs_uint16 line2I         :1;
    cs_uint16 line1I         :1;
    cs_uint16 line0I         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 line0I         :1;
    cs_uint16 line1I         :1;
    cs_uint16 line2I         :1;
    cs_uint16 line3I         :1;
    cs_uint16 host0I         :1;
    cs_uint16 host1I         :1;
    cs_uint16 host2I         :1;
    cs_uint16 host3I         :1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_DATAPATH_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 host3E         :1;
    cs_uint16 host2E         :1;
    cs_uint16 host1E         :1;
    cs_uint16 host0E         :1;
    cs_uint16 line3E         :1;
    cs_uint16 line2E         :1;
    cs_uint16 line1E         :1;
    cs_uint16 line0E         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 line0E         :1;
    cs_uint16 line1E         :1;
    cs_uint16 line2E         :1;
    cs_uint16 line3E         :1;
    cs_uint16 host0E         :1;
    cs_uint16 host1E         :1;
    cs_uint16 host2E         :1;
    cs_uint16 host3E         :1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GLOBAL_DATAPATH_INTE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 output_src_sel :3;
    cs_uint16 invert_output  :1;
    cs_uint16 invert_input   :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 outen          :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 pullup_1k      :1;
    cs_uint16 pullup_10k     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pullup_10k     :1;
    cs_uint16 pullup_1k      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 outen          :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 invert_input   :1;
    cs_uint16 invert_output  :1;
    cs_uint16 output_src_sel :3;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_INTERR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 output_src_sel :3;
    cs_uint16 invert_output  :1;
    cs_uint16 invert_input   :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 outen          :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 pullup_1k      :1;
    cs_uint16 pullup_10k     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pullup_10k     :1;
    cs_uint16 pullup_1k      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 outen          :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 invert_input   :1;
    cs_uint16 invert_output  :1;
    cs_uint16 output_src_sel :3;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 output_src_sel :3;
    cs_uint16 invert_output  :1;
    cs_uint16 invert_input   :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 outen          :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 pullup_1k      :1;
    cs_uint16 pullup_10k     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pullup_10k     :1;
    cs_uint16 pullup_1k      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 outen          :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 invert_input   :1;
    cs_uint16 invert_output  :1;
    cs_uint16 output_src_sel :3;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO2_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 output_src_sel :3;
    cs_uint16 invert_output  :1;
    cs_uint16 invert_input   :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 outen          :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 pullup_1k      :1;
    cs_uint16 pullup_10k     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pullup_10k     :1;
    cs_uint16 pullup_1k      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 outen          :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 invert_input   :1;
    cs_uint16 invert_output  :1;
    cs_uint16 output_src_sel :3;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO3_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 output_src_sel :3;
    cs_uint16 invert_output  :1;
    cs_uint16 invert_input   :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 outen          :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 pullup_1k      :1;
    cs_uint16 pullup_10k     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pullup_10k     :1;
    cs_uint16 pullup_1k      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 outen          :1;
    cs_uint16 outen_ovr      :1;
    cs_uint16 iovddsel_1v2   :1;
    cs_uint16 invert_input   :1;
    cs_uint16 invert_output  :1;
    cs_uint16 output_src_sel :3;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO4_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 sel            :3;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sel            :3;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO_INPUT0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 gpio4          :1;
    cs_uint16 gpio3          :1;
    cs_uint16 gpio2          :1;
    cs_uint16 gpio1          :1;
    cs_uint16 interr         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 interr         :1;
    cs_uint16 gpio1          :1;
    cs_uint16 gpio2          :1;
    cs_uint16 gpio3          :1;
    cs_uint16 gpio4          :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO_INPUT_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 gpio4          :1;
    cs_uint16 gpio3          :1;
    cs_uint16 gpio2          :1;
    cs_uint16 gpio1          :1;
    cs_uint16 interr         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 interr         :1;
    cs_uint16 gpio1          :1;
    cs_uint16 gpio2          :1;
    cs_uint16 gpio3          :1;
    cs_uint16 gpio4          :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO_INPUT_INTE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 gpio4S         :1;
    cs_uint16 gpio3S         :1;
    cs_uint16 gpio2S         :1;
    cs_uint16 gpio1S         :1;
    cs_uint16 interrS        :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 interrS        :1;
    cs_uint16 gpio1S         :1;
    cs_uint16 gpio2S         :1;
    cs_uint16 gpio3S         :1;
    cs_uint16 gpio4S         :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO_INPUT_INTS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 gpio4Z         :1;
    cs_uint16 gpio3Z         :1;
    cs_uint16 gpio2Z         :1;
    cs_uint16 gpio1Z         :1;
    cs_uint16 interrZ        :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 interrZ        :1;
    cs_uint16 gpio1Z         :1;
    cs_uint16 gpio2Z         :1;
    cs_uint16 gpio3Z         :1;
    cs_uint16 gpio4Z         :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_GPIO_GPIO_INPUT_INTZ_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 val            :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 val            :5;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_EFUSE_PDF_SKU_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 go             :1;
    cs_uint16 done           :1;
    cs_uint16 rwn            :1;
    cs_uint16 aia            :1;
    cs_uint16 addr           :12;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr           :12;
    cs_uint16 aia            :1;
    cs_uint16 rwn            :1;
    cs_uint16 done           :1;
    cs_uint16 go             :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_RAM_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 data1          :8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 data1          :8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_RAM_DATA1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 data0          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 data0          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_RAM_DATA0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 ps_mem_ecc_double:1;
    cs_uint16 ps_mem_ecc_single:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ps_mem_ecc_single:1;
    cs_uint16 ps_mem_ecc_double:1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 ei_mem_ecc_doubleE:1;
    cs_uint16 ei_mem_ecc_singleE:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ei_mem_ecc_singleE:1;
    cs_uint16 ei_mem_ecc_doubleE:1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_INTE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 ps_mem_ecc_doubleO:1;
    cs_uint16 ps_mem_ecc_singleO:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ps_mem_ecc_singleO:1;
    cs_uint16 ps_mem_ecc_doubleO:1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_INTO_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 ps_mem_ecc_doubleZ:1;
    cs_uint16 ps_mem_ecc_singleZ:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ps_mem_ecc_singleZ:1;
    cs_uint16 ps_mem_ecc_doubleZ:1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_MSEQ_PS_INTZ_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :10;
    cs_uint16 edc_msmif_sr   :1;
    cs_uint16 edc_mseq_sr    :1;
    cs_uint16 edc_dsp_sr     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 common_tx_sr   :1;
    cs_uint16 common_rx_sr   :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 common_rx_sr   :1;
    cs_uint16 common_tx_sr   :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 edc_dsp_sr     :1;
    cs_uint16 edc_mseq_sr    :1;
    cs_uint16 edc_msmif_sr   :1;
    cs_uint16 rsrvd1         :10;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_SOFT_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 mpif_tx_dotreg_sr:1;
    cs_uint16 mpif_rx_dotreg_sr:1;
    cs_uint16 mpif_sys_dotreg_sr:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 mpif_sys_dotreg_sr:1;
    cs_uint16 mpif_rx_dotreg_sr:1;
    cs_uint16 mpif_tx_dotreg_sr:1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 gepcs_tx_sr    :1;
    cs_uint16 gepcs_rx_sr    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 gepcs_rx_sr    :1;
    cs_uint16 gepcs_tx_sr    :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_GIGEPCS_SOFT_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 entireMSPowerUp:1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 clkpd          :1;
    cs_uint16 rsrvd2         :2;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd2         :2;
    cs_uint16 clkpd          :1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 entireMSPowerUp:1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_MSEQCLKCTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 value          :2;
    cs_uint16 override       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 override       :1;
    cs_uint16 value          :2;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_KR_AN_PORT_ADDR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 mode_sel       :2;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 mode_sel       :2;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_AN_MODE_SEL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :2;
    cs_uint16 kr_tp_flI      :1;
    cs_uint16 kr_tpI         :1;
    cs_uint16 kr_fec_txI     :1;
    cs_uint16 kr_fec_rxI     :1;
    cs_uint16 kr_an_txI      :1;
    cs_uint16 kr_an_rxI      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 xgpcs_rxI      :1;
    cs_uint16 egpcs_txI      :1;
    cs_uint16 egpcs_rxI      :1;
    cs_uint16 gepcsI         :1;
    cs_uint16 emdsI          :1;
    cs_uint16 sdsI           :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sdsI           :1;
    cs_uint16 emdsI          :1;
    cs_uint16 gepcsI         :1;
    cs_uint16 egpcs_rxI      :1;
    cs_uint16 egpcs_txI      :1;
    cs_uint16 xgpcs_rxI      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 kr_an_rxI      :1;
    cs_uint16 kr_an_txI      :1;
    cs_uint16 kr_fec_rxI     :1;
    cs_uint16 kr_fec_txI     :1;
    cs_uint16 kr_tpI         :1;
    cs_uint16 kr_tp_flI      :1;
    cs_uint16 rsrvd1         :2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :2;
    cs_uint16 kr_tp_flE      :1;
    cs_uint16 kr_tpE         :1;
    cs_uint16 kr_fec_txE     :1;
    cs_uint16 kr_fec_rxE     :1;
    cs_uint16 kr_an_txE      :1;
    cs_uint16 kr_an_rxE      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 xgpcs_rxE      :1;
    cs_uint16 egpcs_txE      :1;
    cs_uint16 egpcs_rxE      :1;
    cs_uint16 gepcsE         :1;
    cs_uint16 emdsE          :1;
    cs_uint16 sdsE           :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sdsE           :1;
    cs_uint16 emdsE          :1;
    cs_uint16 gepcsE         :1;
    cs_uint16 egpcs_rxE      :1;
    cs_uint16 egpcs_txE      :1;
    cs_uint16 xgpcs_rxE      :1;
    cs_uint16 rsrvd2         :2;
    cs_uint16 kr_an_rxE      :1;
    cs_uint16 kr_an_txE      :1;
    cs_uint16 kr_fec_rxE     :1;
    cs_uint16 kr_fec_txE     :1;
    cs_uint16 kr_tpE         :1;
    cs_uint16 kr_tp_flE      :1;
    cs_uint16 rsrvd1         :2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_LINEMISC_INTENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 DIG_CLK_RX_CKEN:1;
    cs_uint16 bitswap        :1;
    cs_uint16 bitinv         :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 data_source    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 data_source    :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 bitinv         :1;
    cs_uint16 bitswap        :1;
    cs_uint16 DIG_CLK_RX_CKEN:1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RX0_Config_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :3;
    cs_uint16 SRX_2X2_DMUX_EN:1;
    cs_uint16 SRX_POLARITYINV:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_FCEN       :1;
    cs_uint16 SRX_AMP_PD     :1;
    cs_uint16 SRX_PD         :1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_BUSWIDTH   :2;
    cs_uint16 rsrvd4         :1;
    cs_uint16 SRX_LPBK_EN    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_LPBK_EN    :1;
    cs_uint16 rsrvd4         :1;
    cs_uint16 SRX_BUSWIDTH   :2;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_PD         :1;
    cs_uint16 SRX_AMP_PD     :1;
    cs_uint16 SRX_FCEN       :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_POLARITYINV:1;
    cs_uint16 SRX_2X2_DMUX_EN:1;
    cs_uint16 rsrvd1         :3;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CONFIG_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_FASTDIV_SEL:4;
    cs_uint16 SRX_CTRDIV_SEL :2;
    cs_uint16 SRX_CTVDIV_SEL :2;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_DDIV_SEL   :3;
    cs_uint16 SRX_RDIV_SEL   :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIV_SEL   :4;
    cs_uint16 SRX_DDIV_SEL   :3;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_CTVDIV_SEL :2;
    cs_uint16 SRX_CTRDIV_SEL :2;
    cs_uint16 SRX_FASTDIV_SEL:4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CLKDIV_CTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_REFCLK_SEL :1;
    cs_uint16 SRX_DIV64OUT_EN:1;
    cs_uint16 SRX_EXT_SERCLK_EN:1;
    cs_uint16 SRX_CLKBUF_CMFBOVR_EN:1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_CLKOUTEN   :1;
    cs_uint16 SRX_CLKEN      :1;
    cs_uint16 SRX_CLKINVEN   :1;
    cs_uint16 SRX_TCLKOUT_SEL:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_TCLKOUT_SEL:8;
    cs_uint16 SRX_CLKINVEN   :1;
    cs_uint16 SRX_CLKEN      :1;
    cs_uint16 SRX_CLKOUTEN   :1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_CLKBUF_CMFBOVR_EN:1;
    cs_uint16 SRX_EXT_SERCLK_EN:1;
    cs_uint16 SRX_DIV64OUT_EN:1;
    cs_uint16 SRX_REFCLK_SEL :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CLKOUT_CTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_CPTSTUP    :1;
    cs_uint16 SRX_CPTSTDN    :1;
    cs_uint16 rsrvd1         :6;
    cs_uint16 SRX_CPCUR1     :4;
    cs_uint16 SRX_CPCUR0     :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_CPCUR0     :4;
    cs_uint16 SRX_CPCUR1     :4;
    cs_uint16 rsrvd1         :6;
    cs_uint16 SRX_CPTSTDN    :1;
    cs_uint16 SRX_CPTSTUP    :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CPA_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :1;
    cs_uint16 REF_156MHZ     :1;
    cs_uint16 FILT_SCALE     :2;
    cs_uint16 VCO_SCALE      :2;
    cs_uint16 REF_SCALE      :2;
    cs_uint16 DATA_FILTLOCK  :1;
    cs_uint16 FORCE_FILTLOCK :1;
    cs_uint16 INIT           :1;
    cs_uint16 DATA_LOCK      :1;
    cs_uint16 SHORT_OVRIDE   :2;
    cs_uint16 PD_MODE        :1;
    cs_uint16 FORCE_LOCK     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FORCE_LOCK     :1;
    cs_uint16 PD_MODE        :1;
    cs_uint16 SHORT_OVRIDE   :2;
    cs_uint16 DATA_LOCK      :1;
    cs_uint16 INIT           :1;
    cs_uint16 FORCE_FILTLOCK :1;
    cs_uint16 DATA_FILTLOCK  :1;
    cs_uint16 REF_SCALE      :2;
    cs_uint16 VCO_SCALE      :2;
    cs_uint16 FILT_SCALE     :2;
    cs_uint16 REF_156MHZ     :1;
    cs_uint16 rsrvd1         :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSi:1;
    cs_uint16 FILT_LOCKi     :1;
    cs_uint16 CIDONEi        :1;
    cs_uint16 CIDZEROi       :1;
    cs_uint16 DATA_DETi      :1;
    cs_uint16 SHORT_FILTPNi  :1;
    cs_uint16 BAD_VCO_CLKi   :1;
    cs_uint16 LOCKi          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKi          :1;
    cs_uint16 BAD_VCO_CLKi   :1;
    cs_uint16 SHORT_FILTPNi  :1;
    cs_uint16 DATA_DETi      :1;
    cs_uint16 CIDZEROi       :1;
    cs_uint16 CIDONEi        :1;
    cs_uint16 FILT_LOCKi     :1;
    cs_uint16 CLKLESS_STATUSi:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSs:1;
    cs_uint16 FILT_LOCKs     :1;
    cs_uint16 CIDONEs        :1;
    cs_uint16 CIDZEROs       :1;
    cs_uint16 DATA_DETs      :1;
    cs_uint16 SHORT_FILTPNs  :1;
    cs_uint16 BAD_VCO_CLKs   :1;
    cs_uint16 LOCKs          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKs          :1;
    cs_uint16 BAD_VCO_CLKs   :1;
    cs_uint16 SHORT_FILTPNs  :1;
    cs_uint16 DATA_DETs      :1;
    cs_uint16 CIDZEROs       :1;
    cs_uint16 CIDONEs        :1;
    cs_uint16 FILT_LOCKs     :1;
    cs_uint16 CLKLESS_STATUSs:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSe:1;
    cs_uint16 FILT_LOCKe     :1;
    cs_uint16 CIDONEe        :1;
    cs_uint16 CIDZEROe       :1;
    cs_uint16 DATA_DETe      :1;
    cs_uint16 SHORT_FILTPNe  :1;
    cs_uint16 BAD_VCO_CLKe   :1;
    cs_uint16 LOCKe          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKe          :1;
    cs_uint16 BAD_VCO_CLKe   :1;
    cs_uint16 SHORT_FILTPNe  :1;
    cs_uint16 DATA_DETe      :1;
    cs_uint16 CIDZEROe       :1;
    cs_uint16 CIDONEe        :1;
    cs_uint16 FILT_LOCKe     :1;
    cs_uint16 CLKLESS_STATUSe:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSiZ:1;
    cs_uint16 FILT_LOCKiZ    :1;
    cs_uint16 CIDONEiZ       :1;
    cs_uint16 CIDZEROiZ      :1;
    cs_uint16 DATA_DETiZ     :1;
    cs_uint16 SHORT_FILTPNiZ :1;
    cs_uint16 BAD_VCO_CLKiZ  :1;
    cs_uint16 LOCKiZ         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKiZ         :1;
    cs_uint16 BAD_VCO_CLKiZ  :1;
    cs_uint16 SHORT_FILTPNiZ :1;
    cs_uint16 DATA_DETiZ     :1;
    cs_uint16 CIDZEROiZ      :1;
    cs_uint16 CIDONEiZ       :1;
    cs_uint16 FILT_LOCKiZ    :1;
    cs_uint16 CLKLESS_STATUSiZ:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPTZ_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 datawidth      :8;
    cs_uint16 rsrvd1         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd2         :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd1         :1;
    cs_uint16 datawidth      :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Cfg_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 oosThresh      :8;
    cs_uint16 rsrvd1         :3;
    cs_uint16 autoPolarityThresh:3;
    cs_uint16 autoPolarityEn :1;
    cs_uint16 loadPrbsState  :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 loadPrbsState  :1;
    cs_uint16 autoPolarityEn :1;
    cs_uint16 autoPolarityThresh:3;
    cs_uint16 rsrvd1         :3;
    cs_uint16 oosThresh      :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Ctrl_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 VALUE1         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 VALUE1         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Count1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 VALUE0         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 VALUE0         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_Count0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 AUTOPOLARITYi  :1;
    cs_uint16 PRBS_RX_EFi    :1;
    cs_uint16 PRBS_SYNCi     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_SYNCi     :1;
    cs_uint16 PRBS_RX_EFi    :1;
    cs_uint16 AUTOPOLARITYi  :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 AUTOPOLARITYs  :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 PRBS_SYNCs     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_SYNCs     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 AUTOPOLARITYs  :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSCHK0_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 SRX_RDIVFRAC_TESTCTRL:2;
    cs_uint16 SRX_COREFRAC_TESTCTRL:2;
    cs_uint16 rsrvd2         :6;
    cs_uint16 SRX_COREFRAC_RESET:1;
    cs_uint16 SRX_RDIVFRAC_RESET:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_RESET:1;
    cs_uint16 SRX_COREFRAC_RESET:1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 SRX_COREFRAC_TESTCTRL:2;
    cs_uint16 SRX_RDIVFRAC_TESTCTRL:2;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_FRAC0_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :5;
    cs_uint16 SRX_COREFRAC_VALIDB:1;
    cs_uint16 SRX_COREFRAC_TEST_EN:1;
    cs_uint16 SRX_COREFRAC_EN:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_RDIVFRAC_VALIDB:1;
    cs_uint16 SRX_RDIVFRAC_TEST_EN:1;
    cs_uint16 rsrvd3         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_RDIVFRAC_TEST_EN:1;
    cs_uint16 SRX_RDIVFRAC_VALIDB:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_COREFRAC_EN:1;
    cs_uint16 SRX_COREFRAC_TEST_EN:1;
    cs_uint16 SRX_COREFRAC_VALIDB:1;
    cs_uint16 rsrvd1         :5;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_FRAC0_EN_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_RDIVFRAC_NUMERATOR0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_NUMERATOR0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_NUMERATOR0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_RDIVFRAC_NUMERATOR1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_NUMERATOR1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_NUMERATOR1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_NUMERATOR0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_NUMERATOR0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_COREFRAC0_NUMERATOR0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_COREFRAC_NUMERATOR1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_NUMERATOR1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_COREFRAC0_NUMERATOR1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :12;
    cs_uint16 SRX_COREFRAC_WIDTH:2;
    cs_uint16 SRX_RDIVFRAC_WIDTH:2;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_WIDTH:2;
    cs_uint16 SRX_COREFRAC_WIDTH:2;
    cs_uint16 rsrvd1         :12;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_FRAC0_WIDTH_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_INTDIV:8;
    cs_uint16 SRX_RDIVFRAC_INTDIV:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_INTDIV:8;
    cs_uint16 SRX_COREFRAC_INTDIV:8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_FRAC0_INTDIV_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :5;
    cs_uint16 SRX_COREFRAC_STAGE1_EN:1;
    cs_uint16 SRX_COREFRAC_STAGE2_EN:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_RDIVFRAC_STAGE1_EN:1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_RDIVFRAC_STAGE2_EN:1;
    cs_uint16 rsrvd4         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd4         :1;
    cs_uint16 SRX_RDIVFRAC_STAGE2_EN:1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_RDIVFRAC_STAGE1_EN:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_COREFRAC_STAGE2_EN:1;
    cs_uint16 SRX_COREFRAC_STAGE1_EN:1;
    cs_uint16 rsrvd1         :5;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_FRAC0_STAGE_EN_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_RDIVFRAC_STAGE2PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_STAGE2PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RDIVFRAC0_STAGE2PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE1PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE1PRELOAD1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_STAGE2PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_STAGE2PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_COREFRAC0_STAGE2PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :2;
    cs_uint16 SRX_COREFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_COREFRAC_DITHER_EN:1;
    cs_uint16 rsrvd3         :2;
    cs_uint16 SRX_RDIVFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd4         :3;
    cs_uint16 SRX_RDIVFRAC_DITHER_EN:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_DITHER_EN:1;
    cs_uint16 rsrvd4         :3;
    cs_uint16 SRX_RDIVFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd3         :2;
    cs_uint16 SRX_COREFRAC_DITHER_EN:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_COREFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd1         :2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_FRAC0_DITHER_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :1;
    cs_uint16 STX_CMODE_RES  :3;
    cs_uint16 STX_DRV_LOWER_CM:4;
    cs_uint16 rsrvd2         :2;
    cs_uint16 STX_LEVEL      :6;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_LEVEL      :6;
    cs_uint16 rsrvd2         :2;
    cs_uint16 STX_DRV_LOWER_CM:4;
    cs_uint16 STX_CMODE_RES  :3;
    cs_uint16 rsrvd1         :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STX_SLEW_RATE_ADJ:2;
    cs_uint16 rsrvd1         :1;
    cs_uint16 STX_PRE_PEAK   :5;
    cs_uint16 STX_MUXSUBRATE_SEL:1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 STX_POST_PEAK  :6;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_POST_PEAK  :6;
    cs_uint16 rsrvd2         :1;
    cs_uint16 STX_MUXSUBRATE_SEL:1;
    cs_uint16 STX_PRE_PEAK   :5;
    cs_uint16 rsrvd1         :1;
    cs_uint16 STX_SLEW_RATE_ADJ:2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 STX_SQUELCH    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_SQUELCH    :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_STX0_SQUELCH_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 datawidth      :8;
    cs_uint16 rsrvd1         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd2         :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 fixed_pattern  :1;
    cs_uint16 prbs_enable    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 prbs_enable    :1;
    cs_uint16 fixed_pattern  :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd1         :1;
    cs_uint16 datawidth      :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Cfg_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :12;
    cs_uint16 insertError    :1;
    cs_uint16 insertOneBitError:1;
    cs_uint16 insertErrorOnce:1;
    cs_uint16 insertOneBitErrorOnce:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 insertOneBitErrorOnce:1;
    cs_uint16 insertErrorOnce:1;
    cs_uint16 insertOneBitError:1;
    cs_uint16 insertError    :1;
    cs_uint16 rsrvd1         :12;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Ctrl_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 VALUE1         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 VALUE1         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_PRBSGEN0_Fixed0_Pattern1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 SRX_AGC_ENB_OFFSETMON:1;
    cs_uint16 SRX_AGC_ENB_OFFSET:1;
    cs_uint16 SRX_AGC_ENB_LIMAMP:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_AGC_ENB_LIMAMP:1;
    cs_uint16 SRX_AGC_ENB_OFFSET:1;
    cs_uint16 SRX_AGC_ENB_OFFSETMON:1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_SRX0_AGC_CONFIG1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 DFPROTECTi     :1;
    cs_uint16 TXELSTi        :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 PRBSCHKi       :1;
    cs_uint16 RXELSTi        :1;
    cs_uint16 RXLOCKDi       :1;
    cs_uint16 RXVCOi         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXVCOi         :1;
    cs_uint16 RXLOCKDi       :1;
    cs_uint16 RXELSTi        :1;
    cs_uint16 PRBSCHKi       :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 TXELSTi        :1;
    cs_uint16 DFPROTECTi     :1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_Int_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 DFPROTECTe     :1;
    cs_uint16 TXELSTe        :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 PRBSCHKe       :1;
    cs_uint16 RXELSTe        :1;
    cs_uint16 RXLOCKDe       :1;
    cs_uint16 RXVCOe         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXVCOe         :1;
    cs_uint16 RXLOCKDe       :1;
    cs_uint16 RXELSTe        :1;
    cs_uint16 PRBSCHKe       :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 TXELSTe        :1;
    cs_uint16 DFPROTECTe     :1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_IntEn_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 RXLOCKD0i      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXLOCKD0i      :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKDi_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 RXLOCKD0e      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXLOCKD0e      :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_COMMON_RXLOCKDe_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SEL            :4;
    cs_uint16 GRWN           :1;
    cs_uint16 GRAM_A         :11;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GRAM_A         :11;
    cs_uint16 GRWN           :1;
    cs_uint16 SEL            :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_CR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GRAM_D1        :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GRAM_D1        :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_D1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GRAM_D0        :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GRAM_D0        :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_GRAM_D0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MSEQ_pu_state  :1;
    cs_uint16 rsrvd1         :6;
    cs_uint16 PC_state       :9;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PC_state       :9;
    cs_uint16 rsrvd1         :6;
    cs_uint16 MSEQ_pu_state  :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_PC_SHADOW_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MSEQ_pu_state  :1;
    cs_uint16 rsrvd1         :11;
    cs_uint16 OPTIONS_state  :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 OPTIONS_state  :4;
    cs_uint16 rsrvd1         :11;
    cs_uint16 MSEQ_pu_state  :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_SHADOW_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :12;
    cs_uint16 STALL          :1;
    cs_uint16 ALL_CHK_EN     :1;
    cs_uint16 IN_PAR_ENB     :1;
    cs_uint16 EX_PAR_ENB     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EX_PAR_ENB     :1;
    cs_uint16 IN_PAR_ENB     :1;
    cs_uint16 ALL_CHK_EN     :1;
    cs_uint16 STALL          :1;
    cs_uint16 rsrvd1         :12;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :7;
    cs_uint16 CAL_DIGITAL_S  :1;
    cs_uint16 CAL_ANALOG_S   :1;
    cs_uint16 BAD_STATE_S    :1;
    cs_uint16 EDC_CONVERGED_S:1;
    cs_uint16 EXTERNAL_LOS_S :1;
    cs_uint16 LOCAL_LOCKD_S  :1;
    cs_uint16 DUPLEX_LOCKD_S :1;
    cs_uint16 LOCAL_LOS_S    :1;
    cs_uint16 DUPLEX_LOS_S   :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DUPLEX_LOS_S   :1;
    cs_uint16 LOCAL_LOS_S    :1;
    cs_uint16 DUPLEX_LOCKD_S :1;
    cs_uint16 LOCAL_LOCKD_S  :1;
    cs_uint16 EXTERNAL_LOS_S :1;
    cs_uint16 EDC_CONVERGED_S:1;
    cs_uint16 BAD_STATE_S    :1;
    cs_uint16 CAL_ANALOG_S   :1;
    cs_uint16 CAL_DIGITAL_S  :1;
    cs_uint16 rsrvd1         :7;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 bank_select_reg:3;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bank_select_reg:3;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_BANK_SELECTOR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_DATA_SLICE_LEVEL:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_DATA_SLICE_LEVEL:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_SLICER_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset          :1;
    cs_uint16 Loopback       :1;
    cs_uint16 SpLSB          :1;
    cs_uint16 AutoNegEn      :1;
    cs_uint16 PwrDn          :1;
    cs_uint16 Isolate        :1;
    cs_uint16 AutoNegRestart :1;
    cs_uint16 DuplexMode     :1;
    cs_uint16 ColTest        :1;
    cs_uint16 SpMSB          :1;
    cs_uint16 ResetStats     :1;
    cs_uint16 AutoNegIdleDetectDuration:1;
    cs_uint16 rsrvd1         :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1         :4;
    cs_uint16 AutoNegIdleDetectDuration:1;
    cs_uint16 ResetStats     :1;
    cs_uint16 SpMSB          :1;
    cs_uint16 ColTest        :1;
    cs_uint16 DuplexMode     :1;
    cs_uint16 AutoNegRestart :1;
    cs_uint16 Isolate        :1;
    cs_uint16 PwrDn          :1;
    cs_uint16 AutoNegEn      :1;
    cs_uint16 SpLSB          :1;
    cs_uint16 Loopback       :1;
    cs_uint16 Reset          :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_GIGEPCS_LINE_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 One00BT4       :1;
    cs_uint16 One00BXFD      :1;
    cs_uint16 One00BXHD      :1;
    cs_uint16 One0FD         :1;
    cs_uint16 One0HD         :1;
    cs_uint16 One00BT2FD     :1;
    cs_uint16 One00BT2HD     :1;
    cs_uint16 ExtStat        :1;
    cs_uint16 RSVD0          :1;
    cs_uint16 RSVD1          :1;
    cs_uint16 AutoNegComplete:1;
    cs_uint16 RmtFault       :1;
    cs_uint16 AutoNegAb      :1;
    cs_uint16 LinkStat       :1;
    cs_uint16 Jabber         :1;
    cs_uint16 XtCap          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 XtCap          :1;
    cs_uint16 Jabber         :1;
    cs_uint16 LinkStat       :1;
    cs_uint16 AutoNegAb      :1;
    cs_uint16 RmtFault       :1;
    cs_uint16 AutoNegComplete:1;
    cs_uint16 RSVD1          :1;
    cs_uint16 RSVD0          :1;
    cs_uint16 ExtStat        :1;
    cs_uint16 One00BT2HD     :1;
    cs_uint16 One00BT2FD     :1;
    cs_uint16 One0HD         :1;
    cs_uint16 One0FD         :1;
    cs_uint16 One00BXHD      :1;
    cs_uint16 One00BXFD      :1;
    cs_uint16 One00BT4       :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_GIGEPCS_LINE_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 txgb_oflwI     :1;
    cs_uint16 txgb_uflwI     :1;
    cs_uint16 rxgb_oflwI     :1;
    cs_uint16 rxgb_uflwI     :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 halt_tx_on_lfI :1;
    cs_uint16 an_page_receiveI:1;
    cs_uint16 an_doneI       :1;
    cs_uint16 phy_linkI      :1;
    cs_uint16 linkI          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 linkI          :1;
    cs_uint16 phy_linkI      :1;
    cs_uint16 an_doneI       :1;
    cs_uint16 an_page_receiveI:1;
    cs_uint16 halt_tx_on_lfI :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 rxgb_uflwI     :1;
    cs_uint16 rxgb_oflwI     :1;
    cs_uint16 txgb_uflwI     :1;
    cs_uint16 txgb_oflwI     :1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_GIGEPCS_INT_LINE_PCS1GE_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 halt_tx_on_lfS :1;
    cs_uint16 an_page_receiveS:1;
    cs_uint16 an_doneS       :1;
    cs_uint16 phy_linkS      :1;
    cs_uint16 linkS          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 linkS          :1;
    cs_uint16 phy_linkS      :1;
    cs_uint16 an_doneS       :1;
    cs_uint16 an_page_receiveS:1;
    cs_uint16 halt_tx_on_lfS :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_GIGEPCS_INT_LINE_PCS1GE_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rxrst          :1;
    cs_uint16 rsrvd1         :13;
    cs_uint16 st_char_err    :1;
    cs_uint16 align_char     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 align_char     :1;
    cs_uint16 st_char_err    :1;
    cs_uint16 rsrvd1         :13;
    cs_uint16 rxrst          :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_EGPCS_RX_MODE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 syncI          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 syncI          :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_EGPCS_RX_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 syncS          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 syncS          :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_EGPCS_RX_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 txrst          :1;
    cs_uint16 rsrvd1         :9;
    cs_uint16 post_enc_rd_sel:2;
    cs_uint16 rsrvd2         :3;
    cs_uint16 ins_mode       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ins_mode       :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 post_enc_rd_sel:2;
    cs_uint16 rsrvd1         :9;
    cs_uint16 txrst          :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_EGPCS_TX_MODE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 fifo_uflwI     :1;
    cs_uint16 fifo_oflwI     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fifo_oflwI     :1;
    cs_uint16 fifo_uflwI     :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_EGPCS_TX_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 logicReset     :1;
    cs_uint16 rsrvd1         :11;
    cs_uint16 scram_tx_dis   :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 sync_hdr       :2;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sync_hdr       :2;
    cs_uint16 rsrvd2         :1;
    cs_uint16 scram_tx_dis   :1;
    cs_uint16 rsrvd1         :11;
    cs_uint16 logicReset     :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_XGPCS_TX_TXCNTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 logicReset     :1;
    cs_uint16 dontFilterFsig :1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 latchEach1e    :1;
    cs_uint16 rsrvd2         :4;
    cs_uint16 pcs6_ddsa_enable:1;
    cs_uint16 rsrvd3         :3;
    cs_uint16 descram_rx_dis :1;
    cs_uint16 rsrvd4         :2;
    cs_uint16 fc_st_decerr   :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fc_st_decerr   :1;
    cs_uint16 rsrvd4         :2;
    cs_uint16 descram_rx_dis :1;
    cs_uint16 rsrvd3         :3;
    cs_uint16 pcs6_ddsa_enable:1;
    cs_uint16 rsrvd2         :4;
    cs_uint16 latchEach1e    :1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 dontFilterFsig :1;
    cs_uint16 logicReset     :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_XGPCS_RX_RXCNTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :6;
    cs_uint16 berhighS       :1;
    cs_uint16 syncdetS       :1;
    cs_uint16 rsrvd2         :8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd2         :8;
    cs_uint16 syncdetS       :1;
    cs_uint16 berhighS       :1;
    cs_uint16 rsrvd1         :6;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_XGPCS_RX_RXSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :5;
    cs_uint16 decerrI        :1;
    cs_uint16 berhighI       :1;
    cs_uint16 syncdetI       :1;
    cs_uint16 rsrvd2         :8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd2         :8;
    cs_uint16 syncdetI       :1;
    cs_uint16 berhighI       :1;
    cs_uint16 decerrI        :1;
    cs_uint16 rsrvd1         :5;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_XGPCS_RX_RXINT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_ENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 an_good_check  :1;
    cs_uint16 an_complete    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 an_complete    :1;
    cs_uint16 an_good_check  :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_COMPLETE_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 an_good_check  :1;
    cs_uint16 an_complete    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 an_complete    :1;
    cs_uint16 an_good_check  :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_AN_COMPLETE_STATUS_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :2;
    cs_uint16 rf             :1;
    cs_uint16 pause          :3;
    cs_uint16 rsrvd2         :3;
    cs_uint16 fec            :2;
    cs_uint16 selector       :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 selector       :5;
    cs_uint16 fec            :2;
    cs_uint16 rsrvd2         :3;
    cs_uint16 pause          :3;
    cs_uint16 rf             :1;
    cs_uint16 rsrvd1         :2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_ADV_ABILITY_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tech_ability   :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tech_ability   :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_TECH_ABILITY_1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :6;
    cs_uint16 parallel_detect:1;
    cs_uint16 bp_100gcr10    :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 bp_40gcr4      :1;
    cs_uint16 bp_40gkr4      :1;
    cs_uint16 bp_fec         :1;
    cs_uint16 bp_10gkr       :1;
    cs_uint16 bp_10gkx4      :1;
    cs_uint16 bp_1000kx      :1;
    cs_uint16 bp_an_ability  :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bp_an_ability  :1;
    cs_uint16 bp_1000kx      :1;
    cs_uint16 bp_10gkx4      :1;
    cs_uint16 bp_10gkr       :1;
    cs_uint16 bp_fec         :1;
    cs_uint16 bp_40gkr4      :1;
    cs_uint16 bp_40gcr4      :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 bp_100gcr10    :1;
    cs_uint16 parallel_detect:1;
    cs_uint16 rsrvd1         :6;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_BP_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value0         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value0         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_LINK_FAIL_TIMEOUT0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 en             :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 en             :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_ABILITY_DETECT_TIMER_ENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :10;
    cs_uint16 an_complete_status:1;
    cs_uint16 link_fail_inhibit_timer:1;
    cs_uint16 ability_detect_stuck:1;
    cs_uint16 attempts_failed:1;
    cs_uint16 incompatible_link:1;
    cs_uint16 tx_afifo       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tx_afifo       :1;
    cs_uint16 incompatible_link:1;
    cs_uint16 attempts_failed:1;
    cs_uint16 ability_detect_stuck:1;
    cs_uint16 link_fail_inhibit_timer:1;
    cs_uint16 an_complete_status:1;
    cs_uint16 rsrvd1         :10;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_MAIN_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :10;
    cs_uint16 an_complete_statusS:1;
    cs_uint16 link_fail_inhibit_timerS:1;
    cs_uint16 ability_detect_stuckS:1;
    cs_uint16 attempts_failedS:1;
    cs_uint16 incompatible_linkS:1;
    cs_uint16 tx_afifoS      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tx_afifoS      :1;
    cs_uint16 incompatible_linkS:1;
    cs_uint16 attempts_failedS:1;
    cs_uint16 ability_detect_stuckS:1;
    cs_uint16 link_fail_inhibit_timerS:1;
    cs_uint16 an_complete_statusS:1;
    cs_uint16 rsrvd1         :10;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_MAIN_INTS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 underflow      :1;
    cs_uint16 overflow       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 overflow       :1;
    cs_uint16 underflow      :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_TX_TX_AFIFO_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 page_recieved  :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 page_recieved  :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_RX_PAGE_RECEIVED_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 np             :1;
    cs_uint16 ack            :1;
    cs_uint16 rf             :1;
    cs_uint16 pause          :3;
    cs_uint16 e_nonce        :5;
    cs_uint16 selector       :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 selector       :5;
    cs_uint16 e_nonce        :5;
    cs_uint16 pause          :3;
    cs_uint16 rf             :1;
    cs_uint16 ack            :1;
    cs_uint16 np             :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_RX_LP_ABILITY_1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tech_ability   :11;
    cs_uint16 t_nonce        :5;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 t_nonce        :5;
    cs_uint16 tech_ability   :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_RX_LP_ABILITY_2_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 rx_afifo       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rx_afifo       :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_RX_MAIN_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 underflow      :1;
    cs_uint16 overflow       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 overflow       :1;
    cs_uint16 underflow      :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_AN_RX_RX_AFIFO_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 fc_enable      :1;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 fc_enable      :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_ENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 rx             :1;
    cs_uint16 tx             :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tx             :1;
    cs_uint16 rx             :1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_TX_RX_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_BITSWAP_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 status         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 status         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_TRAINING_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 trans_trace_ld_afifo_oflow:1;
    cs_uint16 trans_trace_lp_afifo_oflow:1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 training_complete:1;
    cs_uint16 training_failure:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 training_failure:1;
    cs_uint16 training_complete:1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 trans_trace_lp_afifo_oflow:1;
    cs_uint16 trans_trace_ld_afifo_oflow:1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_TRAINING_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 trans_trace_ld_afifo_oflowS:1;
    cs_uint16 trans_trace_lp_afifo_oflowS:1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 training_completeS:1;
    cs_uint16 training_failureS:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 training_failureS:1;
    cs_uint16 training_completeS:1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 trans_trace_lp_afifo_oflowS:1;
    cs_uint16 trans_trace_ld_afifo_oflowS:1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_TRAINING_INTS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value1         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value1         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMEOUT1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value0         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value0         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAX_WAIT_TIMEOUT0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_WAIT_TIMEOUT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_ADDITIONAL_FRAMES_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRE_COEFF_MAX_LIMIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAIN_COEFF_MAX_LIMIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_POST_COEFF_MAX_LIMIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRE_COEFF_MIN_LIMIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAIN_COEFF_MIN_LIMIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_POST_COEFF_MIN_LIMIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRE_COEFF_INIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAIN_COEFF_INIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_POST_COEFF_INIT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRESET_COEFF_OVERRIDE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRE_COEFF_PRESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAIN_COEFF_PRESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_POST_COEFF_PRESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRE_COEFF_START_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAIN_COEFF_START_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_POST_COEFF_START_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_PRE_COEFF_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_MAIN_COEFF_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value          :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value          :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_POST_COEFF_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 post           :4;
    cs_uint16 main           :4;
    cs_uint16 pre            :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pre            :4;
    cs_uint16 main           :4;
    cs_uint16 post           :4;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_COEFF_STEP_VALUE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 use_reg        :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 use_reg        :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_TX_LD_COEFF_UPDATE_CTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_RX_BITSWAP_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :3;
    cs_uint16 clear_on_no_frame_lock:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 include_bad_markers:1;
    cs_uint16 limit_down     :4;
    cs_uint16 limit_up       :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 limit_up       :4;
    cs_uint16 limit_down     :4;
    cs_uint16 include_bad_markers:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 clear_on_no_frame_lock:1;
    cs_uint16 rsrvd1         :3;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_RX_REMOTE_RX_READY_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 frame_lock     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 frame_lock     :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_RX_FRAME_LOCK_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 frame_lockS    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 frame_lockS    :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_TP_RX_FRAME_LOCK_INTS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 tx_rst         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tx_rst         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_TX_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 fec_enable_pin :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 fec_sync_detected:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fec_sync_detected:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 fec_enable_pin :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_TX_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 stats_capture  :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 stats_capture  :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt1           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt1           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt0           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt0           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 tx_sync        :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tx_sync        :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_TX_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 rx_rst         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rx_rst         :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 fec_enable_pin :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 fec_sync_detected:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fec_sync_detected:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 fec_enable_pin :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 stats_capture  :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 stats_capture  :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt1           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt1           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt0           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt0           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt1           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt1           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_CORR1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt0           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt0           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_CORR0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt1           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt1           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_UNCORR1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt0           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt0           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_UNCORR0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt1           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt1           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ZERO_ERRS1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt0           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt0           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ZERO_ERRS0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt1           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt1           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ONE_ERRS1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt0           :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt0           :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BIT_ONE_ERRS0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 ei_mem_ecc_double:1;
    cs_uint16 ei_mem_ecc_single:1;
    cs_uint16 meg_mem_ecc_double:1;
    cs_uint16 meg_mem_ecc_single:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 eee_rapidsync_timeout:1;
    cs_uint16 data_ptrn      :1;
    cs_uint16 err_ptrn       :1;
    cs_uint16 rx_sync_fail   :1;
    cs_uint16 rx_sync        :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rx_sync        :1;
    cs_uint16 rx_sync_fail   :1;
    cs_uint16 err_ptrn       :1;
    cs_uint16 data_ptrn      :1;
    cs_uint16 eee_rapidsync_timeout:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 meg_mem_ecc_single:1;
    cs_uint16 meg_mem_ecc_double:1;
    cs_uint16 ei_mem_ecc_single:1;
    cs_uint16 ei_mem_ecc_double:1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_LINE_KR_FEC_RX_INT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :10;
    cs_uint16 edc_msmif_sr   :1;
    cs_uint16 edc_mseq_sr    :1;
    cs_uint16 edc_dsp_sr     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 common_tx_sr   :1;
    cs_uint16 common_rx_sr   :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 common_rx_sr   :1;
    cs_uint16 common_tx_sr   :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 edc_dsp_sr     :1;
    cs_uint16 edc_mseq_sr    :1;
    cs_uint16 edc_msmif_sr   :1;
    cs_uint16 rsrvd1         :10;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_SOFT_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :14;
    cs_uint16 mpif_rx_dotreg_sr:1;
    cs_uint16 mpif_sys_dotreg_sr:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 mpif_sys_dotreg_sr:1;
    cs_uint16 mpif_rx_dotreg_sr:1;
    cs_uint16 rsrvd1         :14;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_MPIF_RESET_DOTREG_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 gepcs_rx_sr    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 gepcs_rx_sr    :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_GIGEPCS_SOFT_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :6;
    cs_uint16 tx_sel_mate_clk:1;
    cs_uint16 rx_sel_mate_clk:1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 tx_sel_mate_data:1;
    cs_uint16 rx_sel_mate_data:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rx_sel_mate_data:1;
    cs_uint16 tx_sel_mate_data:1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 rx_sel_mate_clk:1;
    cs_uint16 tx_sel_mate_clk:1;
    cs_uint16 rsrvd1         :6;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_MATE_SELECT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 entireMSPowerUp:1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 clkpd          :1;
    cs_uint16 rsrvd2         :2;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd2         :2;
    cs_uint16 clkpd          :1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 entireMSPowerUp:1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_MSEQCLKCTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 gepcsI         :1;
    cs_uint16 emdsI          :1;
    cs_uint16 sdsI           :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sdsI           :1;
    cs_uint16 emdsI          :1;
    cs_uint16 gepcsI         :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 gepcsE         :1;
    cs_uint16 emdsE          :1;
    cs_uint16 sdsE           :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sdsE           :1;
    cs_uint16 emdsE          :1;
    cs_uint16 gepcsE         :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_HOSTMISC_INTENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 DIG_CLK_RX_CKEN:1;
    cs_uint16 bitswap        :1;
    cs_uint16 bitinv         :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 data_source    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 data_source    :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 bitinv         :1;
    cs_uint16 bitswap        :1;
    cs_uint16 DIG_CLK_RX_CKEN:1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RX0_Config_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :3;
    cs_uint16 SRX_2X2_DMUX_EN:1;
    cs_uint16 SRX_POLARITYINV:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_FCEN       :1;
    cs_uint16 SRX_AMP_PD     :1;
    cs_uint16 SRX_PD         :1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_BUSWIDTH   :2;
    cs_uint16 rsrvd4         :1;
    cs_uint16 SRX_LPBK_EN    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_LPBK_EN    :1;
    cs_uint16 rsrvd4         :1;
    cs_uint16 SRX_BUSWIDTH   :2;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_PD         :1;
    cs_uint16 SRX_AMP_PD     :1;
    cs_uint16 SRX_FCEN       :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_POLARITYINV:1;
    cs_uint16 SRX_2X2_DMUX_EN:1;
    cs_uint16 rsrvd1         :3;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CONFIG_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_FASTDIV_SEL:4;
    cs_uint16 SRX_CTRDIV_SEL :2;
    cs_uint16 SRX_CTVDIV_SEL :2;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_DDIV_SEL   :3;
    cs_uint16 SRX_RDIV_SEL   :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIV_SEL   :4;
    cs_uint16 SRX_DDIV_SEL   :3;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_CTVDIV_SEL :2;
    cs_uint16 SRX_CTRDIV_SEL :2;
    cs_uint16 SRX_FASTDIV_SEL:4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CLKDIV_CTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_REFCLK_SEL :1;
    cs_uint16 SRX_DIV64OUT_EN:1;
    cs_uint16 SRX_EXT_SERCLK_EN:1;
    cs_uint16 SRX_CLKBUF_CMFBOVR_EN:1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_CLKOUTEN   :1;
    cs_uint16 SRX_CLKEN      :1;
    cs_uint16 SRX_CLKINVEN   :1;
    cs_uint16 SRX_TCLKOUT_SEL:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_TCLKOUT_SEL:8;
    cs_uint16 SRX_CLKINVEN   :1;
    cs_uint16 SRX_CLKEN      :1;
    cs_uint16 SRX_CLKOUTEN   :1;
    cs_uint16 rsrvd1         :1;
    cs_uint16 SRX_CLKBUF_CMFBOVR_EN:1;
    cs_uint16 SRX_EXT_SERCLK_EN:1;
    cs_uint16 SRX_DIV64OUT_EN:1;
    cs_uint16 SRX_REFCLK_SEL :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CLKOUT_CTRL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_CPTSTUP    :1;
    cs_uint16 SRX_CPTSTDN    :1;
    cs_uint16 rsrvd1         :6;
    cs_uint16 SRX_CPCUR1     :4;
    cs_uint16 SRX_CPCUR0     :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_CPCUR0     :4;
    cs_uint16 SRX_CPCUR1     :4;
    cs_uint16 rsrvd1         :6;
    cs_uint16 SRX_CPTSTDN    :1;
    cs_uint16 SRX_CPTSTUP    :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_SRX0_RX_CPA_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :1;
    cs_uint16 REF_156MHZ     :1;
    cs_uint16 FILT_SCALE     :2;
    cs_uint16 VCO_SCALE      :2;
    cs_uint16 REF_SCALE      :2;
    cs_uint16 DATA_FILTLOCK  :1;
    cs_uint16 FORCE_FILTLOCK :1;
    cs_uint16 INIT           :1;
    cs_uint16 DATA_LOCK      :1;
    cs_uint16 SHORT_OVRIDE   :2;
    cs_uint16 PD_MODE        :1;
    cs_uint16 FORCE_LOCK     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FORCE_LOCK     :1;
    cs_uint16 PD_MODE        :1;
    cs_uint16 SHORT_OVRIDE   :2;
    cs_uint16 DATA_LOCK      :1;
    cs_uint16 INIT           :1;
    cs_uint16 FORCE_FILTLOCK :1;
    cs_uint16 DATA_FILTLOCK  :1;
    cs_uint16 REF_SCALE      :2;
    cs_uint16 VCO_SCALE      :2;
    cs_uint16 FILT_SCALE     :2;
    cs_uint16 REF_156MHZ     :1;
    cs_uint16 rsrvd1         :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSi:1;
    cs_uint16 FILT_LOCKi     :1;
    cs_uint16 CIDONEi        :1;
    cs_uint16 CIDZEROi       :1;
    cs_uint16 DATA_DETi      :1;
    cs_uint16 SHORT_FILTPNi  :1;
    cs_uint16 BAD_VCO_CLKi   :1;
    cs_uint16 LOCKi          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKi          :1;
    cs_uint16 BAD_VCO_CLKi   :1;
    cs_uint16 SHORT_FILTPNi  :1;
    cs_uint16 DATA_DETi      :1;
    cs_uint16 CIDZEROi       :1;
    cs_uint16 CIDONEi        :1;
    cs_uint16 FILT_LOCKi     :1;
    cs_uint16 CLKLESS_STATUSi:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSs:1;
    cs_uint16 FILT_LOCKs     :1;
    cs_uint16 CIDONEs        :1;
    cs_uint16 CIDZEROs       :1;
    cs_uint16 DATA_DETs      :1;
    cs_uint16 SHORT_FILTPNs  :1;
    cs_uint16 BAD_VCO_CLKs   :1;
    cs_uint16 LOCKs          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKs          :1;
    cs_uint16 BAD_VCO_CLKs   :1;
    cs_uint16 SHORT_FILTPNs  :1;
    cs_uint16 DATA_DETs      :1;
    cs_uint16 CIDZEROs       :1;
    cs_uint16 CIDONEs        :1;
    cs_uint16 FILT_LOCKs     :1;
    cs_uint16 CLKLESS_STATUSs:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSe:1;
    cs_uint16 FILT_LOCKe     :1;
    cs_uint16 CIDONEe        :1;
    cs_uint16 CIDZEROe       :1;
    cs_uint16 DATA_DETe      :1;
    cs_uint16 SHORT_FILTPNe  :1;
    cs_uint16 BAD_VCO_CLKe   :1;
    cs_uint16 LOCKe          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKe          :1;
    cs_uint16 BAD_VCO_CLKe   :1;
    cs_uint16 SHORT_FILTPNe  :1;
    cs_uint16 DATA_DETe      :1;
    cs_uint16 CIDZEROe       :1;
    cs_uint16 CIDONEe        :1;
    cs_uint16 FILT_LOCKe     :1;
    cs_uint16 CLKLESS_STATUSe:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTENABLE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 CLKLESS_STATUSiZ:1;
    cs_uint16 FILT_LOCKiZ    :1;
    cs_uint16 CIDONEiZ       :1;
    cs_uint16 CIDZEROiZ      :1;
    cs_uint16 DATA_DETiZ     :1;
    cs_uint16 SHORT_FILTPNiZ :1;
    cs_uint16 BAD_VCO_CLKiZ  :1;
    cs_uint16 LOCKiZ         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCKiZ         :1;
    cs_uint16 BAD_VCO_CLKiZ  :1;
    cs_uint16 SHORT_FILTPNiZ :1;
    cs_uint16 DATA_DETiZ     :1;
    cs_uint16 CIDZEROiZ      :1;
    cs_uint16 CIDONEiZ       :1;
    cs_uint16 FILT_LOCKiZ    :1;
    cs_uint16 CLKLESS_STATUSiZ:1;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPTZ_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 datawidth      :8;
    cs_uint16 rsrvd1         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd2         :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 enable         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable         :1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd1         :1;
    cs_uint16 datawidth      :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Cfg_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 oosThresh      :8;
    cs_uint16 rsrvd1         :3;
    cs_uint16 autoPolarityThresh:3;
    cs_uint16 autoPolarityEn :1;
    cs_uint16 loadPrbsState  :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 loadPrbsState  :1;
    cs_uint16 autoPolarityEn :1;
    cs_uint16 autoPolarityThresh:3;
    cs_uint16 rsrvd1         :3;
    cs_uint16 oosThresh      :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Ctrl_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 VALUE1         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 VALUE1         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Count1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 VALUE0         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 VALUE0         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_Count0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 AUTOPOLARITYi  :1;
    cs_uint16 PRBS_RX_EFi    :1;
    cs_uint16 PRBS_SYNCi     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_SYNCi     :1;
    cs_uint16 PRBS_RX_EFi    :1;
    cs_uint16 AUTOPOLARITYi  :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 AUTOPOLARITYs  :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 PRBS_SYNCs     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_SYNCs     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 AUTOPOLARITYs  :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSCHK0_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 SRX_RDIVFRAC_TESTCTRL:2;
    cs_uint16 SRX_COREFRAC_TESTCTRL:2;
    cs_uint16 rsrvd2         :6;
    cs_uint16 SRX_COREFRAC_RESET:1;
    cs_uint16 SRX_RDIVFRAC_RESET:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_RESET:1;
    cs_uint16 SRX_COREFRAC_RESET:1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 SRX_COREFRAC_TESTCTRL:2;
    cs_uint16 SRX_RDIVFRAC_TESTCTRL:2;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_FRAC0_RESET_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :5;
    cs_uint16 SRX_COREFRAC_VALIDB:1;
    cs_uint16 SRX_COREFRAC_TEST_EN:1;
    cs_uint16 SRX_COREFRAC_EN:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_RDIVFRAC_VALIDB:1;
    cs_uint16 SRX_RDIVFRAC_TEST_EN:1;
    cs_uint16 rsrvd3         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_RDIVFRAC_TEST_EN:1;
    cs_uint16 SRX_RDIVFRAC_VALIDB:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_COREFRAC_EN:1;
    cs_uint16 SRX_COREFRAC_TEST_EN:1;
    cs_uint16 SRX_COREFRAC_VALIDB:1;
    cs_uint16 rsrvd1         :5;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_FRAC0_EN_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_RDIVFRAC_NUMERATOR0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_NUMERATOR0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_NUMERATOR0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_RDIVFRAC_NUMERATOR1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_NUMERATOR1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_NUMERATOR1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_NUMERATOR0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_NUMERATOR0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_COREFRAC0_NUMERATOR0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_COREFRAC_NUMERATOR1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_NUMERATOR1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_COREFRAC0_NUMERATOR1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :12;
    cs_uint16 SRX_COREFRAC_WIDTH:2;
    cs_uint16 SRX_RDIVFRAC_WIDTH:2;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_WIDTH:2;
    cs_uint16 SRX_COREFRAC_WIDTH:2;
    cs_uint16 rsrvd1         :12;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_FRAC0_WIDTH_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_INTDIV:8;
    cs_uint16 SRX_RDIVFRAC_INTDIV:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_INTDIV:8;
    cs_uint16 SRX_COREFRAC_INTDIV:8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_FRAC0_INTDIV_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :5;
    cs_uint16 SRX_COREFRAC_STAGE1_EN:1;
    cs_uint16 SRX_COREFRAC_STAGE2_EN:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_RDIVFRAC_STAGE1_EN:1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_RDIVFRAC_STAGE2_EN:1;
    cs_uint16 rsrvd4         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd4         :1;
    cs_uint16 SRX_RDIVFRAC_STAGE2_EN:1;
    cs_uint16 rsrvd3         :1;
    cs_uint16 SRX_RDIVFRAC_STAGE1_EN:1;
    cs_uint16 rsrvd2         :5;
    cs_uint16 SRX_COREFRAC_STAGE2_EN:1;
    cs_uint16 SRX_COREFRAC_STAGE1_EN:1;
    cs_uint16 rsrvd1         :5;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_FRAC0_STAGE_EN_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_STAGE1PRELOAD1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RDIVFRAC0_STAGE1PRELOAD1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE1PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :8;
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD1:8;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_STAGE1PRELOAD1:8;
    cs_uint16 rsrvd1         :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE1PRELOAD1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_COREFRAC_STAGE2PRELOAD0:16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_COREFRAC_STAGE2PRELOAD0:16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_COREFRAC0_STAGE2PRELOAD0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :2;
    cs_uint16 SRX_COREFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_COREFRAC_DITHER_EN:1;
    cs_uint16 rsrvd3         :2;
    cs_uint16 SRX_RDIVFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd4         :3;
    cs_uint16 SRX_RDIVFRAC_DITHER_EN:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_RDIVFRAC_DITHER_EN:1;
    cs_uint16 rsrvd4         :3;
    cs_uint16 SRX_RDIVFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd3         :2;
    cs_uint16 SRX_COREFRAC_DITHER_EN:1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 SRX_COREFRAC_DITHER_SEL:2;
    cs_uint16 rsrvd1         :2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_FRAC0_DITHER_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :1;
    cs_uint16 STX_CMODE_RES  :3;
    cs_uint16 STX_DRV_LOWER_CM:4;
    cs_uint16 rsrvd2         :2;
    cs_uint16 STX_LEVEL      :6;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_LEVEL      :6;
    cs_uint16 rsrvd2         :2;
    cs_uint16 STX_DRV_LOWER_CM:4;
    cs_uint16 STX_CMODE_RES  :3;
    cs_uint16 rsrvd1         :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLA_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STX_SLEW_RATE_ADJ:2;
    cs_uint16 rsrvd1         :1;
    cs_uint16 STX_PRE_PEAK   :5;
    cs_uint16 STX_MUXSUBRATE_SEL:1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 STX_POST_PEAK  :6;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_POST_PEAK  :6;
    cs_uint16 rsrvd2         :1;
    cs_uint16 STX_MUXSUBRATE_SEL:1;
    cs_uint16 STX_PRE_PEAK   :5;
    cs_uint16 rsrvd1         :1;
    cs_uint16 STX_SLEW_RATE_ADJ:2;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLB_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 STX_SQUELCH    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_SQUELCH    :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_STX0_SQUELCH_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 datawidth      :8;
    cs_uint16 rsrvd1         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd2         :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 fixed_pattern  :1;
    cs_uint16 prbs_enable    :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 prbs_enable    :1;
    cs_uint16 fixed_pattern  :1;
    cs_uint16 invertPrbs     :1;
    cs_uint16 rsrvd2         :1;
    cs_uint16 prbs_mode      :3;
    cs_uint16 rsrvd1         :1;
    cs_uint16 datawidth      :8;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Cfg_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :12;
    cs_uint16 insertError    :1;
    cs_uint16 insertOneBitError:1;
    cs_uint16 insertErrorOnce:1;
    cs_uint16 insertOneBitErrorOnce:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 insertOneBitErrorOnce:1;
    cs_uint16 insertErrorOnce:1;
    cs_uint16 insertOneBitError:1;
    cs_uint16 insertError    :1;
    cs_uint16 rsrvd1         :12;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Ctrl_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 VALUE1         :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 VALUE1         :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_PRBSGEN0_Fixed0_Pattern1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 SRX_AGC_ENB_OFFSETMON:1;
    cs_uint16 SRX_AGC_ENB_OFFSET:1;
    cs_uint16 SRX_AGC_ENB_LIMAMP:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_AGC_ENB_LIMAMP:1;
    cs_uint16 SRX_AGC_ENB_OFFSET:1;
    cs_uint16 SRX_AGC_ENB_OFFSETMON:1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_SRX0_AGC_CONFIG1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 DFPROTECTi     :1;
    cs_uint16 TXELSTi        :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 PRBSCHKi       :1;
    cs_uint16 RXELSTi        :1;
    cs_uint16 RXLOCKDi       :1;
    cs_uint16 RXVCOi         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXVCOi         :1;
    cs_uint16 RXLOCKDi       :1;
    cs_uint16 RXELSTi        :1;
    cs_uint16 PRBSCHKi       :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 TXELSTi        :1;
    cs_uint16 DFPROTECTi     :1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_Int_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 DFPROTECTe     :1;
    cs_uint16 TXELSTe        :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 PRBSCHKe       :1;
    cs_uint16 RXELSTe        :1;
    cs_uint16 RXLOCKDe       :1;
    cs_uint16 RXVCOe         :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXVCOe         :1;
    cs_uint16 RXLOCKDe       :1;
    cs_uint16 RXELSTe        :1;
    cs_uint16 PRBSCHKe       :1;
    cs_uint16 rsrvd2         :6;
    cs_uint16 TXELSTe        :1;
    cs_uint16 DFPROTECTe     :1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_IntEn_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 RXLOCKD0i      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXLOCKD0i      :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKDi_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :15;
    cs_uint16 RXLOCKD0e      :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RXLOCKD0e      :1;
    cs_uint16 rsrvd1         :15;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_COMMON_RXLOCKDe_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SEL            :4;
    cs_uint16 GRWN           :1;
    cs_uint16 GRAM_A         :11;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GRAM_A         :11;
    cs_uint16 GRWN           :1;
    cs_uint16 SEL            :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_CR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GRAM_D1        :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GRAM_D1        :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_D1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GRAM_D0        :16;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GRAM_D0        :16;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_GRAM_D0_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MSEQ_pu_state  :1;
    cs_uint16 rsrvd1         :6;
    cs_uint16 PC_state       :9;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PC_state       :9;
    cs_uint16 rsrvd1         :6;
    cs_uint16 MSEQ_pu_state  :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_PC_SHADOW_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MSEQ_pu_state  :1;
    cs_uint16 rsrvd1         :11;
    cs_uint16 OPTIONS_state  :4;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 OPTIONS_state  :4;
    cs_uint16 rsrvd1         :11;
    cs_uint16 MSEQ_pu_state  :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS_SHADOW_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :12;
    cs_uint16 STALL          :1;
    cs_uint16 ALL_CHK_EN     :1;
    cs_uint16 IN_PAR_ENB     :1;
    cs_uint16 EX_PAR_ENB     :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EX_PAR_ENB     :1;
    cs_uint16 IN_PAR_ENB     :1;
    cs_uint16 ALL_CHK_EN     :1;
    cs_uint16 STALL          :1;
    cs_uint16 rsrvd1         :12;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :7;
    cs_uint16 CAL_DIGITAL_S  :1;
    cs_uint16 CAL_ANALOG_S   :1;
    cs_uint16 BAD_STATE_S    :1;
    cs_uint16 EDC_CONVERGED_S:1;
    cs_uint16 EXTERNAL_LOS_S :1;
    cs_uint16 LOCAL_LOCKD_S  :1;
    cs_uint16 DUPLEX_LOCKD_S :1;
    cs_uint16 LOCAL_LOS_S    :1;
    cs_uint16 DUPLEX_LOS_S   :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DUPLEX_LOS_S   :1;
    cs_uint16 LOCAL_LOS_S    :1;
    cs_uint16 DUPLEX_LOCKD_S :1;
    cs_uint16 LOCAL_LOCKD_S  :1;
    cs_uint16 EXTERNAL_LOS_S :1;
    cs_uint16 EDC_CONVERGED_S:1;
    cs_uint16 BAD_STATE_S    :1;
    cs_uint16 CAL_ANALOG_S   :1;
    cs_uint16 CAL_DIGITAL_S  :1;
    cs_uint16 rsrvd1         :7;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 bank_select_reg:3;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bank_select_reg:3;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_SDS_DSP_MSEQ_BANK_SELECTOR_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :4;
    cs_uint16 txgb_oflwI     :1;
    cs_uint16 txgb_uflwI     :1;
    cs_uint16 rxgb_oflwI     :1;
    cs_uint16 rxgb_uflwI     :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 halt_tx_on_lfI :1;
    cs_uint16 an_page_receiveI:1;
    cs_uint16 an_doneI       :1;
    cs_uint16 phy_linkI      :1;
    cs_uint16 linkI          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 linkI          :1;
    cs_uint16 phy_linkI      :1;
    cs_uint16 an_doneI       :1;
    cs_uint16 an_page_receiveI:1;
    cs_uint16 halt_tx_on_lfI :1;
    cs_uint16 rsrvd2         :3;
    cs_uint16 rxgb_uflwI     :1;
    cs_uint16 rxgb_oflwI     :1;
    cs_uint16 txgb_uflwI     :1;
    cs_uint16 txgb_oflwI     :1;
    cs_uint16 rsrvd1         :4;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_GIGEPCS_INT_HOST_PCS1GE_INTERRUPT_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :11;
    cs_uint16 halt_tx_on_lfS :1;
    cs_uint16 an_page_receiveS:1;
    cs_uint16 an_doneS       :1;
    cs_uint16 phy_linkS      :1;
    cs_uint16 linkS          :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 linkS          :1;
    cs_uint16 phy_linkS      :1;
    cs_uint16 an_doneS       :1;
    cs_uint16 an_page_receiveS:1;
    cs_uint16 halt_tx_on_lfS :1;
    cs_uint16 rsrvd1         :11;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_PP_HOST_GIGEPCS_INT_HOST_PCS1GE_INTSTATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 UnstickDone    :1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 LoadAbortDone  :1;
    cs_uint16 LoadFailed     :1;
    cs_uint16 LoadDone       :1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LoadDone       :1;
    cs_uint16 LoadFailed     :1;
    cs_uint16 LoadAbortDone  :1;
    cs_uint16 rsrvd1         :12;
    cs_uint16 UnstickDone    :1;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_EEPROM_LOADER_STATUS_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 MicroAccessPollStart:1;
    cs_uint16 MicroAccessWriteStart:1;
    cs_uint16 MicroAccessReadStart:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MicroAccessReadStart:1;
    cs_uint16 MicroAccessWriteStart:1;
    cs_uint16 MicroAccessPollStart:1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_EEPROM_MICRO_ACCESS_CONTROL_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1         :13;
    cs_uint16 Collision      :1;
    cs_uint16 NoAck          :1;
    cs_uint16 MicroAccessDone:1;

#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MicroAccessDone:1;
    cs_uint16 NoAck          :1;
    cs_uint16 Collision      :1;
    cs_uint16 rsrvd1         :13;

#   endif
    } bf ;
    cs_uint16     wrd ;
} CS4224_EEPROM_MICRO_ACCESS_STATUS_t;
#pragma pack(pop)

#endif /* !CS_HAS_PYTHON */

#endif /* __CS4224_REGISTERS_H__ */

/** @file cs4224_diags.h
 ****************************************************************************
 *
 * @brief
 *     This module provides diagnostic methods to assist in debugging
 *     or bringup of the device.
 *
 ****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ***************************************************************************/
#ifndef __CS4224_DIAGS_H__
#define __CS4224_DIAGS_H__

#if (CS_HAS_DEBUG_LOOPBACKS == 1)
/**
 * The loopback interface point 
 */
typedef enum
{
    /** No Duplex loopback */
    CS4224_LOOPBK_DUPLEX_NONE       = 0x0,

    /** Digital Near data loopback. Deprecated, replaced by CS4224_LOOPBK_DUPLEX_NEAR_DATA */
    CS4224_LOOPBK_DIGITAL_NEAR_DATA = 0x1,

    /** Duplex Near data loopback */
    CS4224_LOOPBK_DUPLEX_NEAR_DATA  = 0x1,

    /** Digital Far data loopback. Deprecated, replaced by CS4224_LOOPBK_DUPLEX_FAR_DATA */
    CS4224_LOOPBK_DIGITAL_FAR_DATA  = 0x2,

    /** Duplex Far data loopback */
    CS4224_LOOPBK_DUPLEX_FAR_DATA   = 0x2,

}e_cs4224_loopback;

/**
 * The loopback interface
 */
typedef enum
{
    /* Loopback on the host interface */
    CS4224_LOOPBK_HOST    = 0x1,

    /* Loopback on the line interface */
    CS4224_LOOPBK_LINE    = 0x3,

}e_cs4224_loopback_interface;

/**
 * This structure is used to store state information
 * that is used when enabling the line or host side duplex
 * near or far loopbacks.
 */
typedef struct
{
    /** initialize flag */
    cs_uint8  initialized;

    /** The state of the SDS_COMMON_STX0_MISC register */
    cs_uint16 stx0_misc;

    /** The state of the line SDS_DSP_MSEQ_POWER_DOWN_LSB register */
    cs_uint16 line_mseq_power_down;

    /** The state of the host SDS_DSP_MSEQ_POWER_DOWN_LSB register */
    cs_uint16 host_mseq_power_down;

    /** The state of the SDS_COMMON_RX0_Config register */
    cs_uint16 rx0_config;

    /** The state of the SDS_COMMON_TX0_Config register */
    cs_uint16 tx0_config;

    /* state of the RXELST0_Control register */
    cs_uint16 rxelst0_control;

    /** The state of the line SDS_COMMON_SRX0_RX_CLKOUT_CTRL register */
    cs_uint16 line_clkout_ctrl;

    /** The state of the host SDS_COMMON_SRX0_RX_CLKOUT_CTRL register */
    cs_uint16 host_clkout_ctrl;

    /** The state of the SDS_COMMON_SRX0_RX_CLKDIV_CTRL register */
    cs_uint16 clkdiv_ctrl;

    /** The state of the SDS_COMMON_RXLOCKD0_CONTROL register */
    cs_uint16 rxlockd0_ctrl;

    /** The state of the LINE_SDS_DSP_MSEQ_SPARE12_LSB register */
    cs_uint16 line_spare12;

    /** The state of the HOST_SDS_DSP_MSEQ_SPARE12_LSB register */
    cs_uint16 host_spare12;

    /** The state of the SDS_DSP_MSEQ_OPTIONS_SHADOW register */
    cs_uint16 mseq_options;

    /** The state of the line EDC mode */
    e_cs4224_edc_mode line_edc_mode;

    /** The state of the host EDC mode */
    e_cs4224_edc_mode host_edc_mode;
    
    /** The state of the SDS_COMMON_SRX0_RX_CPA register */
    cs_uint16 rx_cpa;

}cs4224_diags_duplex_loopback_state_t;

/**
 * This structure is used to store state information
 * that is used when enabling an simplex loopback. 
 */
typedef struct
{
    /** initialize flag */
    cs_uint8  initialized;

    /** The state of the SDS_COMMON_STX0_MISC register */
    cs_uint16 stx0_misc;

    /** The state of the SDS_DSP_MSEQ_POWER_DOWN_LSB register */
    cs_uint16 mseq_power_down;

    /** The state of the SDS_COMMON_SRX0_RX_CONFIG register */
    cs_uint16 rx_config;

    /** The state of the SDS_DSP_MSEQ_SPARE12_LSB register */
    cs_uint16 spare12;

    /** The state of the SDS_COMMON_SRX0_DFE_CONFIG register */
    cs_uint16 dfe_config;

    /** The state of the SDS_COMMON_SRX0_AGC_CONFIG1 register */
    cs_uint16 agc_config1;

}cs4224_diags_simplex_loopback_slice_state_t;

/**
 * This structure is used to store state information
 * that is used when enabling an simplex loopback. 
 */
typedef struct
{
    /** The slice state */
    cs4224_diags_simplex_loopback_slice_state_t slice_state;

    /** The mate slice state */
    cs4224_diags_simplex_loopback_slice_state_t mate_slice_state;

}cs4224_diags_simplex_loopback_state_t;

/* Query the loopback status */
cs_status cs4224_diags_query_loopbacks(
    cs_uint32           slice,
    e_cs4224_loopback*  line_lb_type,
    e_cs4224_loopback*  host_lb_type);

/* Dump the loopback status */
cs_status cs4224_diags_dump_loopbacks(
    cs_uint32  slice);

/* Enable a digital loopback   
 * NOTE: This method has been deprecated, 
 *       use cs4224_diags_duplex_loopback_enable 
 *       instead. */
cs_status cs4224_diags_loopback_enable(
        cs_uint32                   slice,
        e_cs4224_loopback           lb_type,
        e_cs4224_loopback_interface intf);

/* Disable a digital loopback
 * NOTE: This method has been deprecated, 
 *       use cs4224_diags_duplex_loopback_disable 
 *       instead. */
cs_status cs4224_diags_loopback_disable(
        cs_uint32                   slice,
        e_cs4224_loopback           lb_type,
        e_cs4224_loopback_interface intf);

/* Enable a digital loopback
 * NOTE: This method has been deprecated, 
 *       use cs4224_diags_duplex_loopback_enable 
 *       instead. */
cs_status cs4224_diags_digital_loopback_enable(
        cs_uint32                   slice,
        e_cs4224_loopback           lb_type,
        e_cs4224_loopback_interface intf);

/* Disable a digital loopback
 * NOTE: This method has been deprecated, 
 *       use cs4224_diags_duplex_loopback_enable 
 *       instead. */
cs_status cs4224_diags_digital_loopback_disable(
        cs_uint32                   slice,
        e_cs4224_loopback           lb_type,
        e_cs4224_loopback_interface intf);

/* Enable a duplex loopback */
cs_status cs4224_diags_duplex_loopback_enable(
        cs_uint32                   slice,
        e_cs4224_loopback           lb_type,
        e_cs4224_loopback_interface intf, 
        cs4224_diags_duplex_loopback_state_t* state);

/* Disable a duplex loopback */
cs_status cs4224_diags_duplex_loopback_disable(
        cs_uint32                   slice,
        e_cs4224_loopback           lb_type,
        e_cs4224_loopback_interface intf, 
        cs4224_diags_duplex_loopback_state_t* state);

/* Enable an analog loopback
 * NOTE: This method has been deprecated, 
 *       use cs4224_diags_simplex_loopback_enable 
 *       instead. */
cs_status cs4224_diags_analog_loopback_enable(
        cs_uint32   slice);

/* Disable an analog loopback
 * NOTE: This method has been deprecated, 
 *       use cs4224_diags_simplex_loopback_disable 
 *       instead. */
cs_status cs4224_diags_analog_loopback_disable(
        cs_uint32   slice);

/* Enable a simplex loopback */
cs_status cs4224_diags_simplex_loopback_enable(
        cs_uint32                             slice,
        cs4224_diags_simplex_loopback_state_t* state);

/* Disable a simplex loopback */
cs_status cs4224_diags_simplex_loopback_disable(
        cs_uint32                             slice,
        cs4224_diags_simplex_loopback_state_t* state);

#if 0
/* This method saves the original state of the interface
 * and is called when enabling the line or host side digital loopback. */
cs_status cs4224_diags_loopback_save_state(
    cs_uint32                             slice, 
    e_cs4224_loopback_interface           intf,
    cs4224_diags_digital_loopback_state_t *state);
#endif

#endif /* CS_HAS_DEBUG_LOOPBACKS == 1 */

#if (CS_HAS_DEBUG_PRBS == 1)
/**
 * The supported PRBS polynomials 
 */
typedef enum
{
    /** 1 + x^28 + x^31  */
    CS4224_PRBS_Tx_2exp31 = 0x0,

    /** 1 + x^18 + x^23  */
    CS4224_PRBS_Tx_2exp23 = 0x1,
    
    /** 1 + x^14 + x^15  */
    CS4224_PRBS_Tx_2exp15 = 0x2,

    /** 1 + x^6 + x^7  */
    CS4224_PRBS_Tx_2exp7 = 0x3,
    
    /** 1 + x^4 + x^9  */
    CS4224_PRBS_Tx_2exp9 = 0x4,

    /** 1 + x^5 + x^9  */
    CS4224_PRBS_Tx_2exp9_5 = 0x5,

    /** 1 + x^5 + x^58  */
    CS4224_PRBS_Tx_2exp58 = 0x6,

}e_cs4224_prbs_polynomial;

/**
 * Used to select the PRBS generator/checker
 * pair
 */
typedef enum
{
    /** The line side PRBS generator/checker */
    CS4224_PRBS_LINE_INTERFACE  = 0,

    /** The host side PRBS generator/checker. */
    CS4224_PRBS_HOST_INTERFACE  = 1,

    /** If in simplex mode this automatically
     * determines the appropriate checker or generator */
    CS4224_PRBS_SIMPLEX_INTERFACE = 2,

}e_cs4224_prbs_interface;

/** PRBS error injection modes */
typedef enum
{
    /** Inject continuous errors - autopolarity must be disabled */
    CS4224_PRBS_INJECT_ERROR_CONTINUOUS = 0x8,

    /** Inject continuous single bit errors */
    CS4224_PRBS_INJECT_ERROR_ONE_BIT_CONTINUOUS = 0x4,

    /** Inject a single error once */
    CS4224_PRBS_INJECT_ERROR_ONCE = 0x2,

    /** Inject a single bit error once */
    CS4224_PRBS_INJECT_ERROR_ONE_BIT_ONCE = 0x1
}e_cs4224_prbsgen_error_cfg;

/* Configure the PRBS generator for transmit  */
cs_status cs4224_diags_prbs_generator_config(
        cs_uint32                slice,
        e_cs4224_prbs_interface  prbs_sel,
        e_cs4224_prbs_polynomial polynomial,
        cs_uint8                 invert);

/* Enable or disable the PRBS generator  */
cs_status cs4224_diags_prbs_generator_enable(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_uint8                enable);

/* Inject errors via the PRBS generator */
cs_status cs4224_diags_prbs_generator_set_error_ctrl(
    cs_uint32                  slice,
    e_cs4224_prbs_interface    prbs_sel,
    e_cs4224_prbsgen_error_cfg mode,
    cs_boolean                 enable);

/* Configure the PRBS checker for receiving a test pattern and verifying it is correct. 
 *  */
cs_status cs4224_diags_prbs_checker_config(
        cs_uint32                  slice,
        e_cs4224_prbs_interface    prbs_sel,
        e_cs4224_prbs_polynomial   polynomial,
        cs_uint8                   invert, 
        cs_uint8                   lbk_enable);

/* Enable or disable the PRBS checker  */
cs_status cs4224_diags_prbs_checker_enable(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_uint8                enable);

/* Enable or disable the PRBS checker auto-polarity */
cs_status cs4224_diags_prbs_checker_autopol_enable(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_boolean              enable);

/* If auto-polarity is enabled, gets the current inverted state of the signal */
cs_status cs4224_diags_prbs_checker_get_polarity(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_boolean              *inverted);

/* Retrieves the 32 bit error count reported by the PRBS checker.  */
cs_status cs4224_diags_prbs_checker_get_errors(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_uint32*              error_count);

/* Retrieve the full status of the PRBS checker including RX lock,
 * sync detect and error count */
cs_status cs4224_diags_prbs_checker_get_status(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_uint32*              error_count,
        cs_boolean*             prbs_sync);

/* This method is called to configure the pattern that the fix pattern generator 
 * transmits.  */
cs_status cs4224_diags_fix_ptrn_generator_cfg(
        cs_uint32               slice,
        e_cs4224_prbs_interface gen_sel,
        cs_uint32               sequence_a,
        cs_uint8                repeat_a,
        cs_uint32               sequence_b,
        cs_uint8                repeat_b);

/* Enable or disable the fixed pattern generator  */
cs_status cs4224_diags_fix_ptrn_generator_enable(
        cs_uint32               slice,
        e_cs4224_prbs_interface prbs_sel,
        cs_boolean              enable);

/* Set the PRBS generator in Local timing/PFD mode */
cs_status cs4224_diags_prbs_generator_set_pfd_mode(
    cs_uint32               slice,
    e_cs4224_prbs_interface prbs_sel,
    cs_boolean              enable);

/* Set the PRBS generator in Local timing/PFD mode */
cs_status cs4224_diags_prbs_generator_set_local_timing_mode(
    cs_uint32               slice,
    e_cs4224_prbs_interface prbs_sel,
    cs_boolean              enable);

/* Enable or disable squelching of the generated PRBS pattern */
cs_status cs4224_diags_prbs_generator_squelch(
    cs_uint32               slice,
    e_cs4224_prbs_interface prbs_sel,
    cs_boolean              squelch);

#endif /* CS_HAS_DEBUG_PRBS == 1 */

/*
+=========================================
| Status Dump Methods
+=========================================
*/
#if (CS_HAS_DEBUG_STATUS_DUMPS == 1)

cs_status cs4224_diags_switch_show_state(
    cs_uint32 slice);

/* Show the VCO lock status summary
 * NOTE: This method has been deprecated, use cs4224_diags_show_vco_status 
 *       instead. */
void cs4224_diags_show_vco_lock_status(void);

/* Show the VCO status with a prefix */
void cs4224_diags_show_vco_status_prefixed(
    cs_uint32   slice,
    const char* prefix);

/* Show the VCO lock status summary of an ASIC */
void cs4224_diags_show_vco_status(
    cs_uint32 slice);

/* Show the VCO lock status summary in a string
 * NOTE: This method has been deprecated, use cs4224_diags_get_vco_status_string 
 *       instead. */
void cs4224_diags_get_vco_lock_status_string(
    char* buffer);

/* Show the VCO lock status summary of an ASIC in a string */
const char* cs4224_diags_get_vco_status_string(
    cs_uint32 slice,
    char* buffer);

void cs4224_diags_is_vco_locked(
    cs_uint32 slice,
    cs4224_vco_lock_status_t *vco);

/* Sections in the status summary report */

/**
 * A status flag used with cs4224_diags_show_status() to
 * display sections in the summary report
 */
typedef enum
{
    /**
     * display the global status/config in the summary report
     */
    CS4224_STATUS_GLOBAL = 1 << 0,

    /**
     * display the SERDES locking information in the summary report
     */
    CS4224_STATUS_SERDES = 1 << 1,

    /**
     * display the Microsequencer status in the summary report
     */
    CS4224_STATUS_MSEQ = 1 << 2,

    /**
     * display the clocking configuration in the summary report
     */
    CS4224_STATUS_CLOCKING = 1 << 3,

    /**
     * display the line interrupts
     */
    CS4224_STATUS_LINE_INTERRUPT = 1 << 4,
    
    /**
     * display the host interrupts
     */
    CS4224_STATUS_HOST_INTERRUPT = 1 << 5,

    /**
     * The link status (including RX Lock, EDC Convergence and PCS)
     */
    CS4224_STATUS_LINK = 1<<6,
    
    /**
     * display everything
     */
    CS4224_STATUS_ALL = 0xffff,
}e_cs4224_status_mask;

/* This method displays a status summary of the device */
cs_status cs4224_diags_show_status(
    cs_uint32 slice_start,
    cs_uint32 slice_end,
    cs_uint16 sections_to_display);

/* Dump and clear interrupts and FEC stats */
cs_status cs4224_diags_show_and_clear_stats(cs_uint32 die);

#endif /* CS_HAS_DEBUG_STATUS_DUMPS == 1 */

#if (CS_HAS_DEBUG_REGISTER_DUMPS == 1)

/* Check to see if a register can be read or not */
cs_boolean cs4224_diags_register_can_read(
        cs_uint16 addr);

/* Perform a register dump of all slices by calling CS_PRINTF()
 * NOTE: This method has been deprecated, use cs4224_diags_register_dump_asic 
 *       instead. */
void cs4224_diags_register_dump(void);

/* Perform a register dump of all slices of a selected ASIC by calling CS_PRINTF() */
void cs4224_diags_register_dump_asic(
    cs_uint32 slice);

void cs4224_diags_register_dump_die(
    cs_uint32 die);

/* Read a range of registers for a particular slice */
cs_status cs4224_diags_register_dump_range(
        cs_uint32 die,
        cs_uint32 start_addr,
        cs_uint32 end_addr,
        cs_uint16 registers[]);

#endif /* CS_HAS_DEBUG_REGISTER_DUMPS == 1 */

/* Diagnostic method to sweep the calibration parameters for performance analysis */
cs_status cs4224_diags_sweep_calibration_parameters(
    cs_uint32 slice_start,
    cs_uint32 slice_end,
    cs_uint32 sweep_duration);

/* Diagnostic method to perform an automated memory BIST test */
cs_status cs4224_diags_mbist_run(cs_uint32 slice);

/* Returns CS_ERROR if there are FEC errors on any slice */
cs_status cs4224_diags_get_fec_errors(
        cs_uint32 die);

/* Returns CS_ERROR if there are FEC errors on a single slice */
cs_status cs4224_diags_query_fec_status(
        cs_uint32 slice);

/* Clear all the interrupts */
cs_status cs4224_diags_clear_interrupts(
    cs_uint32 slice);

/* Re-sync the digital elastic store */
cs_status cs4224_diags_resync_digital_elsto(
    cs_uint32 slice);

/* Dump the contents of a microsequencer data store */
cs_status cs4224_diags_dump_data_store(
    cs_uint32 slice, 
    e_cs4224_mseq_id side);

/* Dump the contents of a microsequencer program store */
cs_status cs4224_diags_dump_pgrm_store(
    cs_uint32 slice, 
    e_cs4224_mseq_id side);

#endif /* __CS4224_DIAGS_H__ */
/** @file cs4224_fcan.h
 ****************************************************************************
 *
 * @brief
 *     This module contains data types, prototype definitions and constansts 
 *     for the FC-AN feature.
 *
 ******************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ****************************************************************************/
#ifndef __FCAN_H__
#define __FCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize FC-AN */
cs_status cs4224_fcan_init_fc_pre_an(
    cs_uint32 slice,
    cs4224_rules_t* rules);

/* Wait for FC-AN to complete */
e_cs4224_fcan_an_status_t cs4224_fcan_wait_for_an(
    cs_uint32 slice,
    e_cs4224_fcan_data_rate_t *negotiated_rate);

/* Wait for FC-AN to complete, with a timeout */
e_cs4224_fcan_an_status_t cs4224_fcan_wait_for_an_with_timeout(
    cs_uint32 slice,
    cs_uint16 max_secs_to_poll,
    e_cs4224_fcan_data_rate_t *negotiated_rate);

/* Configure slice after FC-AN link has been negotiated */
cs_status cs4224_fcan_init_fc_post_an(
    cs_uint32 slice, 
    cs4224_rules_t* rules);

/* Verify supported data rates */
cs_status cs4224_fcan_check_rates(
    cs4224_fcan_data_rates_t rates);

/* Set the desired data-rate in the rules struct */
cs_status cs4224_fcan_rules_set_rate(
    cs4224_rules_t* rules);

/* Print a FCAN status summary */
cs_status cs4224_fcan_status_summary(
    cs_uint32 slice);

/* Start the AN process */
cs_status cs4224_fcan_start_an(
    cs_uint32 slice);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* __FCAN_H__ */

/** @file cs4224_kran.h
 ****************************************************************************
 *
 * @brief
 *     This module contains prototype definitions for the KR-AN feature.
 *
 ******************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ****************************************************************************/
#ifndef __CS4224_KRAN_H__
#define __CS4224_KRAN_H__

#ifdef __cplusplus
extern "C" {
#endif

cs_status cs4224_kran_init_kr_pre_an(
    cs_uint32 slice,
    cs4224_rules_t* rules,
    cs4224_kran_config_t_s *config);

cs_status cs4224_kran_init_kr_post_an(
    cs_uint32 slice, 
    cs4224_rules_t* rules,
    cs4224_kran_results_t_s *an_results);

void cs4224_kran_dump_regs(
    cs_uint32 slice,
    cs_uint32 loop_count);

cs_status cs4224_kran_poll_an(
    cs_uint32 slice,
    e_cs4224_kran_an_status_t *kran_status);

e_cs4224_kran_an_status_t cs4224_kran_wait_for_an(
    cs_uint32 slice,
    cs4224_kran_results_t_s *an_results,
    void* ptr);

e_cs4224_kran_an_status_t cs4224_kran_wait_for_an_with_timeout(
    cs_uint32 slice,
    cs_uint32 max_secs_to_poll,
    cs4224_kran_results_t_s *an_results);

void cs4224_kran_show_negotiated(
    cs4224_kran_results_t_s *an_results);

cs_status cs4224_kran_status(
    cs_uint32 slice);

cs_status cs4224_kran_status_summary(
    cs_uint32 slice);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* __CS4224_KRAN_H__ */

#ifndef __CS_API_VERSION_H__
#define __CS_API_VERSION_H__

#define CS_API_VERSION    "3.7.8"
#define CS_API_BUILD_DATE "Jul 15, 2015 at 07:06:38"

#define CS_API_VERSION_MAJOR  3
#define CS_API_VERSION_MINOR  7
#define CS_API_VERSION_UPDATE 8

#endif /* __CS_API_VERSION_H__ */
/** @file cs4224_ucode.h
 ****************************************************************************
 *
 * @brief
 *     This module contains code for programming the microcode on the
 *     CS4224 devices.
 *
 ****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ***************************************************************************/
#ifndef __CS4224_UCODE_H__
#define __CS4224_UCODE_H__

/* we need both standard library and a file system for file I/O */
  
#ifndef CS_DONT_USE_STDLIB
#ifdef  CS_HAS_FILESYSTEM
  
#include <stdio.h>

cs_status cs4224_ucode_data_prgm_from_file(
    cs_uint32   slice, 
    const char* file_name);

cs_status cs4224_ucode_data_parse_file(
    cs_uint32 slice, 
    FILE *infilePH);

  
#endif  /* CS_DONT_USE_STDLIB */
#endif  /* CS_HAS_FILESYSTEM  */
  
cs_status cs4224_ucode_data_prgm_image(
    cs_uint32 slice, 
    cs_uint16 microcode[], 
    cs_uint32 len_buffer);

cs_status cs4224_ucode_data_prgm_image_broadcast(
    cs_uint32 slice, 
    cs_uint16 microcode[], 
    cs_uint32 len_buffer);

#endif /* __CS4224_UCODE_H__ */
/** @file cs4224_io.h
 ****************************************************************************
 *
 * @brief
 *     This module contains methods for configuring the GPIO inputs
 *     and outputs on the device.
 *
 ****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ***************************************************************************/
#ifndef __CS4224_IO_H__
#define __CS4224_IO_H__

/**
 * The GPIO output (port specific and common) function mapping for a GPIO. If it is
 * a GPIO input the function should be set to CS4224_GPIO_OUT_FCN_DISABLED
 */
typedef enum
{

/* Start of per port output functions */

    /** RX Loss of Signal, not very reliable, don't use this.
     * Equivalent to SDS_COMMON_RXLOS0_STATUS_0.LOS */
    CS4224_GPIO_OUT_FCN_PORT_RX_LOS           = 0,

    /** RX Loss of Lock (inverted from MSEQ)
     * Equivalent to SDS_COMMON_RXLOCKD0_INTSTATUS.LOCKs
     * Not debounced, use EDC_CONVERGED instead */
    CS4224_GPIO_OUT_FCN_PORT_RX_LOL           = 1,

    /** EDC Converged from the microcode
     * Equivalent to SDS_DSP_MSEQ_STATUS.EDC_CONVERGED_S
     * Use this as the lock detection state */
    CS4224_GPIO_OUT_FCN_PORT_EDC_CONVERGED    = 2,

    /** PRBS sync from the RX PRBS checker
     * Equivalent to SDS_COMMON_PRBSCHK0_INTSTATUS.PRBS_SYNCs
     * 1 = not synced, 0 = synced */
    CS4224_GPIO_OUT_FCN_PORT_RX_PRBS_SYNC     = 3,

    /** Watchdog timer, don't bother using this (internal) */
    CS4224_GPIO_OUT_FCN_PORT_WATCHDOG_TIMEOUT = 4,

    /** MSEQ GPIO 1 */
    CS4224_GPIO_OUT_FCN_PORT_GPIO_MSEQ_GPIO1  = 5,

    /** MSEQ GPIO 2 */
    CS4224_GPIO_OUT_FCN_PORT_GPIO_MSEQ_GPIO2  = 6, 

    /** MSEQ GPIO 3 */
    CS4224_GPIO_OUT_FCN_PORT_GPIO_MSEQ_GPIO3  = 7, 

    /** RX data detect. Don't use this, not implemented */
    CS4224_GPIO_OUT_FCN_PORT_RX_DATA_DETECT   = 8, 

    /** Clock monitor output */
    CS4224_GPIO_OUT_FCN_PORT_CLKMON_CLK       = 9, 

    /** SDS SRX_FILTSHORT, don't bother using this (internal) */
    CS4224_GPIO_OUT_FCN_PORT_SRX_FILTSHORT    = 10,

    /** SDS SRX_LOCKDETI, don't bother using this (internal) */
    CS4224_GPIO_OUT_FCN_PORT_SRX_LOCKDETI     = 11,

    /** Rx filtered Loss of Lock
     * Equivalent to SDS_COMMON_RXLOCKD0_INTSTATUS.FILT_LOCKs
     * Debounced, but not checked like EDC converged. Use that instead. */
    CS4224_GPIO_OUT_FCN_PORT_RX_LOCK_STATUS   = 12,

/* End of per port output functions */

/* Start of Common output fnctions */

    /** Logic 0 */
    CS4224_GPIO_OUT_FCN_COMMON_LOGIC_0        = 0x10+0,

    /** Get the output from GPIO 1 */
    CS4224_GPIO_OUT_FCN_COMMON_GPIO1_INPUT    = 0x10+1,

    /** Get the output from GPIO 2 */
    CS4224_GPIO_OUT_FCN_COMMON_GPIO2_INPUT    = 0x10+2,

    /** Get the output from GPIO 3 */
    CS4224_GPIO_OUT_FCN_COMMON_GPIO3_INPUT    = 0x10+3, 

    /** Get the output from GPIO 4 */
    CS4224_GPIO_OUT_FCN_COMMON_GPIO4_INPUT    = 0x10+4,

    /** Get the output from INTERR */
    CS4224_GPIO_OUT_FCN_COMMON_INTERR_INPUT   = 0x10+5,

    /** Interrupt output from GLOBAL_INT register */
    CS4224_GPIO_OUT_FCN_COMMON_GLOBAL_INT     = 0x10+6,

    /** Interrupt output from GPIO_INT register */
    CS4224_GPIO_OUT_FCN_COMMON_GPIO_INT       = 0x10+7,

    /** Temp/Voltage monitor interrupt */
    CS4224_GPIO_OUT_FCN_COMMON_MONITOR_INT    = 0x10+8,

    /** Selected clock output of global clock monitor */
    CS4224_GPIO_OUT_FCN_COMMON_GBL_CLKMON_CLK = 0x10+9,

/* End of common output fnctions */

    /** Instead of mapping the GPIO to a particular function, let it be driven directly by the GPIO_GPIO[n]_DRIVE register */  
    CS4224_GPIO_OUT_FCN_DIRECT_DRIVE = 0xf0,

    /** Don't use the GPIO as an output */
    CS4224_GPIO_OUT_FCN_DISABLED = 0xff,

}e_cs4224_gpio_out_fcn;

/**
 * The GPIO input function mapping for a GPIO. 
 */
typedef enum
{
    /** Bind the input to logic 0 */
    CS4224_GPIO_IN_FCN_EXT_LOGIC_0 = 0,

    /** Bind the GPIO1 input to the value of a EXT_LOS signal */
    CS4224_GPIO_IN_FCN_EXT_GPIO1_LOS = 1,

    /** Bind the GPIO2 input to the value of a EXT_LOS signal */
    CS4224_GPIO_IN_FCN_EXT_GPIO2_LOS = 2,

    /** Bind the GPIO3 input to the value of a EXT_LOS signal */
    CS4224_GPIO_IN_FCN_EXT_GPIO3_LOS = 3,

    /** Bind the GPIO4 input to the value of a EXT_LOS signal */
    CS4224_GPIO_IN_FCN_EXT_GPIO4_LOS = 4,

    /** Bind the INTERR input to the value of a EXT_LOS signal */
    CS4224_GPIO_IN_FCN_EXT_INTERR = 5,
    
}e_cs4224_gpio_in_fcn;

/**
 * The GPIOs of the device.
 */
typedef enum
{
    /** GPIO INTERR */
    CS4224_GPIOINTERR = 0,
    /** GPIO 1  */
    CS4224_GPIO1      = 1,
    /** GPIO 2 */
    CS4224_GPIO2      = 2,
    /** GPIO 3 */
    CS4224_GPIO3      = 3,
    /** GPIO 4 */
    CS4224_GPIO4      = 4,

}e_cs4224_gpios;

/**
 * GPIO Configuration structure
 */
typedef struct
{

    /** Selects the data source for the GPIO output. */
    cs_uint8 output_src_sel;

    /** Configures the output GPIO polarity
                           0 = GPIO output is sent directly to pad
                           1 = GPIO output is inverted before being sent to pad */
    cs_uint8 invert_output;

    /** Configures the input GPIO polarity */
    cs_uint8 invert_input;

    /** Configures the PAD voltage level */
    cs_uint8 iovddsel_1v2;

    /** Configures whether the GPIO output enable is
                           controlled by 'outen' bit or by an external
                           source. */
    cs_uint8 outen_ovr;

    /** Configures the pad direction when outen_ovr == 0 */
    cs_uint8 outen;

    /** 0 = disables the pad pull up, 1 = enables the pad 1k pullup */
    cs_uint8 pullup_1k;

    /** 0 = disables the pad pull up, 1 = enables the pad 10k pullup */
    cs_uint8 pullup_10k;

    /** The function mapping of the GPIO if configured as an output */
    e_cs4224_gpio_out_fcn out_fcn;

    /** The function mapping of the GPIO if configured as an input */
    e_cs4224_gpio_in_fcn in_fcn;

    /** Toggle rate of a GPIO output */
    cs_uint16 toggle_rate;

    /** Delay in ms for a GPIO output. */
    cs_uint8 on_delay;

    /** Configure the line or host side */
    e_cs4224_cfg_sides_t side;

}cs4224_gpio_cfg_t;

/* Defines for GPIO outputs */
#define CS4224_GPIO_SEL_DRIVE 0
#define CS4224_GPIO_SEL_DELAY 1

/* Setup the defaults to configure the GPIO as an output */
cs_status cs4224_gpio_cfg_output(
    cs4224_gpio_cfg_t* cfg, 
    e_cs4224_gpio_out_fcn out_fcn);

/* Setup the defaults to configure the GPIO as an input */
cs_status cs4224_gpio_cfg_input(
    cs4224_gpio_cfg_t* cfg, 
    e_cs4224_gpio_in_fcn in_fcn);

/* This method is called to configure one of the 16 available GPIO ports on the device. */
cs_status cs4224_gpio_init(
    cs_uint32 slice, 
    cs_uint8 gpio_num, 
    cs4224_gpio_cfg_t* cfg);

/* This method is called to set the value of a GPIO pin on the device if it is configured 
 * as an output. */
cs_status cs4224_gpio_set(
    cs_uint32 slice, 
    cs_uint8 gpio_num, 
    cs_uint8 val);

/* This method is called to get the current value of a GPIO pin on the device */
cs_status cs4224_gpio_get(
    cs_uint32 slice, 
    cs_uint8 gpio_num, 
    cs_uint8* val);

#endif /* __CS4224_IO_H__ */
/** @file cs4224_irq_priv.h
 *****************************************************************************
 * 
 * @brief
 *    This module contains internal IRQ management routines.
 *
 *****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ****************************************************************************/
#ifndef __CS4224_IRQ_PRIV_H__
#define __CS4224_IRQ_PRIV_H__

#if defined(CS_HAS_INTERRUPTS)

/* Including the code that prints out interrupt register and bit-field names is optional due to
 * the amount of memory all of the strings will take. The following define will enable or disable
 * the inclusion of the printing code and data structures.
 * Set to '1' to include the code and structures
 * Set to '0' to NOT include them
 */
#define CS4224_IRQ_PRINT_INTERRUPTS 1

#define CS4224_IRQ_RET_IF_ERROR(f, c) { if ((f) != CS_OK) return (c); }

/******************************************************************************
 * Interrupt Tree Data Structures
 * These structs define the interrupt tree so that interrupts can be
 * enabled/disabled from any node up and/or down the tree. It implements a
 * double-linked-list of interrupt nodes.
 * Every node in the interrupt hierarchy needs the following information:
 *   - The address of the interrupt register (ireg)
 *   - The address of the interrupt enable register (ereg)
 *   - The number of slices of this register (slices)
 *   - The stride between slices (stride)
 *   - The bits of the enable register that is used to enable interrupts (mask)
 *   - Each bit in a node can point to another child node, so for each bit
 *     the child node needs to be identified, for walking down the tree (children)
 *   - To walk up the tree, each slice of a node can point to either
 *     - Different bits in the same parent register
 *     - The same bit in different slices of the same parent register
 *     - Different bits in different parent registers
 *     'parents' holds a pointer to the parent nodes
 *   - To enable/disable interrupt up the tree the enable bits in the
 *     parent register must be known. This can be different for each slice.
 ******************************************************************************/

/** A pointer to an interrupt node */
typedef struct cs4224_irq_node_s *cs4224_irq_node_ptr;

/**
 * An interrupt node has children (up to 16)
 * Register bit 0 is array element 0
 */
typedef struct cs4224_irq_node_children_s {
    /** The chile node */
    cs4224_irq_node_ptr child;
    
    /** slice of the child */
    cs_uint8         slice;
} cs4224_irq_node_child_t;

/**
 * An interrupt node can have multiple children (1 for each slice)
 */
typedef cs4224_irq_node_child_t cs4224_irq_node_children_t[16];

/**
 * An interrupt's parent is a node associated with a mask
 */
typedef struct cs4224_irq_node_parent_s
{
    /** The parent node */
    cs4224_irq_node_ptr parent;

    /** Mask associated with the node */
    cs_uint16        mask;
} cs4224_irq_node_parent_t;

/**
 * An interrupt node can have multiple parents (1 for each slice)
 */
typedef cs4224_irq_node_parent_t cs4224_irq_node_parents_t[16];

/**
 * This is the structure that gets passed to an interrupt handler
 */
typedef struct cs4224_irq_handler_data_s {
    /** The handle, slice or die parameter */
    cs_uint32             handle;

    /** A pointer to the interrupt node structure associated with the interrupt */
    cs4224_irq_node_ptr    node;

    /** The interrupt register address */
    cs_uint16                ireg_data;

    /** The interrupt enable address */
    cs_uint16                ereg_data;

    /** The interrupt status address, will be 0 when there is no status register for a node */
    cs_uint16                sreg_data;

    /** The slice or instance of the interrupt */
    cs_uint8                 slice;
} cs4224_irq_handler_data_t;

/** This is a registerable interrupt handler */
typedef void (*cs4224_irq_handler_t)(cs4224_irq_handler_data_t);

/**
 * An interrupt node. This struct can't be a const because the 'handler'
 * field will be modified when an interrupt handler is registered */
typedef struct cs4224_irq_node_s
{
    /** The interrupt register address */
    cs_uint32                ireg;
    /** The enable register address */
    cs_uint32                ereg;
    /** The interrupt status register address */
    cs_uint32                sreg;
    /** The number of slices or instances of this interrupt */
    cs_uint16                slices;
    /** The stride or span between instances of this interrupt */
    cs_uint16                stride;
    /** The number of children belong to this interrupt node */
    cs_uint8                 num_children;
    /** The mask associated with the parent of this interrupt */
    cs_uint16                mask;
    /** Pointer to any children belong to this interrupt */
    cs4224_irq_node_children_t *children;
    /** Pointer to any parents of this interrupt */
    cs4224_irq_node_parents_t  *parents;
    /** A handler associated with the interrupt */
    cs4224_irq_handler_t        handler;
#if CS4224_IRQ_PRINT_INTERRUPTS
    /** Display name of the interrupt */
    char                    *name;
    /** Defnes the size of the variable length child element array */
    cs_uint8                 last_child;
/**
 * An array defining the array of children or fields belonging
 * to this interrupt.
 * 
 * To save a little bit of memory, this statement can be changed to the following
 * in a newer compiler (gcc 3+)
    char                    *child_name[];
 */
    char                    *child_name[16];
#endif
} cs4224_irq_node_t;

#endif /* CS_HAS_INTERRUPTS */

#endif /* __CS4224_IRQ_PRIV_H__ */

/** @file cs4224_irq.h
 ****************************************************************************
 *
 * @brief
 *    This module contains methods for managing interrupts on the device.
 *
 ****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ****************************************************************************/
#ifndef __CS4224_IRQ_H__
#define __CS4224_IRQ_H__

#if defined(CS_HAS_INTERRUPTS)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * Defines the rules for processing interrupts. It allows
 * the interrupt walker to:
 *
 * - process the current node
 * - walk down the tree
 * - walk up the tree
 */
typedef enum {
    /** Configure the target node but don't walk up or down the hierarchy */
    CS4224_IRQ_DIR_ONLY,

    /** Configure the target node and continue up the tree to the root interrupt */
    CS4224_IRQ_DIR_UP,

    /** Configure the target node and any children beneath it in the hierarchy */ 
    CS4224_IRQ_DIR_DOWN,

    /** Configure the target node and any children beneath it in the hiearchy and up to the root node */
    CS4224_IRQ_DIR_BOTH
} e_cs4224_irq_tree_dir;

cs_status cs4224_irq_enable (cs_uint32 slice, cs4224_irq_node_ptr node, cs_uint16 enable_bits, e_cs4224_irq_tree_dir dir);
cs_status cs4224_irq_disable(cs_uint32 slice, cs4224_irq_node_ptr node, cs_uint16 disable_bits, e_cs4224_irq_tree_dir dir);
cs_status cs4224_irq_isr(cs_uint32 slice);
cs_status cs4224_irq_register_handler(cs4224_irq_node_ptr node, cs4224_irq_handler_t handler);
cs_boolean cs4224_irq_register_handler_exists(cs4224_irq_node_ptr node);
cs_status cs4224_irq_print_enabled_interrupts(cs_uint32 slice);

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* CS_HAS_INTERRUPTS */

#endif /* __CS4224_IRQ_H__ */

/** @file cs4224_irq_tree.h
 *****************************************************************************
 *
 * @brief
 *
 * The interrupt tree
 * 
 * This file is auto generated and should not be modified.
 *
 *****************************************************************************
 * @author
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * 
 *    Copyright (C) 2006-2015 Inphi Corporation, Inc. All rights reserved.
 *
 *    API Version Number: 3.7.8
 ****************************************************************************/

#ifndef __CS4224_IRQ_TREE_H__
#define __CS4224_IRQ_TREE_H__

#if defined(CS_HAS_INTERRUPTS)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CS4224_IRQ_NODE_NO_NODE        0
#define CS4224_IRQ_NODE_SLICED_NO_NODE 0
#define CS4224_IRQ_NO_HANDLER          0
#define CS4224_IRQ_NO_STATUS           0

extern cs4224_irq_node_t  CS4224_IRQ_NODE_GLOBAL_GLOBAL_INTERRUPT;
extern cs4224_irq_node_t              CS4224_IRQ_NODE_MSEQ_PS_INT;
extern cs4224_irq_node_t      CS4224_IRQ_NODE_GPIO_GPIO_INPUT_INT;
extern cs4224_irq_node_t CS4224_IRQ_NODE_GLOBAL_DATAPATH_INTERRUPT;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_HOST_HOSTMISC_INTERRUPT;
extern cs4224_irq_node_t   CS4224_IRQ_NODE_PP_HOST_SDS_COMMON_Int;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_HOST_SDS_COMMON_PRBSCHK0_INTERRUPT;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_HOST_SDS_COMMON_RXLOCKDi;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_HOST_SDS_COMMON_RXLOCKD0_INTERRUPT;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_LINE_LINEMISC_INTERRUPT;
extern cs4224_irq_node_t   CS4224_IRQ_NODE_PP_LINE_SDS_COMMON_Int;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_LINE_SDS_COMMON_RXLOCKDi;
extern cs4224_irq_node_t CS4224_IRQ_NODE_PP_LINE_SDS_COMMON_RXLOCKD0_INTERRUPT;

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* CS_HAS_INTERRUPTS */

#endif /* __CS4224_IRQ_TREE_H__ */



#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif


        
#endif /* __CS_CS4224_API__ */

