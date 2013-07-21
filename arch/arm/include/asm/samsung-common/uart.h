/*
 * (C) Copyright 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Heungjun Kim <riverful.kim@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_UART_H_
#define __ASM_ARCH_UART_H_

#ifndef __ASSEMBLY__
/* baudrate rest value */
union br_rest {
	unsigned short	slot;		/* udivslot */
	unsigned char	value;		/* ufracval */
};

struct s5p_uart {
	unsigned int	ulcon;
	unsigned int	ucon;
	unsigned int	ufcon;
	unsigned int	umcon;
	unsigned int	utrstat;
	unsigned int	uerstat;
	unsigned int	ufstat;
	unsigned int	umstat;
	unsigned char	utxh;
	unsigned char	res1[3];
	unsigned char	urxh;
	unsigned char	res2[3];
	unsigned int	ubrdiv;
	union br_rest	rest;
#if defined(CONFIG_S5PC100) || defined(CONFIG_S5PC110)
	unsigned char	res3[0x3d0];
#else /* Exynos 4 and 5 */
	unsigned char	res3[0xffd0];
#endif
};


static inline int s5p_uart_divslot(void)
{
#if defined(CONFIG_S5PC100) || defined(CONFIG_S5PC110)
	return 1;
#else /* Exynos 4 and 5 */
	return 0;
#endif
}

#define RX_FIFO_COUNT_MASK      0xff
#define RX_FIFO_FULL_MASK       (1 << 8)
#define TX_FIFO_FULL_MASK       (1 << 24)

/* Information about a serial port */
struct fdt_serial {
	u32 base_addr;  /* address of registers in physical memory */
	u8 port_id;     /* uart port number */
	u8 enabled;     /* 1 if enabled, 0 if disabled */
};

#endif	/* __ASSEMBLY__ */

#endif
