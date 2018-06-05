/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#include <common.h>
#include <malloc.h>
#include "include/input.h"
#include "include/init.h"
#include "include/io.h"

struct input *phy_gen2_init_input(const unsigned int ctrl_num, struct dimm *dimm,
			      int freq)
{
	struct input *input;
	int i;

	input = calloc(1, sizeof(struct input));
	if (!input) {
		printf("failed to allocate memory\n");
		return NULL;
	}

	input->basic.dram_type = dimm->dramtype;
	input->basic.dimm_type = dimm->dimmtype;
#ifdef CONFIG_ARCH_LX2160A_PXP
	input->basic.num_dbyte = 0x9;
#else
	input->basic.num_dbyte = dimm->primary_sdram_width / 8 +
				 dimm->ec_sdram_width / 8;
#endif
	input->basic.num_rank_dfi0 = dimm->n_ranks;
	input->basic.frequency = freq;
	input->basic.dram_data_width = dimm->data_width;

	input->basic.hard_macro_ver	= 3;
	input->basic.num_pstates	= 1;
	input->basic.dfi_freq_ratio	= 1;
#if !defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
#ifdef CONFIG_ARCH_LX2160A_PXP
	input->basic.read_dbienable	= 0;
#else
	input->basic.read_dbienable	= 1;
#endif
#endif
	input->basic.num_active_dbyte_dfi0 = input->basic.num_dbyte;
	input->basic.num_anib		= 0xc;

#ifdef CONFIG_ARCH_LX2160A_PXP
	input->basic.train2d		= 0;
#else
	input->basic.train2d		= 1;
#endif

	input->adv.dram_byte_swap		= 0;
	input->adv.ext_cal_res_val		= 0;
	input->adv.tx_slew_rise_dq		= 0xf;
	input->adv.tx_slew_fall_dq		= 0xf;
	input->adv.tx_slew_rise_ac		= 0xf;
	input->adv.tx_slew_fall_ac		= 0xf;
	input->adv.odtimpedance		= 60;
	input->adv.tx_impedance		= 28;
	input->adv.atx_impedance		= 30;
	input->adv.mem_alert_en		= 0;
	input->adv.mem_alert_puimp	= 5;
	input->adv.mem_alert_vref_level	= 0x29;
	input->adv.mem_alert_sync_bypass	= 0;
	input->adv.cal_interval		= 0x9;
	input->adv.cal_once		= 0;
	input->adv.dis_dyn_adr_tri		= 0;
	input->adv.is2ttiming		= 0;
	input->adv.d4rx_preamble_length	= 0;
	input->adv.d4tx_preamble_length	= 0;

	debug("input->basic.num_rank_dfi0 = 0x%x\n", input->basic.num_rank_dfi0);
	debug("input->basic.dram_data_width = 0x%x\n", input->basic.dram_data_width);

	for (i = 0; i < 6; i++) {
		input->mr[i] = dimm->mr[i];
		debug("mr[%d] = 0x%x\n", i, input->mr[i]);
	}
	input->mr[6] = dimm->mr[6] & ~0x7f;	/* force to range 1 */
	input->mr[6] |= 0x24;	/* force Vref value to 83.4% */
	debug("mr[6] = 0x%x\n", input->mr[6]);

	input->cs_d0 = dimm->cs_d0;
	input->cs_d1 = dimm->cs_d1;
	input->mirror = dimm->mirror;
	debug("input->cs_d0 = 0x%x\n", input->cs_d0);
	debug("input->cs_d1 = 0x%x\n", input->cs_d1);
	debug("input->mirror = 0x%x\n", input->mirror);

	for (i = 0; i < 4; i++) {
		input->odt[i] = dimm->odt[i];
		debug("odt[%d] = 0x%x\n", i, input->odt[i]);
	}
	for (i = 0; i < 16; i++)
		input->rcw[i] = dimm->rcw[i];
	input->rcw3x = dimm->rcw3x;

	return input;
}

/*
 * All protocols share the same base structure of mesage block.
 * RDIMM and LRDIMM have more entries defined than UDIMM.
 */
int phy_gen2_msg_init(const unsigned int ctrl_num, void **msg_1d, void **msg_2d,
		  struct input *input, size_t *len)
{
	struct ddr4u1d *msg_blk;
	struct ddr4u2d *msg_blk_2d = NULL;
	struct ddr4r1d *msg_blk_r = NULL;
	struct ddr4lr1d *msg_blk_lr = NULL;

	*len = sizeof(struct ddr4u1d);	/* same size for all protocol */
	msg_blk = calloc(1, *len);
	if (!msg_blk) {
		printf("failed to allocate memory\n");
		return -ENOMEM;
	}

	if (input->basic.train2d) {
		msg_blk_2d = calloc(1, *len);
		if (!msg_blk_2d) {
			printf("failed to allocate memory\n");
			free(msg_blk);
			return -ENOMEM;
		}
	}

	switch (input->basic.dimm_type) {
	case UDIMM:
	case SODIMM:
	case NODIMM:
		msg_blk->dram_type	= 0x2;	/* unbuffered */
		break;
	case RDIMM:
		msg_blk->dram_type	= 0x4;
		break;
	case LRDIMM:
		msg_blk->dram_type	= 0x5;
		break;
	}
	msg_blk->pstate			= 0;
#ifdef CONFIG_ARCH_LX2160A_PXP
		msg_blk->sequence_ctrl	= 0x1;
#else
	if (input->basic.dimm_type == LRDIMM)
		msg_blk->sequence_ctrl	= 0x3f1f;
	else
		msg_blk->sequence_ctrl	= 0x031f;
#endif
	msg_blk->phy_config_override	= 0;
	msg_blk->hdt_ctrl		= 0xc9;
#ifdef CONFIG_ARCH_LX2160A_PXP
	msg_blk->msg_misc		= 0x0;
#else
	msg_blk->msg_misc		= 0x0;
#endif
	msg_blk->dfimrlmargin		= 0x1;
	msg_blk->phy_vref		= 0x61;
#ifdef CONFIG_ARCH_LX2160A_PXP
	msg_blk->cs_present		= 3;
	msg_blk->cs_present_d0		= 3;
#else
	msg_blk->cs_present		= input->cs_d0 | input->cs_d1;
	msg_blk->cs_present_d0		= input->cs_d0;
	msg_blk->cs_present_d1		= input->cs_d1;
#endif
	msg_blk->cs_present_d1		= 0;
#ifdef CONFIG_ARCH_LX2160A_PXP
	msg_blk->addr_mirror		= 0;
#else
	if (input->mirror)
		msg_blk->addr_mirror	= 0x0a;	/* odd CS are mirrored */
#endif

