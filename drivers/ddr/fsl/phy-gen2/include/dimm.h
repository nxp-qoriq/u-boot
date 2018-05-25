/*
 * Copyright 2018 NXP
 * SPDX-License-Identifier:	GPL-2.0+        BSD-3-Clause
 *
 * Author York Sun <york.sun@nxp.com>
 */

#ifndef _DIMM_H_
#define _DIMM_H_

enum dram_types {
	DDR4,
	DDR3,
	LPDDR4,
	LPDDR3,
	DDR5,
};

enum dimm_types {
	UDIMM,
	SODIMM,
	RDIMM,
	LRDIMM,
	NODIMM,
};

struct dimm {
	enum dram_types dramtype;
	enum dimm_types dimmtype;
	unsigned int	n_ranks;
	unsigned int	data_width;
	unsigned int	primary_sdram_width;
	unsigned int	ec_sdram_width;
	unsigned int	mr[7];
	unsigned int	cs_d0;
	unsigned int	cs_d1;
	unsigned int	odt[4];
	unsigned int	rcw[16];
	unsigned int	rcw3x;
};

#endif /* _DIMM_H_ */
