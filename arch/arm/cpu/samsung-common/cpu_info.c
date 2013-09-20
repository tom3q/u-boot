/*
 * Copyright (C) 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/arch/cpu.h>
#include <asm/io.h>
#include <asm/arch/clk.h>

/* Default is s5pc100 */
unsigned int samsung_cpu_id = 0xC100;
/* Default is EVT1 */
unsigned int samsung_cpu_rev = 1;

#ifdef CONFIG_ARCH_CPU_INIT
int arch_cpu_init(void)
{
	samsung_set_cpu_id();

	return 0;
}
#endif

u32 get_device_type(void)
{
	return samsung_cpu_id;
}

#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void)
{
	char buf[32];

	printf("CPU:\t%s%X@%sMHz\n", samsung_get_cpu_name(), samsung_cpu_id,
	       strmhz(buf, get_arm_clk()));

	return 0;
}
#endif