	msg_blk->acsm_odt_ctrl0		= input->odt[0];
	msg_blk->acsm_odt_ctrl1		= input->odt[1];
	msg_blk->acsm_odt_ctrl2		= input->odt[2];
	msg_blk->acsm_odt_ctrl3		= input->odt[3];
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	msg_blk->enabled_dqs = (input->basic.num_active_dbyte_dfi0 +
			input->basic.num_active_dbyte_dfi1) * 8;
#else
	msg_blk->enabled_dqs		= 0;	/* FIXME: 0? */
#endif
	msg_blk->phy_cfg		= input->adv.is2ttiming;
	msg_blk->x16present		= input->basic.dram_data_width == 0x10
						? msg_blk->cs_present : 0;
	msg_blk->d4misc			= 0x1;
	msg_blk->cs_setup_gddec		= 0x1;
	msg_blk->rtt_nom_wr_park0	= 0;
	msg_blk->rtt_nom_wr_park1	= 0;
	msg_blk->rtt_nom_wr_park2	= 0;
	msg_blk->rtt_nom_wr_park3	= 0;
	msg_blk->rtt_nom_wr_park4	= 0;
	msg_blk->rtt_nom_wr_park5	= 0;
	msg_blk->rtt_nom_wr_park6	= 0;
	msg_blk->rtt_nom_wr_park7	= 0;
#ifdef CONFIG_ARCH_LX2160A_PXP
	msg_blk->mr0			= 0x44;
	msg_blk->mr1			= 0x1;
	msg_blk->mr2			= 0x28;
	msg_blk->mr3			= 0;
	msg_blk->mr4			= 0;
	msg_blk->mr5			= 0x0400;
	msg_blk->mr6			= 0x0400;
#else
	msg_blk->mr0			= input->mr[0];
	msg_blk->mr1			= input->mr[1];
	msg_blk->mr2			= input->mr[2];
	msg_blk->mr3			= input->mr[3];
	msg_blk->mr4			= input->mr[4];
	msg_blk->mr5			= input->mr[5];
	msg_blk->mr6			= input->mr[6];
#endif
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	if (msg_blk->mr4 & 0x1c0) {
		printf("Error %s: Setting DRAM CAL mode is not supported\n",
		       __func__);
	}
#endif
	msg_blk->alt_cas_l		= 0;
	msg_blk->alt_wcas_l		= 0;

	msg_blk->dramfreq		= input->basic.frequency * 2;
	msg_blk->pll_bypass_en		= input->basic.pll_bypass;
	msg_blk->dfi_freq_ratio		= input->basic.dfi_freq_ratio == 0 ? 1
					: input->basic.dfi_freq_ratio == 1 ? 2
					: 4;
#ifdef CONFIG_ARCH_LX2160A_PXP
	msg_blk->phy_odt_impedance = 0;
	msg_blk->phy_drv_impedance = 0;
#else
	if (input->basic.hard_macro_ver == 4) {
		msg_blk->phy_odt_impedance = 0;
		msg_blk->phy_drv_impedance = 0;
	} else {
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
		msg_blk->phy_odt_impedance = 0;
		msg_blk->phy_drv_impedance = 0;
#else
		msg_blk->phy_odt_impedance = input->adv.odtimpedance;
		msg_blk->phy_drv_impedance = input->adv.tx_impedance;
#endif
	}
#endif
	msg_blk->bpznres_val		= input->adv.ext_cal_res_val;
	msg_blk->disabled_dbyte		= 0;

	debug("msg_blk->dram_type = 0x%x\n", msg_blk->dram_type);
	debug("msg_blk->sequence_ctrl = 0x%x\n", msg_blk->sequence_ctrl);
	debug("msg_blk->phy_cfg = 0x%x\n", msg_blk->phy_cfg);
	debug("msg_blk->x16present = 0x%x\n", msg_blk->x16present);
	debug("msg_blk->dramfreq = 0x%x\n", msg_blk->dramfreq);
	debug("msg_blk->pll_bypass_en = 0x%x\n", msg_blk->pll_bypass_en);
	debug("msg_blk->dfi_freq_ratio = 0x%x\n", msg_blk->dfi_freq_ratio);
	debug("msg_blk->phy_odt_impedance = 0x%x\n", msg_blk->phy_odt_impedance);
	debug("msg_blk->phy_drv_impedance = 0x%x\n", msg_blk->phy_drv_impedance);
	debug("msg_blk->bpznres_val = 0x%x\n", msg_blk->bpznres_val);
	debug("msg_blk->enabled_dqs = 0x%x\n", msg_blk->enabled_dqs);
	debug("msg_blk->phy_cfg = 0x%x\n", msg_blk->phy_cfg);
	debug("msg_blk->acsm_odt_ctrl0 = 0x%x\n", msg_blk->acsm_odt_ctrl0);
	debug("msg_blk->acsm_odt_ctrl1 = 0x%x\n", msg_blk->acsm_odt_ctrl1);
	debug("msg_blk->acsm_odt_ctrl2 = 0x%x\n", msg_blk->acsm_odt_ctrl2);
	debug("msg_blk->acsm_odt_ctrl3 = 0x%x\n", msg_blk->acsm_odt_ctrl3);

