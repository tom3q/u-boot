/*
 * (C) Copyright 2013 Tomasz Figa <tomasz.figa@gmail.com>
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

#include <common.h>
#include <asm/hardware.h>

#define S3C64XX_SPCON		__REG(0x7F0081A0)
#define S3C64XX_MODEM_MIFPCON	__REG(0x7410800C)

void set_system_display_ctrl(void)
{
	unsigned int tmp;

	/* set the LCD type */
	tmp = S3C64XX_SPCON;
	tmp &= ~0x3;
	tmp |= 0x1;
	S3C64XX_SPCON = tmp;

	/* remove the LCD bypass */
	tmp = S3C64XX_MODEM_MIFPCON;
	tmp &= ~0x8;
	S3C64XX_MODEM_MIFPCON = tmp;
}
