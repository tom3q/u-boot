/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <netdev.h>
#include <asm/arch/cpu.h>
#include <asm/samsung-common/sromc.h>
#include <asm/arch/mmc.h>

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */
#define CS8900_Tacs	0x0	/* 0clk		address set-up		*/
#define CS8900_Tcos	0x4	/* 4clk		chip selection set-up	*/
#define CS8900_Tacc	0xE	/* 14clk	access cycle		*/
#define CS8900_Tcoh	0x1	/* 1clk		chip selection hold	*/
#define CS8900_Tah	0x4	/* 4clk		address holding time	*/
#define CS8900_Tacp	0x6	/* 6clk		page mode access cycle	*/
#define CS8900_PMC	0x0	/* normal(1data)page mode configuration	*/

/*
 * Miscellaneous platform dependent initialisations
 */

static void cs8900_pre_init(void)
{
	u32 bw_conf, bc_conf;

	bw_conf = (1 << 7) | (1 << 6) | (1 << 4);
	bc_conf = ((CS8900_Tacs << 28) + (CS8900_Tcos << 24) +
			(CS8900_Tacc << 16) + (CS8900_Tcoh << 12) +
			(CS8900_Tah << 8) + (CS8900_Tacp << 4) + CS8900_PMC);

	s5p_config_sromc(1, bw_conf, bc_conf);
}

int board_init(void)
{
	cs8900_pre_init();

	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:  SMDK6410\n");
	return 0;
}
#endif

#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(bd_t *bis)
{
	int i, ret;
#if 0
	/*
	 * Pin 		Function
	 * GPG[0]	SD_0_CLK
	 * GPG[1]	SD_0_CMD
	 * GPG[2:5]	SD_0_DATA[0:3]
	 * GPG[6]	SD_0_CDn
	 */
	for (i = 0; i < 7; i++) {
		/* GPG0[0:6] special function 2 */
		s5p_gpio_cfg_pin(&s3c64xx_gpio->g, i, 0x2);
		/* GPG0[0:6] pull disable */
		s5p_gpio_set_pull(&s3c64xx_gpio->g, i, GPIO_PULL_NONE);
	}
#endif
	ret = s5p_mmc_init(0, 4);
	if (ret)
		error("MMC: Failed to init MMC:0.\n");

	return ret;
}
#endif

#ifdef CONFIG_USB_GADGET
static int s3c64xx_phy_control(int on)
{
	return 0;
}

struct s3c_plat_otg_data s5pc110_otg_data = {
	.phy_control = s3c64xx_phy_control,
	.regs_phy = S3C64XX_PHY_BASE,
	.regs_otg = S3C64XX_OTG_BASE,
	.usb_phy_ctrl = S3C64XX_SYS_BASE + S3C64XX_OTHERS_REG,
};
#endif

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_CS8900
	rc = cs8900_initialize(0, CONFIG_CS8900_BASE);
#endif
	return rc;
}
#endif

#ifdef CONFIG_SPL_BUILD
#include <asm/arch/spl.h>
#include <asm/arch/sys.h>
#include <asm/arch/dmc.h>
#include <asm/arch/gpio.h>

static const struct s3c64xx_clk_cfg clk_config = {
#ifdef CONFIG_SYNC_MODE
	.sync		= true,
#endif
	.clk_div0	= CLK_DIV_VAL,
	.mpll_con	= MPLL_VAL,
	.apll_con	= APLL_VAL,
	.epll_con0	= 0x80200103,
	.epll_con1	= 0,
};

const struct s3c64xx_clk_cfg *board_get_clk_cfg(void)
{
	return &clk_config;
}

/*
 * Memory Configuration
 */
#define DMC1_MEM_CFG	0x00010012	/* burst 4, 13-bit row, 10-bit col */
#define DMC1_MEM_CFG2	0xB45
#define DMC1_CHIP0_CFG	0x150F8		/* 0x5000_0000~0x57ff_ffff (128 MiB) */
#define DMC_DDR_32_CFG	0x0 		/* 32bit, DDR */

