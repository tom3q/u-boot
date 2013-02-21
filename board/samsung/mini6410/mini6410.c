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

#include <lcd.h>

#include <asm/io.h>
#include <asm/arch/s3c6400.h>
#include <asm/arch/clock.h>
#include <asm/arch/pin-cfg.h>
#include <asm/arch/mmc.h>
#include <asm/gpio.h>

#include <linux/input.h>
#include <gpio_keys.h>

#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscellaneous platform dependent initialisations
 */

static const unsigned int dram_config[CONFIG_NR_DRAM_BANKS][2] = {
	BOARD_DRAM_CONFIG
};

#ifdef CONFIG_GPIO_KEYS
static const struct gpio_key spica_gpio_keys[] = {
	{
		.gpio = S3C64XX_GPN(0),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_1,
	}, {
		.gpio = S3C64XX_GPN(1),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_2,
	}, {
		.gpio = S3C64XX_GPN(2),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_3,
	}, {
		.gpio = S3C64XX_GPN(3),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_4,
	}, {
		.gpio = S3C64XX_GPN(4),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_5,
	}, {
		.gpio = S3C64XX_GPN(5),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_6,
	}, {
		.gpio = S3C64XX_GPL(11),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_7,
	}, {
		.gpio = S3C64XX_GPL(12),
		.polarity = 1,
		.debounce_ms = 20,
		.scancode = KEY_8,
	}
};
#endif

int board_init(void)
{
	gd->bd->bi_arch_number = CONFIG_MACH_TYPE;
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	return 0;
}

int drv_keyboard_init(void)
{
#if defined(CONFIG_GPIO_KEYS)
	gpio_keys_init(spica_gpio_keys, ARRAY_SIZE(spica_gpio_keys));
#endif
	return 0;
}

void dram_init_banksize(void)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; ++i) {
		gd->bd->bi_dram[i].start = dram_config[i][0];
		gd->bd->bi_dram[i].size = dram_config[i][1];
	}
}

int dram_init(void)
{
	int i;

	gd->ram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; ++i)
		gd->ram_size += dram_config[i][1];

	return 0;
}

#ifdef CONFIG_DRIVER_DM9000
#define DM9000_Tacs	0	/* address set-up */
#define DM9000_Tcos	4	/* chip selection set-up */
#define DM9000_Tacc	14	/* access cycle */
#define DM9000_Tcoh	1	/* chip selection hold */
#define DM9000_Tah	4	/* address holding time */
#define DM9000_Tacp	6	/* page mode access cycle */
#define DM9000_PMC	0x0	/* normal (1data) page mode configuration */

int board_eth_init(bd_t *bis)
{
	SROM_BW_REG &= ~(0xf << 4);
	SROM_BW_REG |= (1 << 7) | (1 << 6) | (1 << 4);
	SROM_BC1_REG = (DM9000_Tacs << 28) | (DM9000_Tcos << 24)
		| (DM9000_Tacc << 16) | (DM9000_Tcoh << 12)
		| (DM9000_Tah << 8) | (DM9000_Tacp << 4) | DM9000_PMC;
	return dm9000_initialize(bis);
}
#endif

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:\tMini6410\n");
	return 0;
}
#endif

#ifdef CONFIG_GENERIC_MMC
static struct s3c_pin_cfg_entry mmc0_gpio_table[] = {
	S3C64XX_GPG0_MMC0_CLK, S3C_PIN_PULL(NONE),
	S3C64XX_GPG1_MMC0_CMD, S3C_PIN_PULL(NONE),
	S3C64XX_GPG2_MMC0_DATA0, S3C_PIN_PULL(NONE),
	S3C64XX_GPG3_MMC0_DATA1, S3C_PIN_PULL(NONE),
	S3C64XX_GPG4_MMC0_DATA2, S3C_PIN_PULL(NONE),
	S3C64XX_GPG5_MMC0_DATA3, S3C_PIN_PULL(NONE),
	S3C64XX_GPG6_MMC0_CDN, S3C_PIN_PULL(NONE),
};

int board_mmc_init(bd_t *bis)
{
	/* Configure GPIO pins */
	s3c_gpio_cfg_bulk(mmc0_gpio_table, ARRAY_SIZE(mmc0_gpio_table));
	/* Init mmc host */
	return s5p_mmc_init(0, 4);
}
#endif

#ifdef CONFIG_LCD
static void lcd_power_set()
{
	gpio_direction_output(S3C64XX_GPE(0), 1);
}

vidinfo_t panel_info = {
	.vl_freq	= 60,
	.vl_col		= 480,
	.vl_row		= 272,
	.vl_width	= 480,
	.vl_height	= 272,
	.vl_clkp	= 1,
	.vl_hsp		= 0,
	.vl_vsp		= 0,
	.vl_dp		= 1,
	.vl_bpix	= 4,	/* Bits per pixel, 2^5 = 32 */

	.vl_hspw	= 40,
	.vl_hbpd	= 3,
	.vl_hfpd	= 2,

	.vl_vspw	= 1,
	.vl_vbpd	= 1,
	.vl_vfpd	= 1,

	.win_id		= 0,
	.cfg_gpio	= NULL,
	.backlight_on	= NULL,
	.lcd_power_on	= lcd_power_set,
	.reset_lcd	= NULL,
	.dual_lcd_enabled = 0,

	.init_delay	= 0,
	.power_on_delay = 0,
	.reset_delay	= 0,
	.interface_mode = FIMD_RGB_INTERFACE,
};

void init_panel_info(vidinfo_t *vid)
{
	vid->rgb_mode	= MODE_RGB_P;
}

#ifdef CONFIG_LCD_INFO
#include <nand.h>
#include <version.h>

void lcd_show_board_info(void)
{
	ulong dram_size;
	int i;
	char temp[32];

	lcd_printf ("%s\n", U_BOOT_VERSION);
	lcd_printf ("CPU at %s MHz\n", strmhz(temp, get_ARMCLK()));

	dram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		dram_size += gd->bd->bi_dram[i].size;
	lcd_printf ("  %ld MB SDRAM\n", dram_size >> 20);
}
#endif /* CONFIG_LCD_INFO */

#endif
