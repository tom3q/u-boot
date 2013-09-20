/*
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
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
#include <asm/arch/cpu.h>
#include <asm/arch/clk.h>
/* ------------------------------------------------------------------------- */
/*
 * NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_fclk(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

unsigned long get_pll_clk(int pllreg)
{
	volatile void *sys_base = (volatile void *)samsung_get_base_sys();
	unsigned long r, m, p, s;

	switch (pllreg) {
	case APLL:
		r = readl(sys_base + APLL_CON_REG);
		break;
	case MPLL:
		r = readl(sys_base + MPLL_CON_REG);
		break;
	case EPLL:
		r = readl(sys_base + EPLL_CON0_REG);
		break;
	default:
		hang();
	}

	m = (r >> 16) & 0x3ff;
	p = (r >> 8) & 0x3f;
	s = r & 0x7;

	return m * (CONFIG_SYS_CLK_FREQ / (p * (1 << s)));
}

/* return ARMCORE frequency */
unsigned long get_arm_clk(void)
{
	volatile void *sys_base = (volatile void *)samsung_get_base_sys();
	unsigned long div;

	div = readl(sys_base + CLK_DIV0_REG);

	return get_pll_clk(APLL) / ((div & 0x7) + 1);
}

/* return FCLK frequency */
unsigned long get_fclk(void)
{
	return get_pll_clk(APLL);
}

/* return HCLK frequency */
unsigned long get_hclk(void)
{
	volatile void *sys_base = (volatile void *)samsung_get_base_sys();
	unsigned long fclk;
	u32 hclkx2_div, hclk_div;
	u32 div0;

	div0 = readl(sys_base + CLK_DIV0_REG);
	hclkx2_div = ((div0 >> 9) & 0x7) + 1;
	hclk_div = ((div0 >> 8) & 0x1) + 1;

	/*
	 * Bit 7 exists on s3c6410, and not on s3c6400, it is reserved on
	 * s3c6400 and is always 0, and it is indeed running in ASYNC mode
	 */
	if (readl(sys_base + OTHERS_REG) & 0x80)
		fclk = get_fclk();		/* SYNC Mode	*/
	else
		fclk = get_pll_clk(MPLL);	/* ASYNC Mode	*/

	return fclk / (hclk_div * hclkx2_div);
}

/* return PCLK frequency */
unsigned long get_pclk(void)
{
	volatile void *sys_base = (volatile void *)samsung_get_base_sys();
	unsigned long fclk;
	u32 hclkx2_div, pre_div;
	u32 div0;

	div0 = readl(sys_base + CLK_DIV0_REG);
	hclkx2_div = ((div0 >> 9) & 0x7) + 1;
	pre_div = ((div0 >> 12) & 0xf) + 1;

	if (readl(sys_base + OTHERS_REG) & 0x80)
		fclk = get_fclk();		/* SYNC Mode	*/
	else
		fclk = get_pll_clk(MPLL);	/* ASYNC Mode	*/

	return fclk / (hclkx2_div * pre_div);
}

/* return UCLK frequency */
unsigned long get_uclk(void)
{
	return get_pll_clk(EPLL);
}

unsigned long get_mmc_clk(int dev_index)
{
	volatile void *sys_base = (volatile void *)samsung_get_base_sys();
	unsigned int tmp;
	unsigned long freq;

	tmp = readl(sys_base + CLK_SRC_REG);
	tmp >>= 18 + 2 * dev_index;
	tmp &= 0x3;

	switch (tmp) {
	case 0:
		freq = get_pll_clk(EPLL);
		break;
	case 1:
		freq = get_pll_clk(MPLL);
		break;
	case 2:
		freq = CONFIG_SYS_CLK_FREQ;
		break;
	case 3:
		freq = 27000000;
		break;
	}

	tmp = readl(sys_base + CLK_SRC_REG);
	tmp >>= 4 * dev_index;
	tmp &= 0xf;
	tmp += 1;
	freq /= tmp;

	debug("%s: mmc clock = %lu\n", __func__, freq);

	return freq;
}

void set_mmc_clk(int dev_index, unsigned int div)
{
	/* Do nothing */
}

unsigned long get_pwm_clk(void)
{
	return get_pclk();
}

unsigned long get_uart_clk(int dev_index)
{
	return get_pclk();
}
