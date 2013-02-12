/*
 * (C) Copyright 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Heungjun Kim <riverful.kim@samsung.com>
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
 *
 */

#ifndef _S3C64XX_CPU_H
#define _S3C64XX_CPU_H

#include <asm/arch/s3c6400.h>

#define ELFIN_MMC_BASE		(0x7c200000)
#define ELFIN_PRO_ID		(0x7e00e000)
#define ELFIN_ONENAND_BASE	(0x70100000)
#define ELFIN_KEYPAD_BASE	(0x7e00a000)

#ifndef __ASSEMBLY__
#define SAMSUNG_BASE(device, base)				\
static inline unsigned int samsung_get_base_##device(void)	\
{								\
		return ELFIN_##base;				\
}

SAMSUNG_BASE(clock, CLOCK_POWER_BASE)
SAMSUNG_BASE(gpio, GPIO_BASE)
SAMSUNG_BASE(pro_id, PRO_ID)
SAMSUNG_BASE(mmc, MMC_BASE)
SAMSUNG_BASE(sromc, SROM_BASE)
SAMSUNG_BASE(timer, TIMER_BASE)
SAMSUNG_BASE(uart, UART_BASE)
SAMSUNG_BASE(watchdog, WATCHDOG_BASE)
SAMSUNG_BASE(onenand, ONENAND_BASE)
SAMSUNG_BASE(keypad, KEYPAD_BASE)
#endif

#endif	/* _S3C64XX_CPU_H */
