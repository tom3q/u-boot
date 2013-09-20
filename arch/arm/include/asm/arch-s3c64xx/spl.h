/*
 * Copyright (C) 2013 Marek Vasut <marex@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_SPL_H__
#define __ASM_ARCH_SPL_H__

#define BOOT_DEVICE_NONE	0
#define BOOT_DEVICE_RAM		1

struct s3c64xx_clk_cfg {
	bool sync;
	u32 clk_div0;
	u32 apll_con;
	u32 mpll_con;
	u32 epll_con0;
	u32 epll_con1;
};

struct s3c64xx_mem_cfg {
	u32 refresh_prd;
	u32 cas_latency;
	u32 t_dqss;
	u32 t_mrd;
	u32 t_ras;
	u32 t_rc;
	u32 t_rcd;
	u32 t_rfc;
	u32 t_rp;
	u32 t_rrd;
	u32 t_wr;
	u32 t_wtr;
	u32 t_xp;
	u32 t_xsr;
	u32 t_esr;
	u32 mem_cfg;
	u32 mem_cfg2;
	u32 chip0_cfg;
	u32 user_config;
};

const struct s3c64xx_clk_cfg *board_get_clk_cfg(void);
const struct s3c64xx_mem_cfg *board_get_mem_cfg(void);
void board_init_gpio(void);

#endif	/* __ASM_ARCH_SPL_H__ */
