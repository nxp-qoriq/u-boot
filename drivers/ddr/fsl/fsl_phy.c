/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#include <common.h>
#include <fsl_ddr_sdram.h>
#include <fsl_errata.h>
#include <fsl_ddr.h>
#include <fsl_immap.h>
#include <asm/io.h>
#if defined(CONFIG_FSL_LSCH2) || defined(CONFIG_FSL_LSCH3) || \
        defined(CONFIG_ARM)
#include <asm/arch/clock.h>
#endif

/*
 * DDR SDRAM Clock Control (DDR_SDRAM_CLK_CNTL)
 * The old controller on the 8540/60 doesn't have this register.
 * Hope it's OK to set it (to 0) anyway.
 */
static void set_ddr_sdram_clk_cntl(fsl_ddr_cfg_regs_t *ddr,
					 const memctl_options_t *popts)
{
	unsigned int clk_adjust;	/* Clock adjust */
	unsigned int ss_en = 0;		/* Source synchronous enable */

#if defined(CONFIG_ARCH_MPC8541) || defined(CONFIG_ARCH_MPC8555)
	/* Per FSL Application Note: AN2805 */
	ss_en = 1;
#endif
	if (fsl_ddr_get_version(0) >= 0x40701) {
		/* clk_adjust in 5-bits on T-series and LS-series */
		clk_adjust = (popts->clk_adjust & 0x1F) << 22;
	} else {
		/* clk_adjust in 4-bits on earlier MPC85xx and P-series */
		clk_adjust = (popts->clk_adjust & 0xF) << 23;
	}

	ddr->ddr_sdram_clk_cntl = (0
				   | ((ss_en & 0x1) << 31)
				   | clk_adjust
				   );
	debug("FSLDDR: clk_cntl = 0x%08x\n", ddr->ddr_sdram_clk_cntl);
}

/* DDR Initialization Address (DDR_INIT_ADDR) */
static void set_ddr_init_addr(fsl_ddr_cfg_regs_t *ddr)
{
	unsigned int init_addr = 0;	/* Initialization address */

	ddr->ddr_init_addr = init_addr;
}

/* DDR Initialization Address (DDR_INIT_EXT_ADDR) */
static void set_ddr_init_ext_addr(fsl_ddr_cfg_regs_t *ddr)
{
	unsigned int uia = 0;	/* Use initialization address */
	unsigned int init_ext_addr = 0;	/* Initialization address */

	ddr->ddr_init_ext_addr = (0
				  | ((uia & 0x1) << 31)
				  | (init_ext_addr & 0xF)
				  );
}

/* DDR Write Leveling Control (DDR_WRLVL_CNTL) */
static void set_ddr_wrlvl_cntl(fsl_ddr_cfg_regs_t *ddr, unsigned int wrlvl_en,
				const memctl_options_t *popts)
{
	/*
	 * First DQS pulse rising edge after margining mode
	 * is programmed (tWL_MRD)
	 */
	unsigned int wrlvl_mrd = 0;
	/* ODT delay after margining mode is programmed (tWL_ODTEN) */
	unsigned int wrlvl_odten = 0;
	/* DQS/DQS_ delay after margining mode is programmed (tWL_DQSEN) */
	unsigned int wrlvl_dqsen = 0;
	/* WRLVL_SMPL: Write leveling sample time */
	unsigned int wrlvl_smpl = 0;
	/* WRLVL_WLR: Write leveling repeition time */
	unsigned int wrlvl_wlr = 0;
	/* WRLVL_START: Write leveling start time */
	unsigned int wrlvl_start = 0;

	/* suggest enable write leveling for DDR3 due to fly-by topology */
	if (wrlvl_en) {
		/* tWL_MRD min = 40 nCK, we set it 64 */
		wrlvl_mrd = 0x6;
		/* tWL_ODTEN 128 */
		wrlvl_odten = 0x7;
		/* tWL_DQSEN min = 25 nCK, we set it 32 */
		wrlvl_dqsen = 0x5;
		/*
		 * Write leveling sample time at least need 6 clocks
		 * higher than tWLO to allow enough time for progagation
		 * delay and sampling the prime data bits.
		 */
		wrlvl_smpl = 0xf;
		/*
		 * Write leveling repetition time
		 * at least tWLO + 6 clocks clocks
		 * we set it 64
		 */
		wrlvl_wlr = 0x6;
		/*
		 * Write leveling start time
		 * The value use for the DQS_ADJUST for the first sample
		 * when write leveling is enabled. It probably needs to be
		 * overridden per platform.
		 */
		wrlvl_start = 0x8;
		/*
		 * Override the write leveling sample and start time
		 * according to specific board
		 */
		if (popts->wrlvl_override) {
			wrlvl_smpl = popts->wrlvl_sample;
			wrlvl_start = popts->wrlvl_start;
		}
	}

	ddr->ddr_wrlvl_cntl = (0
			       | ((wrlvl_en & 0x1) << 31)
			       | ((wrlvl_mrd & 0x7) << 24)
			       | ((wrlvl_odten & 0x7) << 20)
			       | ((wrlvl_dqsen & 0x7) << 16)
			       | ((wrlvl_smpl & 0xf) << 12)
			       | ((wrlvl_wlr & 0x7) << 8)
			       | ((wrlvl_start & 0x1F) << 0)
			       );
	debug("FSLDDR: wrlvl_cntl = 0x%08x\n", ddr->ddr_wrlvl_cntl);
	ddr->ddr_wrlvl_cntl_2 = popts->wrlvl_ctl_2;
	debug("FSLDDR: wrlvl_cntl_2 = 0x%08x\n", ddr->ddr_wrlvl_cntl_2);
	ddr->ddr_wrlvl_cntl_3 = popts->wrlvl_ctl_3;
	debug("FSLDDR: wrlvl_cntl_3 = 0x%08x\n", ddr->ddr_wrlvl_cntl_3);

}

static void set_ddr_cdr1(fsl_ddr_cfg_regs_t *ddr, const memctl_options_t *popts)
{
	ddr->ddr_cdr1 = popts->ddr_cdr1;
	debug("FSLDDR: ddr_cdr1 = 0x%08x\n", ddr->ddr_cdr1);
}

static void set_ddr_cdr2(fsl_ddr_cfg_regs_t *ddr, const memctl_options_t *popts)
{
	ddr->ddr_cdr2 = popts->ddr_cdr2;
	debug("FSLDDR: ddr_cdr2 = 0x%08x\n", ddr->ddr_cdr2);
}

static unsigned int
compute_fsl_phy_config_regs(const memctl_options_t *popts,
			       fsl_ddr_cfg_regs_t *ddr)
{
	unsigned int wrlvl_en;

	/* write leveling */
	wrlvl_en = (popts->wrlvl_en) ? 1 : 0;

	set_ddr_cdr1(ddr, popts);
	set_ddr_cdr2(ddr, popts);

	set_ddr_sdram_clk_cntl(ddr, popts);
	set_ddr_init_addr(ddr);
	set_ddr_init_ext_addr(ddr);

	set_ddr_wrlvl_cntl(ddr, wrlvl_en, popts);

	return 0;
}

int compute_phy_config_regs(const unsigned int ctrl_num,
			    const memctl_options_t *popts,
			    const dimm_params_t *dimm_parameters,
			    fsl_ddr_cfg_regs_t *ddr)
{
	compute_fsl_phy_config_regs(popts, ddr);
	return 0;
}
