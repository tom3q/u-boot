/*
 * S3C64xx DRAM Controller registers.
 *
 * (C) Copyright 2013
 * Lukasz Majewski <l.majewski@majess.pl>
 * Tomasz Figa <tomasz.figa@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __S3C64XX_DMC_H__
#define __S3C64XX_DMC_H__ 

/*
 * Definitions for memory configuration
 * Set memory configuration
 *	active_chips	= 1'b0 (1 chip)
 *	qos_master_chip	= 3'b000(ARID[3:0])
 *	memory burst	= 3'b010(burst 4)
 *	stop_mem_clock	= 1'b0(disable dynamical stop)
 *	auto_power_down	= 1'b0(disable auto power-down mode)
 *	power_down_prd	= 6'b00_0000(0 cycle for auto power-down)
 *	ap_bit		= 1'b0 (bit position of auto-precharge is 10)
 *	row_bits	= 3'b010(# row address 13)
 *	column_bits	= 3'b010(# column address 10 )
 *
 * Set user configuration
 *	2'b10=SDRAM/mSDRAM, 2'b11=DDR, 2'b01=mDDR
 *
 * Set chip select for chip [n]
 *	 row bank control, bank address 0x3000_0000 ~ 0x37ff_ffff
 *	 CHIP_[n]_CFG=0x30F8,  30: ADDR[31:24], F8: Mask[31:24]
 */

/* DRAM controller registers */
#define DMC_MEMC_STATUS_REG	0x00
#define DMC_MEMC_CMD_REG	0x04
#define DMC_DIRECT_CMD_REG	0x08
#define DMC_MEMORY_CFG_REG	0x0c
#define DMC_REFRESH_PRD_REG	0x10
#define DMC_CAS_LATENCY_REG	0x14
#define DMC_T_DQSS_REG		0x18
#define DMC_T_MRD_REG		0x1c
#define DMC_T_RAS_REG		0x20
#define DMC_T_RC_REG		0x24
#define DMC_T_RCD_REG		0x28
#define DMC_T_RFC_REG		0x2c
#define DMC_T_RP_REG		0x30
#define DMC_T_RRD_REG		0x34
#define DMC_T_WR_REG		0x38
#define DMC_T_WTR_REG		0x3c
#define DMC_T_XP_REG		0x40
#define DMC_T_XSR_REG		0x44
#define DMC_T_ESR_REG		0x48
#define DMC_MEMORY_CFG2_REG	0x4c
#define DMC_CHIP_0_CFG_REG	0x200
#define DMC_CHIP_1_CFG_REG	0x204
#define DMC_CHIP_2_CFG_REG	0x208
#define DMC_CHIP_3_CFG_REG	0x20c
#define DMC_USER_STATUS_REG	0x300
#define DMC_USER_CONFIG_REG	0x304

/* DRAM commands */
#define DMC_NOP0		0x0c0000
#define DMC_NOP1		0x1c0000
#define DMC_PA0			0x000000 /* Precharge all */
#define DMC_PA1			0x100000
#define DMC_AR0			0x040000 /* Autorefresh */
#define DMC_AR1			0x140000
#define DMC_SDR_MR0		0x080032 /* MRS, CAS 3,  Burst Length 4 */
#define DMC_SDR_MR1		0x180032
#define DMC_DDR_MR0		0x080162
#define DMC_DDR_MR1		0x180162
#define DMC_mDDR_MR0		0x080032 /* CAS 3, Burst Length 4 */
#define DMC_mDDR_MR1		0x180032
#define DMC_mSDR_EMR0		0x0a0000 /* EMRS, DS:Full, PASR:Full Array */
#define DMC_mSDR_EMR1		0x1a0000
#define DMC_DDR_EMR0		0x090000
#define DMC_DDR_EMR1		0x190000
#define DMC_mDDR_EMR0		0x0a0000 /*  DS:Full, PASR:Full Array */
#define DMC_mDDR_EMR1		0x1a0000

/* mDDR memory configuration */
#define NS_TO_CLK(t)		((STARTUP_HCLK / 1000 * (t) - 1) / 1000000)

#endif /* __S3C64XX_DMC_H__ */
