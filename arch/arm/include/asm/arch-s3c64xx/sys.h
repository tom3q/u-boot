/*
 * S3C64xx System Controller registers.
 *
 * (C) Copyright 2013
 * Lukasz Majewski <l.majewski@majess.pl>
 * Tomasz Figa <tomasz.figa@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __S3C64XX_SYS_H__
#define __S3C64XX_SYS_H__

#define APLL_LOCK_REG		0x00
#define MPLL_LOCK_REG		0x04
#define EPLL_LOCK_REG		0x08
#define APLL_CON_REG		0x0C
#define MPLL_CON_REG		0x10
#define EPLL_CON0_REG		0x14
#define EPLL_CON1_REG		0x18
#define CLK_SRC_REG		0x1C
#define CLK_DIV0_REG		0x20
#define CLK_DIV1_REG		0x24
#define CLK_DIV2_REG		0x28
#define CLK_OUT_REG		0x2C
#define HCLK_GATE_REG		0x30
#define PCLK_GATE_REG		0x34
#define SCLK_GATE_REG		0x38
#define AHB_CON0_REG		0x100
#define AHB_CON1_REG		0x104
#define AHB_CON2_REG		0x108
#define SELECT_DMA_REG		0x110
#define SW_RST_REG		0x114
#define SYS_ID_REG		0x118
#define MEM_SYS_CFG_REG		0x120
#define QOS_OVERRIDE0_REG	0x124
#define QOS_OVERRIDE1_REG	0x128
#define MEM_CFG_STAT_REG	0x12C
#define PWR_CFG_REG		0x804
#define EINT_MASK_REG		0x808
#define NOR_CFG_REG		0x810
#define STOP_CFG_REG		0x814
#define SLEEP_CFG_REG		0x818
#define OSC_FREQ_REG		0x820
#define OSC_STABLE_REG		0x824
#define PWR_STABLE_REG		0x828
#define FPC_STABLE_REG		0x82C
#define MTC_STABLE_REG		0x830
#define OTHERS_REG		0x900
#define RST_STAT_REG		0x904
#define WAKEUP_STAT_REG		0x908
#define BLK_PWR_STAT_REG	0x90C
#define INF_REG0_REG		0xA00
#define INF_REG1_REG		0xA04
#define INF_REG2_REG		0xA08
#define INF_REG3_REG		0xA0C
#define INF_REG4_REG		0xA10
#define INF_REG5_REG		0xA14
#define INF_REG6_REG		0xA18
#define INF_REG7_REG		0xA1C

/* Syscon register bitfields */
#define MEM_SYS_CFG_16BIT	(1 << 12)
#define MEM_SYS_CFG_NAND	(1 << 3)

/* Initial clock settings */
#if defined(CONFIG_CLK_400_100_50)
#define STARTUP_AMDIV		400
#define STARTUP_MDIV		400
#define STARTUP_PDIV		6
#define STARTUP_SDIV		1
#elif defined(CONFIG_CLK_400_133_66)
#define STARTUP_AMDIV		400
#define STARTUP_MDIV		533
#define STARTUP_PDIV		6
#define STARTUP_SDIV		1
#elif defined(CONFIG_CLK_533_133_66)
#define STARTUP_AMDIV		533
#define STARTUP_MDIV		533
#define STARTUP_PDIV		6
#define STARTUP_SDIV		1
#elif defined(CONFIG_CLK_532_133_66)
#define STARTUP_AMDIV		266
#define STARTUP_MDIV		266
#define STARTUP_PDIV		3
#define STARTUP_SDIV		1
#elif defined(CONFIG_CLK_667_133_66)
#define STARTUP_AMDIV		667
#define STARTUP_MDIV		533
#define STARTUP_PDIV		6
#define STARTUP_SDIV		1
#endif

#define	STARTUP_PCLKDIV		3
#define STARTUP_HCLKX2DIV	1
#define STARTUP_HCLKDIV		1
#define STARTUP_MPLLDIV		1
#define STARTUP_APLLDIV		0

#define CLK_DIV_VAL	((STARTUP_PCLKDIV << 12) | (STARTUP_HCLKX2DIV << 9) | \
	(STARTUP_HCLKDIV << 8) | (STARTUP_MPLLDIV<<4) | STARTUP_APLLDIV)
#define MPLL_VAL	((1 << 31) | (STARTUP_MDIV << 16) | \
	(STARTUP_PDIV << 8) | STARTUP_SDIV)
#define STARTUP_MPLL	(((CONFIG_SYS_CLK_FREQ >> STARTUP_SDIV) / \
	STARTUP_PDIV) * STARTUP_MDIV)

#if defined(CONFIG_SYNC_MODE)
#define APLL_VAL	((1 << 31) | (STARTUP_MDIV << 16) | \
	(STARTUP_PDIV << 8) | STARTUP_SDIV)
#define STARTUP_APLL	(((CONFIG_SYS_CLK_FREQ >> STARTUP_SDIV) / \
	STARTUP_PDIV) * STARTUP_MDIV)
#define STARTUP_HCLK	(STARTUP_MPLL / (STARTUP_HCLKX2DIV + 1) / \
	(STARTUP_HCLKDIV + 1))
#else
#define APLL_VAL	((1 << 31) | (STARTUP_AMDIV << 16) | \
	(STARTUP_PDIV << 8) | STARTUP_SDIV)
#define STARTUP_APLL	(((CONFIG_SYS_CLK_FREQ >> STARTUP_SDIV) / \
	STARTUP_PDIV) * STARTUP_AMDIV)
#define STARTUP_HCLK	(STARTUP_MPLL / (STARTUP_HCLKX2DIV + 1) / \
	(STARTUP_HCLKDIV + 1))
#endif

#endif /* __S3C64XX_SYS_H__ */