	/* RDIMM only */
	if (input->basic.dimm_type == RDIMM ||
	    input->basic.dimm_type == LRDIMM) {
		msg_blk_r = (struct ddr4r1d *)msg_blk;
		if (msg_blk_r->cs_present_d0) {
			msg_blk_r->f0rc00_d0 = input->rcw[0];
			msg_blk_r->f0rc01_d0 = input->rcw[1];
			msg_blk_r->f0rc02_d0 = input->rcw[2];
			msg_blk_r->f0rc03_d0 = input->rcw[3];
			msg_blk_r->f0rc04_d0 = input->rcw[4];
			msg_blk_r->f0rc05_d0 = input->rcw[5];
			msg_blk_r->f0rc06_d0 = input->rcw[6];
			msg_blk_r->f0rc07_d0 = input->rcw[7];
			msg_blk_r->f0rc08_d0 = input->rcw[8];
			msg_blk_r->f0rc09_d0 = input->rcw[9];
			msg_blk_r->f0rc0a_d0 = input->rcw[10];
			msg_blk_r->f0rc0b_d0 = input->rcw[11];
			msg_blk_r->f0rc0c_d0 = input->rcw[12];
			msg_blk_r->f0rc0d_d0 = input->rcw[13];
			msg_blk_r->f0rc0e_d0 = input->rcw[14];
			msg_blk_r->f0rc0f_d0 = input->rcw[15];
			msg_blk_r->f0rc3x_d0 = input->rcw3x;
		}
		if (msg_blk_r->cs_present_d1) {
			msg_blk_r->f0rc00_d1 = input->rcw[0];
			msg_blk_r->f0rc01_d1 = input->rcw[1];
			msg_blk_r->f0rc02_d1 = input->rcw[2];
			msg_blk_r->f0rc03_d1 = input->rcw[3];
			msg_blk_r->f0rc04_d1 = input->rcw[4];
			msg_blk_r->f0rc05_d1 = input->rcw[5];
			msg_blk_r->f0rc06_d1 = input->rcw[6];
			msg_blk_r->f0rc07_d1 = input->rcw[7];
			msg_blk_r->f0rc08_d1 = input->rcw[8];
			msg_blk_r->f0rc09_d1 = input->rcw[9];
			msg_blk_r->f0rc0a_d1 = input->rcw[10];
			msg_blk_r->f0rc0b_d1 = input->rcw[11];
			msg_blk_r->f0rc0c_d1 = input->rcw[12];
			msg_blk_r->f0rc0d_d1 = input->rcw[13];
			msg_blk_r->f0rc0e_d1 = input->rcw[14];
			msg_blk_r->f0rc0f_d1 = input->rcw[15];
			msg_blk_r->f0rc3x_d1 = input->rcw3x;
		}
		if (input->basic.dimm_type == LRDIMM) {
			msg_blk_lr = (struct ddr4lr1d *)msg_blk;
			msg_blk_lr->bc0a_d0 = msg_blk_lr->f0rc0a_d0;
			msg_blk_lr->bc0a_d1 = msg_blk_lr->f0rc0a_d1;
			msg_blk_lr->f0bc6x_d0 = msg_blk_lr->f0rc3x_d0;
			msg_blk_lr->f0bc6x_d1 = msg_blk_lr->f0rc3x_d1;
		}
	}

	/* below is different for 1D and 2D message block */
	if (input->basic.train2d) {
		memcpy(msg_blk_2d, msg_blk, *len);
		msg_blk_2d->sequence_ctrl	= 0x0061;
		msg_blk_2d->rx2d_train_opt	= 1;
		msg_blk_2d->tx2d_train_opt	= 1;
		msg_blk_2d->share2dvref_result	= 1;
		msg_blk_2d->delay_weight2d	= 0x4;
		msg_blk_2d->voltage_weight2d	= 0x1;
	}

	*msg_1d = msg_blk;
	*msg_2d = msg_blk_2d;

	return 0;
}

static void prog_tx_pre_drv_mode(const unsigned int ctrl_num,
				 const struct input *input)
{
	int lane, byte, b_addr, c_addr, p_addr;
	int tx_slew_rate, tx_pre_p, tx_pre_n;
	int tx_pre_drv_mode = 0x2;
	uint32_t addr;

	/* Program TxPreDrvMode with 0x2 */
	/* FIXME: TxPreDrvMode depends on DramType? */
	tx_pre_p = input->adv.tx_slew_rise_dq;
	tx_pre_n = input->adv.tx_slew_fall_dq;
	tx_slew_rate = tx_pre_drv_mode << csr_tx_pre_drv_mode_lsb	|
		     tx_pre_p << csr_tx_pre_p_lsb			|
		     tx_pre_n << csr_tx_pre_n_lsb;
	p_addr = 0;
	for (byte = 0; byte < input->basic.num_dbyte; byte++) {
		c_addr = byte << 12;
		for (lane = 0; lane <= 1; lane++) {
			b_addr = lane << 8;
			addr = p_addr | t_dbyte | c_addr | b_addr |
					csr_tx_slew_rate_addr;
			phy_io_write16(ctrl_num, addr, tx_slew_rate);
		}
	}
}

static void prog_atx_pre_drv_mode(const unsigned int ctrl_num,
				  const struct input *input)
{
	int anib, c_addr;
	int atx_slew_rate, atx_pre_p, atx_pre_n, atx_pre_drv_mode,
		ck_anib_inst[2];
	uint32_t addr;

	atx_pre_n = input->adv.tx_slew_fall_ac;
	atx_pre_p = input->adv.tx_slew_rise_ac;

	if (input->basic.num_anib == 10 || input->basic.num_anib == 12 ||
	    input->basic.num_anib == 13) {
		ck_anib_inst[0] = 4;
		ck_anib_inst[1] = 5;
	} else {
		printf("error: invalid number of aNIBs: %d\n",
		       input->basic.num_anib);

		return;
	}
	for (anib = 0; anib < input->basic.num_anib; anib++) {
		c_addr = anib << 12;
		if (anib == ck_anib_inst[0] || anib == ck_anib_inst[1])
			atx_pre_drv_mode = 0;
		else
			atx_pre_drv_mode = 3;
		atx_slew_rate = atx_pre_drv_mode << csr_atx_pre_drv_mode_lsb |
			      atx_pre_n << csr_atx_pre_n_lsb		|
			      atx_pre_p << csr_atx_pre_p_lsb;
		addr = t_anib | c_addr | csr_atx_slew_rate_addr;
		phy_io_write16(ctrl_num, addr, atx_slew_rate);
	}
}

