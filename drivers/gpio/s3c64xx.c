/*
 * (C) Copyright 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
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
#include <asm/io.h>
#include <asm/gpio.h>

#define CON_MASK(x)		(0x3 << (2*(x)))
#define CON_SFR(x, v)		((v) << (2*(x)))

#define CON_MASK_4BIT(x)	(0xf << (4*(x)))
#define CON_SFR_4BIT(x, v)	((v) << (4*(x)))

#define DAT_MASK(x)		(0x1 << (x))
#define DAT_SET(x)		(0x1 << (x))

#define PULL_MASK(x)		(0x3 << ((x) << 1))
#define PULL_MODE(x, v)		((v) << ((x) << 1))

static void s3c_gpio_cfg_cfg_pin_2bit(struct s3c_gpio_bank *bank, unsigned offs, unsigned cfg)
{
	unsigned int value;

	value = readl(&bank->con1);
	value &= ~CON_MASK(offs);
	value |= CON_SFR(offs, cfg);
	writel(value, &bank->con1);
}

static void s3c_gpio_cfg_cfg_pin_4bit(struct s3c_gpio_bank *bank, unsigned offs, unsigned cfg)
{
	unsigned int value;

	value = readl(&bank->con1);
	value &= ~CON_MASK_4BIT(offs);
	value |= CON_SFR_4BIT(offs, cfg);
	writel(value, &bank->con1);
}

static void s3c_gpio_cfg_cfg_pin_4bit_2con(struct s3c_gpio_bank *bank, unsigned offs, unsigned cfg)
{
	unsigned int value;

	if (offs > 15)
		value = readl(&bank->con1);
	else
		value = readl(&bank->con0);
	value &= ~CON_MASK_4BIT(offs & 15);
	value |= CON_SFR_4BIT(offs & 15, cfg);
	if (offs > 15)
		writel(value, &bank->con1);
	else
		writel(value, &bank->con0);
}

static void s3c_gpio_cfg_set_value(struct s3c_gpio_bank *bank, unsigned offs, unsigned en)
{
	unsigned int value;

	value = readl(&bank->dat);
	value &= ~DAT_MASK(offs);
	if (en)
		value |= DAT_SET(offs);
	writel(value, &bank->dat);
}

static int s3c_gpio_cfg_get_value(struct s3c_gpio_bank *bank, unsigned offs)
{
	unsigned int value;

	value = readl(&bank->dat);
	return !!(value & DAT_MASK(offs));
}

static void s3c_gpio_cfg_set_pull(struct s3c_gpio_bank *bank, unsigned offs, unsigned mode)
{
	unsigned int value;

	value = readl(&bank->pull);
	value &= ~PULL_MASK(offs);

	switch (mode) {
	case GPIO_PULL_DOWN:
	case GPIO_PULL_UP:
		value |= PULL_MODE(offs, mode);
		break;
	default:
		break;
	}

	writel(value, &bank->pull);
}

static const struct s3c_gpio_cfg s3c_gpio_2bit = {
	.set_config	= s3c_gpio_cfg_cfg_pin_2bit,
	.set_value	= s3c_gpio_cfg_set_value,
	.get_value	= s3c_gpio_cfg_get_value,
	.set_pull	= s3c_gpio_cfg_set_pull,
};

static const struct s3c_gpio_cfg s3c_gpio_4bit = {
	.set_config	= s3c_gpio_cfg_cfg_pin_4bit,
	.set_value	= s3c_gpio_cfg_set_value,
	.get_value	= s3c_gpio_cfg_get_value,
	.set_pull	= s3c_gpio_cfg_set_pull,
};

static const struct s3c_gpio_cfg s3c_gpio_4bit_2con = {
	.set_config	= s3c_gpio_cfg_cfg_pin_4bit_2con,
	.set_value	= s3c_gpio_cfg_set_value,
	.get_value	= s3c_gpio_cfg_get_value,
	.set_pull	= s3c_gpio_cfg_set_pull,
};

static const struct s3c_gpio_chip s3c_gpio_chips[] = {
	{
		.offset	= -4,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x01c,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x03c,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x05c,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x07c,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x09c,
		.cfg	= &s3c_gpio_2bit,
	},{
		.offset	= 0x0bc,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x0e0,
		.cfg	= &s3c_gpio_4bit_2con,
	},{
		.offset	= 0x0fc,
		.cfg	= &s3c_gpio_2bit,
	},{
		.offset	= 0x11c,
		.cfg	= &s3c_gpio_2bit,
	},{
		.offset	= 0x800,
		.cfg	= &s3c_gpio_4bit_2con,
	},{
		.offset	= 0x810,
		.cfg	= &s3c_gpio_4bit_2con,
	},{
		.offset	= 0x81c,
		.cfg	= &s3c_gpio_4bit,
	},{
		.offset	= 0x82c,
		.cfg	= &s3c_gpio_2bit,
	},{
		.offset	= 0x13c,
		.cfg	= &s3c_gpio_2bit,
	},{
		.offset	= 0x15c,
		.cfg	= &s3c_gpio_2bit,
	},{
		.offset	= 0x17c,
		.cfg	= &s3c_gpio_2bit,
	}
};

void s3c_gpio_cfg_pin(unsigned gpio, unsigned config)
{
	unsigned bank = GPIO_BANK(gpio);
	unsigned pin = GPIO_PIN(gpio);
	const struct s3c_gpio_chip *chip = &s3c_gpio_chips[bank];
	const struct s3c_gpio_cfg *cfg = chip->cfg;
	struct s3c_gpio_bank *regs =
		(struct s3c_gpio_bank *)(samsung_get_base_gpio() + chip->offset);
	cfg->set_config(regs, pin, config);
}

static void s3c_gpio_set_value(unsigned gpio, unsigned value)
{
	unsigned bank = GPIO_BANK(gpio);
	unsigned pin = GPIO_PIN(gpio);
	const struct s3c_gpio_chip *chip = &s3c_gpio_chips[bank];
	const struct s3c_gpio_cfg *cfg = chip->cfg;
	struct s3c_gpio_bank *regs =
		(struct s3c_gpio_bank *)(samsung_get_base_gpio() + chip->offset);
	cfg->set_value(regs, pin, value);
}

static int s3c_gpio_get_value(unsigned gpio)
{
	unsigned bank = GPIO_BANK(gpio);
	unsigned pin = GPIO_PIN(gpio);
	const struct s3c_gpio_chip *chip = &s3c_gpio_chips[bank];
	const struct s3c_gpio_cfg *cfg = chip->cfg;
	struct s3c_gpio_bank *regs =
		(struct s3c_gpio_bank *)(samsung_get_base_gpio() + chip->offset);
	return cfg->get_value(regs, pin);
}

void s3c_gpio_set_pull(unsigned gpio, unsigned pull)
{
	unsigned bank = GPIO_BANK(gpio);
	unsigned pin = GPIO_PIN(gpio);
	const struct s3c_gpio_chip *chip = &s3c_gpio_chips[bank];
	const struct s3c_gpio_cfg *cfg = chip->cfg;
	struct s3c_gpio_bank *regs =
		(struct s3c_gpio_bank *)(samsung_get_base_gpio() + chip->offset);
	cfg->set_pull(regs, pin, pull);
}

void s3c_gpio_cfg_bulk(const struct s3c_pin_cfg_entry *cfg, u32 count)
{
	unsigned int gpio = 0;

	if (!count)
		return;

	if (cfg->type != S3C_PIN_ENTRY_NUM)
		BUG();

	while (count--) {
		switch (cfg->type) {
		case S3C_PIN_ENTRY_NUM:
			gpio = cfg->val;
			break;
		case S3C_PIN_ENTRY_CFG:
			s3c_gpio_cfg_pin(gpio, cfg->val);
			break;
		case S3C_PIN_ENTRY_OUT:
			s3c_gpio_set_value(gpio, cfg->val);
			s3c_gpio_cfg_pin(gpio, GPIO_OUTPUT);
			s3c_gpio_set_value(gpio, cfg->val);
			break;
		case S3C_PIN_ENTRY_IN:
			s3c_gpio_cfg_pin(gpio, GPIO_INPUT);
			break;
		case S3C_PIN_ENTRY_PULL:
			s3c_gpio_set_pull(gpio, cfg->val);
			break;
		default:
			BUG();
		}
		++cfg;
	}
}

/* Common GPIO API */

int gpio_request(unsigned gpio, const char *label)
{
	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_direction_input(unsigned gpio)
{
	s3c_gpio_cfg_pin(gpio, GPIO_INPUT);
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	s3c_gpio_set_value(gpio, value);
	s3c_gpio_cfg_pin(gpio, GPIO_OUTPUT);
	s3c_gpio_set_value(gpio, value);
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	return s3c_gpio_get_value(gpio);
}

int gpio_set_value(unsigned gpio, int value)
{
	s3c_gpio_set_value(gpio, value);

	return 0;
}
