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

#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H

#ifndef __ASSEMBLY__
struct s3c_gpio_bank {
	unsigned int	con0;
	unsigned int	con1;
	unsigned int	dat;
	unsigned int	pull;
	unsigned int	pdn_con;
	unsigned int	pdn_pull;
};

enum {
	S3C_GPA = 0,
	S3C_GPB,
	S3C_GPC,
	S3C_GPD,
	S3C_GPE,
	S3C_GPF,
	S3C_GPG,
	S3C_GPH,
	S3C_GPI,
	S3C_GPJ,
	S3C_GPK,
	S3C_GPL,
	S3C_GPM,
	S3C_GPN,
	S3C_GPO,
	S3C_GPP,
	S3C_GPQ
};

#define GPIO_NUM(bank, pin)	(((bank) << 8) | (pin))

/* S3C64XX GPIO number definitions. */
#define S3C64XX_GPA(_nr)	GPIO_NUM(S3C_GPA, (_nr))
#define S3C64XX_GPB(_nr)	GPIO_NUM(S3C_GPB, (_nr))
#define S3C64XX_GPC(_nr)	GPIO_NUM(S3C_GPC, (_nr))
#define S3C64XX_GPD(_nr)	GPIO_NUM(S3C_GPD, (_nr))
#define S3C64XX_GPE(_nr)	GPIO_NUM(S3C_GPE, (_nr))
#define S3C64XX_GPF(_nr)	GPIO_NUM(S3C_GPF, (_nr))
#define S3C64XX_GPG(_nr)	GPIO_NUM(S3C_GPG, (_nr))
#define S3C64XX_GPH(_nr)	GPIO_NUM(S3C_GPH, (_nr))
#define S3C64XX_GPI(_nr)	GPIO_NUM(S3C_GPI, (_nr))
#define S3C64XX_GPJ(_nr)	GPIO_NUM(S3C_GPJ, (_nr))
#define S3C64XX_GPK(_nr)	GPIO_NUM(S3C_GPK, (_nr))
#define S3C64XX_GPL(_nr)	GPIO_NUM(S3C_GPL, (_nr))
#define S3C64XX_GPM(_nr)	GPIO_NUM(S3C_GPM, (_nr))
#define S3C64XX_GPN(_nr)	GPIO_NUM(S3C_GPN, (_nr))
#define S3C64XX_GPO(_nr)	GPIO_NUM(S3C_GPO, (_nr))
#define S3C64XX_GPP(_nr)	GPIO_NUM(S3C_GPP, (_nr))
#define S3C64XX_GPQ(_nr)	GPIO_NUM(S3C_GPQ, (_nr))

/* functions */
void s3c_gpio_cfg_pin(unsigned gpio, unsigned cfg);
void s3c_gpio_set_pull(unsigned gpio, unsigned mode);

/* Pin config interface */

enum s3c_pin_cfg_entry_type {
	S3C_PIN_ENTRY_NUM = 0,
	S3C_PIN_ENTRY_CFG,
	S3C_PIN_ENTRY_IN,
	S3C_PIN_ENTRY_OUT,
	S3C_PIN_ENTRY_PULL
};

struct s3c_pin_cfg_entry {
	enum s3c_pin_cfg_entry_type	type;
	unsigned int			val;
};

void s3c_gpio_cfg_bulk(const struct s3c_pin_cfg_entry *cfg, u32 count);

#define GPIO_PIN(gpio)		((gpio) & 0xff)
#define GPIO_BANK(gpio)		((gpio) >> 8)

/* For use with s3c_pin_config() */
#define S3C_PIN(num)		{ S3C_PIN_ENTRY_NUM, (num) }
#define S3C_PIN_CFG(cfg)	{ S3C_PIN_ENTRY_CFG, (cfg) }
#define S3C_PIN_IN		{ S3C_PIN_ENTRY_IN, 0 }
#define S3C_PIN_OUT(val)	{ S3C_PIN_ENTRY_OUT, val }
#define S3C_PIN_PULL(pull)	{ S3C_PIN_ENTRY_PULL, GPIO_PULL_ ## pull }

/* Convenience macro for pin-specific special functions */
#define S3C_PIN_NUM_CFG(num, cfg)	S3C_PIN(num), S3C_PIN_CFG(cfg)

/* Pin configurations */
#define GPIO_INPUT	0x0
#define GPIO_OUTPUT	0x1
#define GPIO_FUNC(x)	(x)

/* Pull mode */
#define GPIO_PULL_NONE	0x0
#define GPIO_PULL_DOWN	0x1
#define GPIO_PULL_UP	0x2

struct s3c_gpio_cfg {
	void (*set_config)(struct s3c_gpio_bank *bank, unsigned offs, unsigned config);
	void (*set_value)(struct s3c_gpio_bank *bank, unsigned offs, unsigned value);
	int (*get_value)(struct s3c_gpio_bank *bank, unsigned offs);
	void (*set_pull)(struct s3c_gpio_bank *bank, unsigned offs, unsigned pull);
};

struct s3c_gpio_chip {
	int offset;
	const struct s3c_gpio_cfg *cfg;
};

#endif /* __ASSEMBLY__ */

#endif /* __ASM_ARCH_GPIO_H */