static void prog_enable_cs_multicast(const unsigned int ctrl_num,
				     const struct input *input)
{
	uint32_t addr = t_master | csr_enable_cs_multicast_addr;

	if (input->basic.dimm_type != RDIMM &&
	    input->basic.dimm_type != LRDIMM)
		return;

	phy_io_write16(ctrl_num, addr, input->adv.cast_cs_to_cid);
}

static void prog_dfi_rd_data_cs_dest_map(const unsigned int ctrl_num,
					 const struct input *input,
					 const struct ddr4lr1d *msg)
{
	uint16_t dfi_xxdestm0 = 0;
	uint16_t dfi_xxdestm1 = 0;
	uint16_t dfi_xxdestm2 = 0;
	uint16_t dfi_xxdestm3 = 0;
	uint16_t dfi_rd_data_cs_dest_map;
	uint16_t dfi_wr_data_cs_dest_map;

	if (input->basic.dimm_type != LRDIMM)
		return;

	if (msg->cs_present_d1) {
		dfi_xxdestm2 = 1;
		dfi_xxdestm3 = 1;
	}

	dfi_rd_data_cs_dest_map = dfi_xxdestm0 << csr_dfi_rd_destm0_lsb	|
			     dfi_xxdestm1 << csr_dfi_rd_destm1_lsb	|
			     dfi_xxdestm2 << csr_dfi_rd_destm2_lsb	|
			     dfi_xxdestm3 << csr_dfi_rd_destm3_lsb;
	dfi_wr_data_cs_dest_map = dfi_xxdestm0 << csr_dfi_wr_destm0_lsb	|
			     dfi_xxdestm1 << csr_dfi_wr_destm1_lsb	|
			     dfi_xxdestm2 << csr_dfi_wr_destm2_lsb	|
			     dfi_xxdestm3 << csr_dfi_wr_destm3_lsb;
	phy_io_write16(ctrl_num, t_master | csr_dfi_rd_data_cs_dest_map_addr,
		       dfi_rd_data_cs_dest_map);
	phy_io_write16(ctrl_num, t_master | csr_dfi_wr_data_cs_dest_map_addr,
		       dfi_wr_data_cs_dest_map);
}

static void prog_pll_ctrl2(const unsigned int ctrl_num,
			   const struct input *input)
{
	int pll_ctrl2;
	uint32_t addr = t_master | csr_pll_ctrl2_addr;

	/* FIXME: is /2 correct here? */
	if (input->basic.frequency / 2 < 235)
		pll_ctrl2 = 0x7;
	else if (input->basic.frequency / 2 < 313)
		pll_ctrl2 = 0x6;
	else if (input->basic.frequency / 2 < 469)
		pll_ctrl2 = 0xb;
	else if (input->basic.frequency / 2 < 625)
		pll_ctrl2 = 0xa;
	else if (input->basic.frequency / 2 < 938)
		pll_ctrl2 = 0x19;
	else if (input->basic.frequency / 2 < 1067)
		pll_ctrl2 = 0x18;
	else
		pll_ctrl2 = 0x19;

	phy_io_write16(ctrl_num, addr, pll_ctrl2);
}

static void prog_ard_ptr_init_val(const unsigned int ctrl_num,
				  const struct input *input)
{
	int ard_ptr_init_val;
	uint32_t addr = t_master | csr_ard_ptr_init_val_addr;

	if (input->basic.frequency >= 933)
		ard_ptr_init_val = 0x2;
	else
		ard_ptr_init_val = 0x1;

	phy_io_write16(ctrl_num, addr, ard_ptr_init_val);
}

static void prog_dqs_preamble_control(const unsigned int ctrl_num,
				      const struct input *input)
{
	int data;
	uint32_t addr = t_master | csr_dqs_preamble_control_addr;
	const int wdqsextension = 0;
	const int lp4sttc_pre_bridge_rx_en = 0;
	const int lp4postamble_ext = 0;
	const int lp4tgl_two_tck_tx_dqs_pre = 0;
	const int position_dfe_init = 2;
	const int dll_rx_preamble_mode = 1;
	int two_tck_tx_dqs_pre = input->adv.d4tx_preamble_length;
	int two_tck_rx_dqs_pre = input->adv.d4rx_preamble_length;

	data = wdqsextension << csr_wdqsextension_lsb			|
	       lp4sttc_pre_bridge_rx_en << csr_lp4sttc_pre_bridge_rx_en_lsb |
	       lp4postamble_ext << csr_lp4postamble_ext_lsb		|
	       lp4tgl_two_tck_tx_dqs_pre << csr_lp4tgl_two_tck_tx_dqs_pre_lsb |
	       position_dfe_init << csr_position_dfe_init_lsb		|
	       two_tck_tx_dqs_pre << csr_two_tck_tx_dqs_pre_lsb		|
	       two_tck_rx_dqs_pre << csr_two_tck_rx_dqs_pre_lsb;
	phy_io_write16(ctrl_num, addr, data);

	data = dll_rx_preamble_mode << csr_dll_rx_preamble_mode_lsb;
	addr = t_master | csr_dbyte_dll_mode_cntrl_addr;
	phy_io_write16(ctrl_num, addr, data);
}

