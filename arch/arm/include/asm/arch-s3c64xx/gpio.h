/*
 * S3C64xx GPIO controller registers.
 *
 * (C) Copyright 2013
 * Lukasz Majewski <l.majewski@majess.pl>
 * Tomasz Figa <tomasz.figa@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __S3C64XX_GPIO_H__
#define __S3C64XX_GPIO_H__

#define GPACON_REG		0x00
#define GPADAT_REG		0x04
#define GPAPUD_REG		0x08
#define GPACONSLP_REG		0x0C
#define GPAPUDSLP_REG		0x10
#define GPBCON_REG		0x20
#define GPBDAT_REG		0x04
#define GPBPUD_REG		0x08
#define GPBCONSLP_REG		0x0C
#define GPBPUDSLP_REG		0x30
#define GPCCON_REG		0x40
#define GPCDAT_REG		0x44
#define GPCPUD_REG		0x48
#define GPCCONSLP_REG		0x4C
#define GPCPUDSLP_REG		0x50
#define GPDCON_REG		0x60
#define GPDDAT_REG		0x64
#define GPDPUD_REG		0x68
#define GPDCONSLP_REG		0x6C
#define GPDPUDSLP_REG		0x70
#define GPECON_REG		0x80
#define GPEDAT_REG		0x84
#define GPEPUD_REG		0x88
#define GPECONSLP_REG		0x8C
#define GPEPUDSLP_REG		0x90
#define GPFCON_REG		0xA0
#define GPFDAT_REG		0xA4
#define GPFPUD_REG		0xA8
#define GPFCONSLP_REG		0xAC
#define GPFPUDSLP_REG		0xB0
#define GPGCON_REG		0xC0
#define GPGDAT_REG		0xC4
#define GPGPUD_REG		0xC8
#define GPGCONSLP_REG		0xCC
#define GPGPUDSLP_REG		0xD0
#define GPHCON0_REG		0xE0
#define GPHCON1_REG		0xE4
#define GPHDAT_REG		0xE8
#define GPHPUD_REG		0xEC
#define GPHCONSLP_REG		0xF0
#define GPHPUDSLP_REG		0xF4
#define GPICON_REG		0x100
#define GPIDAT_REG		0x104
#define GPIPUD_REG		0x108
#define GPICONSLP_REG		0x10C
#define GPIPUDSLP_REG		0x110
#define GPJCON_REG		0x120
#define GPJDAT_REG		0x124
#define GPJPUD_REG		0x128
#define GPJCONSLP_REG		0x12C
#define GPJPUDSLP_REG		0x130
#define SPCON_REG		0x1A0
#define MEM0DRVCON_REG		0x1D0
#define MEM1DRVCON_REG		0x1D4
#define GPKCON0_REG		0x800
#define GPKCON1_REG		0x804
#define GPKDAT_REG		0x808
#define GPKPUD_REG		0x80C
#define GPLCON0_REG		0x810
#define GPLCON1_REG		0x814
#define GPLDAT_REG		0x818
#define GPLPUD_REG		0x81C
#define GPMCON_REG		0x820
#define GPMDAT_REG		0x824
#define GPMPUD_REG		0x828
#define GPNCON_REG		0x830
#define GPNDAT_REG		0x834
#define GPNPUD_REG		0x838
#define GPOCON_REG		0x140
#define GPODAT_REG		0x144
#define GPOPUD_REG		0x148
#define GPOCONSLP_REG		0x14C
#define GPOPUDSLP_REG		0x150
#define GPPCON_REG		0x160
#define GPPDAT_REG		0x164
#define GPPPUD_REG		0x168
#define GPPCONSLP_REG		0x16C
#define GPPPUDSLP_REG		0x170
#define GPQCON_REG		0x180
#define GPQDAT_REG		0x184
#define GPQPUD_REG		0x188
#define GPQCONSLP_REG		0x18C
#define GPQPUDSLP_REG		0x190
#define SPCON_REG		0x1a0
#define EINTPEND_REG		0x924

#endif /* __S3C64XX_GPIO_H__ */
