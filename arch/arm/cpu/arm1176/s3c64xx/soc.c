/*
 * Copyright (c) 2010 Samsung Electronics.
 * Minkyu Kang <mk7.kang@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/arch/cpu.h>
#include <asm/arch/mmc_boot.h>

void reset_cpu(ulong addr)
{
	volatile void *sys_base = (volatile void *)S3C64XX_SYS_BASE;

	writel(get_device_type(), sys_base + SW_RST_REG);

	while(1);
}

int arch_cpu_init(void)
{
	samsung_set_cpu_id();
#ifndef CONFIG_SYS_ICACHE_OFF
	icache_enable();
#endif
#ifndef CONFIG_SYS_DCACHE_OFF
	dcache_enable();
#endif
	return 0;
}

#ifdef CONFIG_ENV_IS_IN_MMC
int mmc_get_env_addr(struct mmc *mmc, int copy, u32 *env_addr)
{
	s64 offset;

	*env_addr = MMC_ENV_BLKPOS * MMC_BLKSIZE;

	return 0;
}
#endif

/* OHCI HCD (Host Controller Driver) initialization */
#ifdef CONFIG_USB_OHCI_NEW
int usb_cpu_init(void)
{
	volatile void *sys_base = (volatile void *)S3C64XX_SYS_BASE;
	u32 reg;

	reg = readl(sys_base + OTHERS_REG);
	reg |= 0x10000;
	writel(reg, sys_base + OTHERS_REG);

	return 0;
}

int usb_cpu_stop(void)
{
	volatile void *sys_base = (volatile void *)S3C64XX_SYS_BASE;
	u32 reg;

	reg = readl(sys_base + OTHERS_REG);
	reg &= ~0x10000;
	writel(reg, sys_base + OTHERS_REG);

	return 0;
}

void usb_cpu_init_fail(void)
{
	usb_cpu_stop();
}
#endif