static void prog_proc_odt_time_ctl(const unsigned int ctrl_num,
				   const struct input *input)
{
	int proc_odt_time_ctl;
	uint32_t addr = t_master | csr_proc_odt_time_ctl_addr;

	if (input->adv.wdqsext) {
		proc_odt_time_ctl = 0x3;
	} else if (input->basic.frequency <= 933) {
		proc_odt_time_ctl = 0xa;
	} else if (input->basic.frequency <= 1200) {
		if (input->adv.d4rx_preamble_length == 1)
			proc_odt_time_ctl = 0x2;
		else
			proc_odt_time_ctl = 0x6;
	} else {
		if (input->adv.d4rx_preamble_length == 1)
			proc_odt_time_ctl = 0x3;
		else
			proc_odt_time_ctl = 0x7;
	}
	phy_io_write16(ctrl_num, addr, proc_odt_time_ctl);
}

static const struct impedance_mapping map[] = {
	{	29,	0x3f	},
	{	31,	0x3e	},
	{	33,	0x3b	},
	{	36,	0x3a	},
	{	39,	0x39	},
	{	42,	0x38	},
	{	46,	0x1b	},
	{	51,	0x1a	},
	{	57,	0x19	},
	{	64,	0x18	},
	{	74,	0x0b	},
	{	88,	0x0a	},
	{	108,	0x09	},
	{	140,	0x08	},
	{	200,	0x03	},
	{	360,	0x02	},
	{	481,	0x01	},
	{}
};

static int map_impedance(int strength)
{
	const struct impedance_mapping *tbl = map;
	int val = 0;

	if (strength == 0)
		return 0;

	while (tbl->ohm) {
		if (strength < tbl->ohm) {
			val = tbl->code;
			break;
		}
		tbl++;
	}

	return val;
}

static int map_odtstren_p(int strength, int hard_macro_ver)
{
	int val = -1;

	if (hard_macro_ver == 4) {
		if (strength == 0)
			val = 0;
		else if (strength == 120)
			val = 0x8;
		else if (strength == 60)
			val = 0x18;
		else if (strength == 40)
			val = 0x38;
		else
			printf("error: unsupported ODTStrenP %d\n", strength);
	} else {
		val = map_impedance(strength);
	}

	return val;
}

#ifdef CONFIG_ARCH_LX2160A_PXP
void prog_tx_odt_drv_stren(const unsigned int ctrl_num,
			   const struct input *input)
#else
static void prog_tx_odt_drv_stren(const unsigned int ctrl_num,
			   const struct input *input)
#endif
{
	int lane, byte, b_addr, c_addr;
	int tx_odt_drv_stren;
	int odtstren_p, odtstren_n;
	uint32_t addr;

	odtstren_p = map_odtstren_p(input->adv.odtimpedance,
				input->basic.hard_macro_ver);
	if (odtstren_p < 0)
		return;

	odtstren_n = 0;	/* always high-z */
	tx_odt_drv_stren = odtstren_n << csr_odtstren_n_lsb | odtstren_p;
	for (byte = 0; byte < input->basic.num_dbyte; byte++) {
		c_addr = byte << 12;
		for (lane = 0; lane <= 1; lane++) {
			b_addr = lane << 8;
			addr = t_dbyte | c_addr | b_addr |
				csr_tx_odt_drv_stren_addr;
#ifdef CONFIG_ARCH_LX2160A_PXP
			phy_io_write16_debug(ctrl_num, addr, 0);
#else
			phy_io_write16(ctrl_num, addr, tx_odt_drv_stren);
#endif
		}
	}
}

static int map_drvstren_fsdq_p(int strength, int hard_macro_ver)
{
	int val = -1;

	if (hard_macro_ver == 4) {
		if (strength == 0)
			val = 0x07;
		else if (strength == 120)
			val = 0x0F;
		else if (strength == 60)
			val = 0x1F;
		else if (strength == 40)
			val = 0x3F;
		else
			printf("error: unsupported drv_stren_fSDq_p %d\n",
			       strength);
	} else {
		val = map_impedance(strength);
	}

	return val;
}

static int map_drvstren_fsdq_n(int strength, int hard_macro_ver)
{
	int val = -1;

	if (hard_macro_ver == 4) {
		if (strength == 0)
			val = 0x00;
		else if (strength == 120)
			val = 0x08;
		else if (strength == 60)
			val = 0x18;
		else if (strength == 40)
			val = 0x38;
		else
			printf("error: unsupported drvStrenFSDqN %d\n",
			       strength);
	} else {
		val = map_impedance(strength);
	}

	return val;
}

static void prog_tx_impedance_ctrl1(const unsigned int ctrl_num,
				    const struct input *input)
{
	int lane, byte, b_addr, c_addr;
	int tx_impedance_ctrl1;
	int drv_stren_fsdq_p, drv_stren_fsdq_n;
	uint32_t addr;

	drv_stren_fsdq_p = map_drvstren_fsdq_p(input->adv.tx_impedance,
					input->basic.hard_macro_ver);
	drv_stren_fsdq_n = map_drvstren_fsdq_n(input->adv.tx_impedance,
					input->basic.hard_macro_ver);
	tx_impedance_ctrl1 = drv_stren_fsdq_n << csr_drv_stren_fsdq_n_lsb |
			   drv_stren_fsdq_p << csr_drv_stren_fsdq_p_lsb;
	for (byte = 0; byte < input->basic.num_dbyte; byte++) {
		c_addr = byte << 12;
		for (lane = 0; lane <= 1; lane++) {
			b_addr = lane << 8;
			addr = t_dbyte | c_addr | b_addr |
				csr_tx_impedance_ctrl1_addr;
			phy_io_write16(ctrl_num, addr, tx_impedance_ctrl1);
		}
	}
}

static int map_adrv_stren_p(int strength, int hard_macro_ver)
{
	int val = -1;

	if (hard_macro_ver == 4) {
		if (strength == 120)
			val = 0x1c;
		else if (strength == 60)
			val = 0x1d;
		else if (strength == 40)
			val = 0x1f;
		else
			printf("error: unsupported aDrv_stren_p %d\n",
			       strength);
	} else {
		if (strength == 120)
			val = 0x00;
		else if (strength == 60)
			val = 0x01;
		else if (strength == 40)
			val = 0x03;
		else if (strength == 30)
			val = 0x07;
		else if (strength == 24)
			val = 0x0f;
		else if (strength == 20)
			val = 0x1f;
		else
			printf("error: unsupported aDrv_stren_p %d\n",
			       strength);
	}

	return val;
}

