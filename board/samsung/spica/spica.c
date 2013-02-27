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

#include <linux/mtd/mtd.h>
#include <linux/mtd/onenand.h>
#include <linux/mtd/samsung_onenand.h>

#include <onenand_uboot.h>
#include <power/pmic.h>
#include <power/max8698_pmic.h>

#include <lcd.h>

#include <asm/io.h>
#include <asm/arch/s3c6400.h>
#include <asm/arch/clock.h>
#include <asm/arch/pin-cfg.h>
#include <asm/arch/mmc.h>
#include <asm/gpio.h>

#include <linux/input.h>
#include <samsung_keypad.h>

#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Miscellaneous platform dependent initialisations
 */

static const unsigned int dram_config[CONFIG_NR_DRAM_BANKS][2] = {
#ifdef CONFIG_BOARD_DRAM_CONFIG
	CONFIG_BOARD_DRAM_CONFIG
#else
#error Please define CONFIG_BOARD_DRAM_CONFIG
#endif
};

#if defined(CONFIG_SAMSUNG_KEYPAD)
static uint32_t spica_keymap[] = {
	/* KEY(row, col, keycode) */
	KEY(0, 0, KEY_K), KEY(0, 1, KEY_J),
	KEY(1, 0, KEY_K), KEY(1, 1, KEY_J),
	KEY(2, 2, KEY_SPACE),
	KEY(3, 0, KEY_SPACE),
};

static struct matrix_keymap_data spica_keymap_data = {
	.keymap		= spica_keymap,
	.keymap_size	= ARRAY_SIZE(spica_keymap),
};

static struct samsung_keypad_platdata spica_keypad_pdata = {
	.keymap_data	= &spica_keymap_data,
	.rows		= 4,
	.cols		= 4,
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
#if defined(CONFIG_SAMSUNG_KEYPAD)
	samsung_keypad_init(&spica_keypad_pdata);
#endif
	return 0;
}

int power_init_board(void)
{
	int ret;

	ret = pmic_init(I2C_0);
	if (ret)
		return ret;

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

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:\tSpica\n");
	return 0;
}
#endif

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_spica(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xd0000000))
		return addr - 0xc0000000 + 0x50000000;
	else
		printf("do not support this address : %08lx\n", addr);

	return addr;
}
#endif

#ifdef CONFIG_GENERIC_MMC
#define GPIO_TF_DETECT		S3C64XX_GPN(6)

static struct s3c_pin_cfg_entry tf_gpio_table[] = {
	S3C64XX_GPG0_MMC0_CLK, S3C_PIN_PULL(NONE),
	S3C64XX_GPG1_MMC0_CMD, S3C_PIN_PULL(NONE),
	S3C64XX_GPG2_MMC0_DATA0, S3C_PIN_PULL(NONE),
	S3C64XX_GPG3_MMC0_DATA1, S3C_PIN_PULL(NONE),
	S3C64XX_GPG4_MMC0_DATA2, S3C_PIN_PULL(NONE),
	S3C64XX_GPG5_MMC0_DATA3, S3C_PIN_PULL(NONE),
};

int board_mmc_init(bd_t *bis)
{
	struct pmic *p = pmic_get("MAX8698_PMIC");

	if (!p)
		return -ENODEV;

	/* Configure card detect GPIO */
	gpio_direction_input(GPIO_TF_DETECT);
	s3c_gpio_set_pull(GPIO_TF_DETECT, GPIO_PULL_NONE);

	/* Disable TF regulator */
	pmic_set_output(p, MAX8698_REG_ONOFF1, MAX8698_LDO5, 0);
	/* Reconfigure it for 3.3V */
	pmic_reg_write(p, MAX8698_REG_LDO5, 0x11); /* 3.3 V */
	/* Enable TF regulator */
	pmic_set_output(p, MAX8698_REG_ONOFF1, MAX8698_LDO5, 1);
	/* Wait for the regulator to stabilize */
	udelay(100);
	/* Configure GPIO pins */
	s3c_gpio_cfg_bulk(tf_gpio_table, ARRAY_SIZE(tf_gpio_table));
	/* Init mmc host */
	return s5p_mmc_init(0, 4);
}
#endif

#ifdef CONFIG_SAMSUNG_ONENAND
void onenand_board_init(struct mtd_info *mtd)
{
	struct onenand_chip *this = mtd->priv;
	struct samsung_onenand *onenand;
	u32 value;

	this->base = (void *)samsung_get_base_onenand();
	onenand = (struct samsung_onenand *)this->base;

	value = readl(&onenand->int_err_stat);
	writel(value, &onenand->int_err_ack);

	value = readl(&onenand->mem_cfg);
	value &= ~(1 << 8); /* Disable ECC bypass */
	writel(value, &onenand->mem_cfg);

	s3c_onenand_init(mtd);
}
#endif

#ifdef CONFIG_LCD
static void lcd_power_set()
{
}

vidinfo_t panel_info = {
	.vl_freq	= 60,
	.vl_col		= 320,
	.vl_row		= 480,
	.vl_width	= 320,
	.vl_height	= 480,
	.vl_clkp	= 0,
	.vl_hsp		= 0,
	.vl_vsp		= 0,
	.vl_dp		= 1,
	.vl_bpix	= 4,	/* Bits per pixel, 2^5 = 32 */

	.vl_hspw	= 10,
	.vl_hbpd	= 10,
	.vl_hfpd	= 10,

	.vl_vspw	= 2,
	.vl_vbpd	= 3,
	.vl_vfpd	= 8,

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

#ifdef CONFIG_VISUAL_MENU

#include <visual_menu.h>
#include <stdio_dev.h>
#include <iomux.h>

static int menu_read(void *priv)
{
	int c;

	c = getc();

	debug("%s: c = %d\n", __func__, c);

	switch (c) {
	case 'k':
		return ACTION_UP;
	case 'j':
		return ACTION_DOWN;
	case ' ':
		return ACTION_ENTER;
	case 'q':
		return ACTION_EXIT;
	default:
		break;
	}

	return ACTION_NONE;
}

struct spica_menu_item {
	const char *label;
	const char *handler;
};

static const struct spica_menu_item menu_items[] = {
#ifdef CONFIG_SPICA_MENU
	CONFIG_SPICA_MENU
#endif
	{
		.label = "Reboot",
		.handler = "reset",
	}, {
		.label = "Power off",
		.handler = "poweroff",
	}
};

int menu_show(int bootdelay)
{
	struct spica_menu_item *choice;
	struct visual_menu *m;
	int err;
	int i;

	m = visual_menu_create("Spica boot menu:",
					bootdelay, 0, menu_read, NULL);
	if (!m)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(menu_items); ++i) {
		err = visual_menu_item_add(m,
					menu_items[i].label, &menu_items[i]);
		if (err < 0)
			goto err_destroy;
	}

	err = visual_menu_get_choice(m, &choice);

	debug("err = %d, choice = '%s'\n", err, choice->label);

	if (err < 0)
		return err;

	if (choice->handler)
		run_command(choice->handler, 0);

	visual_menu_destroy(m);
	return 0;

err_destroy:
	visual_menu_destroy(m);
	return err;
}
#endif

#endif

#define GPIO_PDA_PS_HOLD	S3C64XX_GPP(13)

static int do_poweroff(cmd_tbl_t *cmdtp, int flag, int argc,
							char * const argv[])
{
	gpio_direction_output(GPIO_PDA_PS_HOLD, 0);
}

U_BOOT_CMD(
	poweroff,	1,	0,	do_poweroff,
	"power off",
	"\n"
	);
