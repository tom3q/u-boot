/*
 *  Copyright (C) 2011 Samsung Electronics
 *  Lukasz Majewski <l.majewski@samsung.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __MAX8698_PMIC_H_
#define __MAX8698_PMIC_H_

/* MAX 8998 registers */
enum {
	MAX8698_REG_ONOFF1 = 0,
	MAX8698_REG_ONOFF2,
	MAX8698_REG_ADISCHG_EN1,
	MAX8698_REG_ADISCHG_EN2,
	MAX8698_REG_DVSARM12,
	MAX8698_REG_DVSARM34,
	MAX8698_REG_DVSINT12,
	MAX8698_REG_BUCK3,
	MAX8698_REG_LDO23,
	MAX8698_REG_LDO4,
	MAX8698_REG_LDO5,
	MAX8698_REG_LDO6,
	MAX8698_REG_LDO7,
	MAX8698_REG_LDO8_BKCHR,
	MAX8698_REG_LDO9,
	MAX8698_REG_LBCNFG,
	PMIC_NUM_OF_REGS,
};

/* MAX8698_REG_ONOFF1 */
#define MAX8698_LDO2		(1 << 4)
#define MAX8698_LDO3		(1 << 3)
#define MAX8698_LDO4		(1 << 2)
#define MAX8698_LDO5		(1 << 1)

/* MAX8698_REG_ONOFF2 */
#define MAX8698_LDO6		(1 << 7)
#define MAX8698_LDO7		(1 << 6)
#define MAX8698_LDO8		(1 << 5)
#define MAX8698_LDO9		(1 << 4)

#define MAX8698_I2C_ADDR        (0x66)

#endif /* __MAX8698_PMIC_H_ */