static int map_adrv_stren_n(int strength, int hard_macro_ver)
{
	int val = -1;

	if (hard_macro_ver == 4) {
		if (strength == 120)
			val = 0x00;
		else if (strength == 60)
			val = 0x01;
		else if (strength == 40)
			val = 0x03;
		else
			printf("Error: unsupported ADrvStrenP %d\n", strength);
	} else {
		if (strength == 120)
			val = 0x00;
		else if (strength == 60)
			val = 0x01;
		else if (strength == 40)
			val = 0x03;
		else if (strength == 30)
			val = 0x07;
		else if (strength == 24)
			val = 0x0f;
		else if (strength == 20)
			val = 0x1f;
		else
			printf("Error: unsupported ADrvStrenP %d\n", strength);
	}

	return val;
}

static void prog_atx_impedance(const unsigned int ctrl_num,
			       const struct input *input)
{
	int anib, c_addr;
	int atx_impedance;
	int adrv_stren_p;
	int adrv_stren_n;
	uint32_t addr;

	if (input->basic.hard_macro_ver == 4 &&
	    input->adv.atx_impedance == 20) {
		printf("Error:ATxImpedance has to be 40 for HardMacroVer 4\n");
		return;
	}

	adrv_stren_p = map_adrv_stren_p(input->adv.atx_impedance,
					input->basic.hard_macro_ver);
	adrv_stren_n = map_adrv_stren_n(input->adv.atx_impedance,
					input->basic.hard_macro_ver);
	atx_impedance = adrv_stren_n << csr_adrv_stren_n_lsb		|
		       adrv_stren_p << csr_adrv_stren_p_lsb;
	for (anib = 0; anib < input->basic.num_anib; anib++) {
		c_addr = anib << 12;
		addr = t_anib | c_addr | csr_atx_impedance_addr;
		phy_io_write16(ctrl_num, addr, atx_impedance);
	}
}

static void prog_dfi_mode(const unsigned int ctrl_num,
			  const struct input *input)
{
	int dfi_mode;
	uint32_t addr;

	if (input->basic.dfi1exists == 1)
		dfi_mode = 0x5;	/* DFI1 exists but disabled */
	else
		dfi_mode = 0x1;	/* DFI1 does not physically exists */
	addr = t_master | csr_dfi_mode_addr;
	phy_io_write16(ctrl_num, addr, dfi_mode);
}

static void prog_dfi_camode(const unsigned int ctrl_num,
			    const struct input *input)
{
	int dfi_camode = 2;
	uint32_t addr = t_master | csr_dfi_camode_addr;

	phy_io_write16(ctrl_num, addr, dfi_camode);
}

static void prog_cal_drv_str0(const unsigned int ctrl_num,
			      const struct input *input)
{
	int cal_drv_str0;
	int cal_drv_str_pd50;
	int cal_drv_str_pu50;
	uint32_t addr;

	cal_drv_str_pu50 = input->adv.ext_cal_res_val;
	cal_drv_str_pd50 = cal_drv_str_pu50;
	cal_drv_str0 = cal_drv_str_pu50 << csr_cal_drv_str_pu50_lsb |
			cal_drv_str_pd50;
	addr = t_master | csr_cal_drv_str0_addr;
	phy_io_write16(ctrl_num, addr, cal_drv_str0);
}

static void prog_cal_uclk_info(const unsigned int ctrl_num,
			       const struct input *input)
{
	int cal_uclk_ticks_per1u_s;
	uint32_t addr;

	cal_uclk_ticks_per1u_s = input->basic.frequency >> 1;
	if (cal_uclk_ticks_per1u_s < 24)
		cal_uclk_ticks_per1u_s = 24;

	addr = t_master | csr_cal_uclk_info_addr;
	phy_io_write16(ctrl_num, addr, cal_uclk_ticks_per1u_s);
}

static void prog_cal_rate(const unsigned int ctrl_num,
			  const struct input *input)
{
	int cal_rate;
	int cal_interval;
	int cal_once;
	uint32_t addr;

	cal_interval = input->adv.cal_interval;
	cal_once = input->adv.cal_once;
	cal_rate = cal_once << csr_cal_once_lsb		|
		  cal_interval << csr_cal_interval_lsb;
	addr = t_master | csr_cal_rate_addr;
	phy_io_write16(ctrl_num, addr, cal_rate);
}

#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
static void prog_vref_in_global(const unsigned int ctrl_num,
				const struct input *input,
				const struct ddr4u1d *msg)
#else
static void prog_vref_in_global(const unsigned int ctrl_num,
				const struct input *input)
#endif
{
	int vref_in_global;
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	int global_vref_in_dac = 0;
#else
	int global_vref_in_dac = 81;	/* 0.75 * vddq */
#endif
	int global_vref_in_sel = 0;
	uint32_t addr;

#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	float phy_vref_prcnt = msg->phy_vref / 128.0;
	global_vref_in_dac = (phy_vref_prcnt - 0.345) / 0.005;
#endif

	vref_in_global = global_vref_in_dac << csr_global_vref_in_dac_lsb |
		       global_vref_in_sel;
	addr = t_master | csr_vref_in_global_addr;
	phy_io_write16(ctrl_num, addr, vref_in_global);
}

