/*
 * S3C64xx Vectored Interrupt Controller registers.
 *
 * (C) Copyright 2013
 * Lukasz Majewski <l.majewski@majess.pl>
 * Tomasz Figa <tomasz.figa@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __S3C64XX_VIC_H__
#define __S3C64XX_VIC_H__

#define OINTMOD_REG		0x00c /* VIC INT SELECT (IRQ or FIQ) */
#define OINTUNMSK_REG		0x010 /* VIC INT EN (write 1 to unmask) */
#define OINTMSK_REG		0x014 /* VIC INT EN CLEAR (write 1 to mask) */
#define OINTSUBMSK_REG		0x01c /* VIC SOFT INT CLEAR */
#define OVECTADDR_REG		0xf00 /* VIC ADDRESS */

#endif /* __S3C64XX_VIC_H__ */
