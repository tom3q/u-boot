/*
 * Copyright (C) 2013 Tomasz Figa <tomasz.figa@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include<common.h>
#include<config.h>

#include <spl.h>
#include <asm/arch/clk.h>
#include <asm/arch/spl.h>
#include <asm/arch/cpu.h>
#include <asm/arch/mmc_boot.h>
#include <asm/arch/vic.h>
#include <asm/arch/dmc.h>
#include <asm/arch/gpio.h>
#include <u-boot/crc.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * IROM image copy functions support
 */
enum index {
	NAND_512_INDEX,
	NAND_2K_4K_INDEX,
	MMC_INDEX,
	ONENAND_INDEX,
	ONENAND_4BURST_INDEX,
	ONENAND_8BURST_INDEX,
};

enum boot_mode {
	BOOT_MODE_MMC0,
	BOOT_MODE_ONENAND,
	BOOT_MODE_NAND_512_3C,
	BOOT_MODE_NAND_512_4C,
	BOOT_MODE_NAND_2K_4C,
	BOOT_MODE_NAND_2K_5C,
	BOOT_MODE_NAND_4K_5C,
	BOOT_MODE_MMC1,
};

static void *get_irom_func(int index)
{
	return (void *)*(u32 *)(S3C64XX_TCM1_BASE + 4 * index);
}

/*
 * MMC boot
 */
static void copy_uboot_from_mmc(void)
{
	unsigned char (*copy_func)(int unused, unsigned int start_block,
				   unsigned short block_count, void *buf,
				   unsigned char reinit);

	copy_func = get_irom_func(MMC_INDEX);

	copy_func(0, MMC_BL2_BLKPOS, MMC_BL2_BLKCNT,
			(void *)CONFIG_SYS_TEXT_BASE, 1);
}

/*
 * SPL boot device selection
 *
 * IROM functions are used here to load BL2 image into memory, so it can
 * be then executed by SPL core in BOOT_DEVICE_RAM mode.
 */
u32 spl_boot_device(void)
{
	enum boot_mode bootmode = BOOT_MODE_MMC0;
	u32 crc;
	int i;

	switch (bootmode) {
	case BOOT_MODE_MMC0:
		copy_uboot_from_mmc();
		break;
	default:
		break;
	}

	/* HACK: Make sure that no header is present before the image */
	memset((void *)CONFIG_SYS_TEXT_BASE - sizeof(struct image_header),
		sizeof(struct image_header), 0);

	crc = crc32(0, (void *)CONFIG_SYS_TEXT_BASE, CONFIG_BL2_SIZE);

	puts("Loaded BL2 image (crc32 = 0x");
	for (i = 28; i >= 0; i -= 4) {
		char c = (crc >> i) & 0xf;

		if (c < 0xa)
			putc(c + '0');
		else
			putc(c - 0xa + 'a');
	}
	puts(")\n");

	return BOOT_DEVICE_RAM;
}

/*
 * Low level SoC initialization
 */

static void watchdog_disable(void)
{
	volatile void *base = (volatile void *)S3C64XX_WATCHDOG_BASE;

	/* Disable Watchdog */
	writel(0x0, base + 0x00);
}

static void irq_clear(void)
{
	volatile void *gpio = (volatile void *)S3C64XX_GPIO_BASE;
	volatile void *vic0 = (volatile void *)S3C64XX_VIC0_BASE;
	volatile void *vic1 = (volatile void *)S3C64XX_VIC1_BASE;

	/* External interrupt pending clear */
	writel(readl(gpio + EINTPEND_REG), gpio + EINTPEND_REG);

	/* Disable all interrupts (VIC0 and VIC1) */
	writel(0x0, vic0 + OINTMSK_REG);
	writel(0x0, vic1 + OINTMSK_REG);

	/* Set all interrupts as IRQ */
	writel(0x0, vic0 + OINTMOD_REG);
	writel(0x0, vic1 + OINTMOD_REG);

	/* Pending Interrupt Clear */
	writel(0x0, vic0 + OVECTADDR_REG);
	writel(0x0, vic1 + OVECTADDR_REG);
}

#define PLL_LOCK_US	300UL
#define USEC_PER_SEC	1000000UL
#define PLL_LOCK_TIME	((CONFIG_SYS_CLK_FREQ * PLL_LOCK_US) / USEC_PER_SEC)