static void prog_dq_dqs_rcv_cntrl(const unsigned int ctrl_num,
				  const struct input *input)
{
	int lane, byte, b_addr, c_addr;
	int dq_dqs_rcv_cntrl;
	int gain_curr_adj_defval = 0xb;
	int major_mode_dbyte = 3;
	int dfe_ctrl_defval = 0;
	int ext_vref_range_defval = 0;
	int sel_analog_vref = 1;
	uint32_t addr;

	dq_dqs_rcv_cntrl = gain_curr_adj_defval << csr_gain_curr_adj_lsb |
			major_mode_dbyte << csr_major_mode_dbyte_lsb	|
			dfe_ctrl_defval << csr_dfe_ctrl_lsb		|
			ext_vref_range_defval << csr_ext_vref_range_lsb	|
			sel_analog_vref << csr_sel_analog_vref_lsb;
	for (byte = 0; byte < input->basic.num_dbyte; byte++) {
		c_addr = byte << 12;
		for (lane = 0; lane <= 1; lane++) {
			b_addr = lane << 8;
			addr = t_dbyte | c_addr | b_addr |
					csr_dq_dqs_rcv_cntrl_addr;
			phy_io_write16(ctrl_num, addr, dq_dqs_rcv_cntrl);
		}
	}
}

static void prog_mem_alert_control(const unsigned int ctrl_num,
				   const struct input *input)
{
	int mem_alert_control;
	int mem_alert_control2;
	int malertpu_en;
	int malertrx_en;
	int malertvref_level;
	int malertpu_stren;
	int malertsync_bypass;
	int malertdisable_val_defval = 1;
	uint32_t addr;

	if (input->basic.dram_type ==  DDR4 && input->adv.mem_alert_en == 1) {
		malertpu_en = 1;
		malertrx_en = 1;
		malertpu_stren = input->adv.mem_alert_puimp;
		malertvref_level = input->adv.mem_alert_vref_level;
		malertsync_bypass = input->adv.mem_alert_sync_bypass;
		mem_alert_control = malertdisable_val_defval << 14	|
				  malertrx_en << 13		|
				  malertpu_en << 12		|
				  malertpu_stren << 8		|
				  malertvref_level;
		mem_alert_control2 = malertsync_bypass <<
					csr_malertsync_bypass_lsb;
		addr = t_master | csr_mem_alert_control_addr;
		phy_io_write16(ctrl_num, addr, mem_alert_control);
		addr = t_master | csr_mem_alert_control2_addr;
		phy_io_write16(ctrl_num, addr, mem_alert_control2);
	}
}

static void prog_dfi_freq_ratio(const unsigned int ctrl_num,
				const struct input *input)
{
	int dfi_freq_ratio;
	uint32_t addr = t_master | csr_dfi_freq_ratio_addr;

	dfi_freq_ratio = input->basic.dfi_freq_ratio;
	phy_io_write16(ctrl_num, addr, dfi_freq_ratio);
}

static void prog_tristate_mode_ca(const unsigned int ctrl_num,
				  const struct input *input)
{
	int tristate_mode_ca;
	int dis_dyn_adr_tri;
	int ddr2tmode;
	int ck_dis_val_def = 1;
	uint32_t addr = t_master | csr_tristate_mode_ca_addr;

	dis_dyn_adr_tri = input->adv.dis_dyn_adr_tri;
	ddr2tmode = input->adv.is2ttiming;
	tristate_mode_ca = ck_dis_val_def << csr_ck_dis_val_lsb	|
			 ddr2tmode << csr_ddr2tmode_lsb		|
			 dis_dyn_adr_tri << csr_dis_dyn_adr_tri_lsb;
	phy_io_write16(ctrl_num, addr, tristate_mode_ca);
}

static void prog_dfi_xlat(const unsigned int ctrl_num,
			  const struct input *input)
{
	uint16_t loop_vector;
	int dfifreqxlat_dat;
	int pllbypass_dat;
	uint32_t addr;

	/* fIXME: Shall unused P1, P2, P3 be bypassed? */
	pllbypass_dat = input->basic.pll_bypass; /* only [0] is used */
	for (loop_vector = 0; loop_vector < 8; loop_vector++) {
		if (loop_vector == 0)
			dfifreqxlat_dat = pllbypass_dat + 0x5555;
		else if (loop_vector == 7)
			dfifreqxlat_dat = 0xf000;
		else
			dfifreqxlat_dat = 0x5555;
		addr = t_master | (csr_dfi_freq_xlat0_addr + loop_vector);
		phy_io_write16(ctrl_num, addr, dfifreqxlat_dat);
	}
}

#ifdef CONFIG_FSL_PHY_GEN2_PHY_A2017_11
void prog_seq0bdly0(const unsigned int ctrl_num,
			   const struct input *input)
#else
static void prog_seq0bdly0(const unsigned int ctrl_num,
			   const struct input *input)
#endif
{
	int ps_count[4];
#ifdef CONFIG_FSL_PHY_GEN2_PHY_A2017_11
	double frq;
#else
	int frq;
#endif
	uint32_t addr;
#ifdef CONFIG_FSL_PHY_GEN2_PHY_A2017_11
	int lower_freq_opt = 0;
#endif

	frq = input->basic.frequency >> 1;
#ifdef CONFIG_FSL_PHY_GEN2_PHY_A2017_11
	ps_count[0] = (int)(0.5 * 0.25 * frq);
	if (input->basic.frequency < 400)
		lower_freq_opt = (input->basic.dimm_type == RDIMM) ? 7 : 3;
	else if (input->basic.frequency < 533)
		lower_freq_opt = (input->basic.dimm_type == RDIMM) ? 14 : 11;

	ps_count[1] = (int)(1.0 * 0.25 * frq) - lower_freq_opt;
	ps_count[2] = (int) (10.0 * 0.25 * frq);

	if (frq > 266.5)
		ps_count[3] = 44;
	else if (frq > 200)
		ps_count[3] = 33;
	else
		ps_count[3] = 16;
#else
	ps_count[0] = (frq >> 3) + 1;
	ps_count[1] = (frq >> 2) + 1;
	ps_count[2] = input->basic.frequency + (frq >> 1) + 1;

	if (frq > 267)
		ps_count[3] = 44;
	else if (frq > 200)
		ps_count[3] = 33;
	else
		ps_count[3] = 16;
#endif
	addr = t_master | csr_seq0bdly0_addr;
	phy_io_write16(ctrl_num, addr, ps_count[0]);
	addr = t_master | csr_seq0bdly1_addr;
	phy_io_write16(ctrl_num, addr, ps_count[1]);
	addr = t_master | csr_seq0bdly2_addr;
	phy_io_write16(ctrl_num, addr, ps_count[2]);
	addr = t_master | csr_seq0bdly3_addr;
	phy_io_write16(ctrl_num, addr, ps_count[3]);
}