/* Timings */
#define DDR_tREFRESH		7800	/* ns */
#define DDR_tRAS		45	/* ns (min: 45ns)*/
#define DDR_tRC 		68	/* ns (min: 67.5ns)*/
#define DDR_tRCD		23	/* ns (min: 22.5ns)*/
#define DDR_tRFC		80	/* ns (min: 80ns)*/
#define DDR_tRP 		23	/* ns (min: 22.5ns)*/
#define DDR_tRRD		15	/* ns (min: 15ns)*/
#define DDR_tWR 		15	/* ns (min: 15ns)*/
#define DDR_tXSR		120	/* ns (min: 120ns)*/
#define DDR_CASL		3	/* CAS Latency 3 */

/* Timings in clock periods */
#define DMC_DDR_CAS_LATENCY	(DDR_CASL << 1)
#define DMC_DDR_t_DQSS		1
#define DMC_DDR_t_MRD		2
#define DMC_DDR_t_RAS		(NS_TO_CLK(DDR_tRAS) + 1)
#define DMC_DDR_t_RC		(NS_TO_CLK(DDR_tRC) + 1)
#define DMC_DDR_t_RCD		(NS_TO_CLK(DDR_tRCD) + 1)
#define DMC_DDR_schedule_RCD	((DMC_DDR_t_RCD - 3) << 3)
#define DMC_DDR_t_RFC		(NS_TO_CLK(DDR_tRFC) + 1)
#define DMC_DDR_schedule_RFC	((DMC_DDR_t_RFC - 3) << 5)
#define DMC_DDR_t_RP		(NS_TO_CLK(DDR_tRP) + 1)
#define DMC_DDR_schedule_RP	((DMC_DDR_t_RP - 3) << 3)
#define DMC_DDR_t_RRD		(NS_TO_CLK(DDR_tRRD) + 1)
#define DMC_DDR_t_WR		(NS_TO_CLK(DDR_tWR) + 1)
#define DMC_DDR_t_WTR		2
#define DMC_DDR_t_XP		2
#define DMC_DDR_t_XSR		(NS_TO_CLK(DDR_tXSR) + 1)
#define DMC_DDR_t_ESR		DMC_DDR_t_XSR
#define DMC_DDR_REFRESH_PRD	(NS_TO_CLK(DDR_tREFRESH))

static const struct s3c64xx_mem_cfg mem_config = {
	.refresh_prd	= DMC_DDR_REFRESH_PRD,
	.cas_latency	= DMC_DDR_CAS_LATENCY,
	.t_dqss		= DMC_DDR_t_DQSS,
	.t_mrd		= DMC_DDR_t_MRD,
	.t_ras		= DMC_DDR_t_RAS,
	.t_rc		= DMC_DDR_t_RC,
	.t_rcd		= DMC_DDR_t_RCD | DMC_DDR_schedule_RCD,
	.t_rfc		= DMC_DDR_t_RFC | DMC_DDR_schedule_RFC,
	.t_rp		= DMC_DDR_t_RP | DMC_DDR_schedule_RP,
	.t_rrd		= DMC_DDR_t_RRD,
	.t_wr		= DMC_DDR_t_WR,
	.t_wtr		= DMC_DDR_t_WTR,
	.t_xp		= DMC_DDR_t_XP,
	.t_xsr		= DMC_DDR_t_XSR,
	.t_esr		= DMC_DDR_t_ESR,
	.mem_cfg	= DMC1_MEM_CFG,
	.mem_cfg2	= DMC1_MEM_CFG2,
	.chip0_cfg	= DMC1_CHIP0_CFG,
	.user_config	= DMC_DDR_32_CFG,
};

const struct s3c64xx_mem_cfg *board_get_mem_cfg(void)
{
	return &mem_config;
}

void board_init_gpio(void)
{
	volatile void *gpio_base = (volatile void *)samsung_get_base_gpio();
	u32 reg;

	writel(0x55540000, gpio_base + GPNCON_REG);
	writel(0x55555555, gpio_base + GPNPUD_REG);
	writel(0x0000f000, gpio_base + GPNDAT_REG);
	writel(0x55555555, gpio_base + MEM1DRVCON_REG);
	writel(0x00220022, gpio_base + GPACON_REG);
}
#endif