static void clock_init(void)
{
	const struct s3c64xx_clk_cfg *config = board_get_clk_cfg();
	volatile void *sys_base = (volatile void *)S3C64XX_SYS_BASE;
	u32 reg, stat;
	int i;

	reg = readl(sys_base + OTHERS_REG);
	if (config->sync) {
		/* Switch SYNCMUX to APLL */
		reg |= (1 << 6);
		writel(reg, sys_base + OTHERS_REG);
	}

	/* Wait for the mux to switch */
	nop();
	nop();
	nop();
	nop();
	nop();

	/* Configure ARM core sync setting */
	if (config->sync)
		reg |= (1 << 7);
	else
		reg &= ~(1 << 7);
	writel(reg, sys_base + OTHERS_REG);

	/* Wait for ARM core to confirm sync setting */
	stat = config->sync ? 0xf00 : 0x000;
	do {
		reg = readl(sys_base + OTHERS_REG) & 0xf00;
	} while (reg != stat);

	if (!config->sync) {
		/* Switch SYNCMUX to MPLL */
		reg = readl(sys_base + OTHERS_REG);
		reg &= ~(1 << 6);
		writel(reg, sys_base + OTHERS_REG);
	}

	/* Set clock dividers */
	reg = readl(sys_base + CLK_DIV0_REG);
	reg &= ~0x3ffff;
	reg |= config->clk_div0 & 0x3ffff;
	writel(reg, sys_base + CLK_DIV0_REG);

	/* Set PLLs */
	writel(PLL_LOCK_TIME + 1, sys_base + APLL_LOCK_REG);
	writel(PLL_LOCK_TIME + 1, sys_base + MPLL_LOCK_REG);
	writel(PLL_LOCK_TIME + 1, sys_base + EPLL_LOCK_REG);
	writel(config->apll_con, sys_base + APLL_CON_REG);
	writel(config->mpll_con, sys_base + MPLL_CON_REG);
	writel(config->epll_con0, sys_base + EPLL_CON0_REG);
	writel(config->epll_con1, sys_base + EPLL_CON1_REG);

	/* Wait for PLLs to lock */
	for (i = 0; i < 0x10000; ++i)
		nop();

	/* Switch PLL muxes to PLL outputs */
	reg = readl(sys_base + CLK_SRC_REG);
	reg |= 0x7;
	writel(reg, sys_base + CLK_SRC_REG);

	/* Configure VIC synchronization setting */
	reg = readl(sys_base + OTHERS_REG);
	if (config->sync)
		reg |= (1 << 5);
	else
		reg &= ~(1 << 5);
	writel(reg, sys_base + OTHERS_REG);

	writel(MEM_SYS_CFG_NAND, sys_base + MEM_SYS_CFG_REG);
}

static void mem_init(void)
{
	const struct s3c64xx_mem_cfg *config = board_get_mem_cfg();
	volatile void *dmc1_base = (volatile void *)S3C64XX_DMC1_BASE;
	u32 reg;

	/* Enter configuration mode */
	writel(0x4, dmc1_base + DMC_MEMC_CMD_REG);

	/* Set memory parameters */
	writel(config->refresh_prd, dmc1_base + DMC_REFRESH_PRD_REG);
	writel(config->cas_latency, dmc1_base + DMC_CAS_LATENCY_REG);
	writel(config->t_dqss, dmc1_base + DMC_T_DQSS_REG);
	writel(config->t_mrd, dmc1_base + DMC_T_MRD_REG);
	writel(config->t_ras, dmc1_base + DMC_T_RAS_REG);
	writel(config->t_rc, dmc1_base + DMC_T_RC_REG);
	writel(config->t_rcd, dmc1_base + DMC_T_RCD_REG);
	writel(config->t_rfc, dmc1_base + DMC_T_RFC_REG);
	writel(config->t_rp, dmc1_base + DMC_T_RP_REG);
	writel(config->t_rrd, dmc1_base + DMC_T_RRD_REG);
	writel(config->t_wr, dmc1_base + DMC_T_WR_REG);
	writel(config->t_wtr, dmc1_base + DMC_T_WTR_REG);
	writel(config->t_xp, dmc1_base + DMC_T_XP_REG);
	writel(config->t_xsr, dmc1_base + DMC_T_XSR_REG);
	writel(config->t_esr, dmc1_base + DMC_T_ESR_REG);
	writel(config->mem_cfg, dmc1_base + DMC_MEMORY_CFG_REG);
	writel(config->mem_cfg2, dmc1_base + DMC_MEMORY_CFG2_REG);
	writel(config->chip0_cfg, dmc1_base + DMC_CHIP_0_CFG_REG);
	writel(config->user_config, dmc1_base + DMC_USER_CONFIG_REG);

	/* Send initialization commands */
	writel(DMC_NOP0, dmc1_base + DMC_DIRECT_CMD_REG);
	writel(DMC_PA0, dmc1_base + DMC_DIRECT_CMD_REG);
	writel(DMC_AR0, dmc1_base + DMC_DIRECT_CMD_REG);
	writel(DMC_AR0, dmc1_base + DMC_DIRECT_CMD_REG);
	writel(DMC_mDDR_EMR0, dmc1_base + DMC_DIRECT_CMD_REG);
	writel(DMC_mDDR_MR0, dmc1_base + DMC_DIRECT_CMD_REG);

	/* Enable DMC1 */
	writel(0x0, dmc1_base + DMC_MEMC_CMD_REG);

	/* Wait until DMC1 gets ready */
	do {
		reg = readl(dmc1_base + DMC_MEMC_STATUS_REG) & 0x3;
	} while (reg != 0x1);

	nop();
}

static void check_wakeup(void)
{
	volatile void *sys_base = (volatile void *)S3C64XX_SYS_BASE;
	void (*resume_vector)(void);
	u32 reg;

	/* Check for wakeup reset */
	reg = readl(sys_base + RST_STAT_REG);
	if (!(reg & 0x8))
		return;

	/* Clear wakeup status register */
	writel(readl(sys_base + WAKEUP_STAT_REG), sys_base + WAKEUP_STAT_REG);

	/* Return address has been saved for us in INFORM0 register */
	resume_vector = (void *)readl(sys_base + INF_REG0_REG);

	/* Jump to resume vector */
	resume_vector();

	/* Unreachable... */
}

void board_init_f(ulong dummy)
{
	/* Set the stack pointer. */
	asm volatile("mov sp, %0\n" : : "r"(CONFIG_SPL_STACK));

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	/* Set global data pointer. */
	gd = &gdata;

	/* Initialize basic hardware */
	board_init_gpio();
	watchdog_disable();
	irq_clear();
	clock_init();
	mem_init();
	check_wakeup();

	/* Init uart */
	preloader_console_init();

	/* Jump to main SPL code */
	board_init_r(NULL, 0);
}