#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
static void prog_dbyte_misc_mode(const unsigned int ctrl_num,
				 const struct input *input,
				 const struct ddr4u1d *msg)
#else
static void prog_dbyte_misc_mode(const unsigned int ctrl_num,
				 const struct input *input)
#endif
{
	int dbyte_misc_mode;
	int dq_dqs_rcv_cntrl1;
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	int dq_dqs_rcv_cntrl1_1;
#endif
	int byte, c_addr;
	uint32_t addr;

	dbyte_misc_mode = 0x1 << csr_dbyte_disable_lsb;
	dq_dqs_rcv_cntrl1 = 0x1ff << csr_power_down_rcvr_lsb		|
			 0x1 << csr_power_down_rcvr_dqs_lsb	|
			 0x1 << csr_rx_pad_standby_en_lsb;
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	dq_dqs_rcv_cntrl1_1 = (0x100 << csr_power_down_rcvr_lsb |
			csr_rx_pad_standby_en_mask) ;
#endif
	for (byte = 0; byte < input->basic.num_dbyte; byte++) {
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
		c_addr = byte << 12;
		if (byte <= input->basic.num_active_dbyte_dfi0 - 1) {
			/* disable RDBI lane if not used. */
			if ((input->basic.dram_data_width != 4) &&
				(((msg->mr5 >> 12) & 0x1) == 0)) {
				addr = t_dbyte | c_addr | csr_dq_dqs_rcv_cntrl1_addr;
				phy_io_write16(ctrl_num, addr, dq_dqs_rcv_cntrl1_1);
			}
		} else {
			addr = t_dbyte | c_addr | csr_dbyte_misc_mode_addr;
			phy_io_write16(ctrl_num, addr, dbyte_misc_mode);
			addr = t_dbyte | c_addr | csr_dq_dqs_rcv_cntrl1_addr;
			phy_io_write16(ctrl_num, addr, dq_dqs_rcv_cntrl1);
		}
#else
		if (byte <= input->basic.num_active_dbyte_dfi0 - 1)
			continue;
		c_addr = byte << 12;
		addr = t_dbyte | c_addr | csr_dbyte_misc_mode_addr;
		phy_io_write16(ctrl_num, addr, dbyte_misc_mode);
		addr = t_dbyte | c_addr | csr_dq_dqs_rcv_cntrl1_addr;
		phy_io_write16(ctrl_num, addr, dq_dqs_rcv_cntrl1);
#endif
	}
}

static void prog_master_x4config(const unsigned int ctrl_num,
				 const struct input *input)
{
	int master_x4config;
	int x4tg;
	uint32_t addr = t_master | csr_master_x4config_addr;

	x4tg = input->basic.dram_data_width == 4 ? 0xf : 0;
	master_x4config = x4tg << csr_x4tg_lsb;
	phy_io_write16(ctrl_num, addr, master_x4config);
}

#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
static void prog_dmipin_present(const unsigned int ctrl_num,
				const struct input *input,
				const struct ddr4u1d *msg)
#else
static void prog_dmipin_present(const unsigned int ctrl_num,
				const struct input *input)
#endif
{
	int dmipin_present;
	uint32_t addr = t_master | csr_dmipin_present_addr;

#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	dmipin_present = (msg->mr5 >> 12) & 0x1;
#else
	dmipin_present = input->basic.read_dbienable;
#endif
	phy_io_write16(ctrl_num, addr, dmipin_present);
}

int c_init_phy_config(const unsigned int ctrl_num, struct input *input,
		      const void *msg)
{
	prog_tx_pre_drv_mode(ctrl_num, input);
	prog_atx_pre_drv_mode(ctrl_num, input);
	prog_enable_cs_multicast(ctrl_num, input);	/* rdimm and lrdimm */
	prog_dfi_rd_data_cs_dest_map(ctrl_num, input, msg);	/* lrdimm */
	prog_pll_ctrl2(ctrl_num, input);
	prog_ard_ptr_init_val(ctrl_num, input);
	prog_dqs_preamble_control(ctrl_num, input);
	prog_proc_odt_time_ctl(ctrl_num, input);
	prog_tx_odt_drv_stren(ctrl_num, input);
	prog_tx_impedance_ctrl1(ctrl_num, input);
	prog_atx_impedance(ctrl_num, input);
	prog_dfi_mode(ctrl_num, input);
	prog_dfi_camode(ctrl_num, input);
	prog_cal_drv_str0(ctrl_num, input);
	prog_cal_uclk_info(ctrl_num, input);
	prog_cal_rate(ctrl_num, input);
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	prog_vref_in_global(ctrl_num, input, msg);
#else
	prog_vref_in_global(ctrl_num, input);
#endif
	prog_dq_dqs_rcv_cntrl(ctrl_num, input);
	prog_mem_alert_control(ctrl_num, input);
	prog_dfi_freq_ratio(ctrl_num, input);
	prog_tristate_mode_ca(ctrl_num, input);
	prog_dfi_xlat(ctrl_num, input);
#if !defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	prog_seq0bdly0(ctrl_num, input);
#endif
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	prog_dbyte_misc_mode(ctrl_num, input, msg);
#else
	prog_dbyte_misc_mode(ctrl_num, input);
#endif
	prog_master_x4config(ctrl_num, input);
#if defined(CONFIG_FSL_PHY_GEN2_PHY_A2017_11)
	prog_dmipin_present(ctrl_num, input, msg);
#else
	prog_dmipin_present(ctrl_num, input);
#endif

	return 0;
}
