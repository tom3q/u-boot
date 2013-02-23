/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
 *
 * Configuation settings for the SAMSUNG SMDK6400(mDirac-III) board.
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

#ifndef __CONFIG_H
#define __CONFIG_H

#define DEBUG

/* High Level Configuration Options */
#define CONFIG_SAMSUNG			1 /* SAMSUNG core */
#define CONFIG_S3C64XX			1 /* SAMSUNG S3C64XX Family */
#define CONFIG_S3C6400			1 /* SAMSUNG S3C6400 SoC */
#define CONFIG_MINI6410			1 /* SAMSUNG Spica board */

#include <asm/arch/cpu.h>		/* get chip and board defs */

#define CONFIG_SYS_SDRAM_BASE		0x50000000

/* base address for uboot */
#define CONFIG_SYS_TEXT_BASE		0x50800000
#define CONFIG_SYS_PHY_UBOOT_BASE	CONFIG_SYS_TEXT_BASE

/* input clock of PLL: SMDK6400 has 12MHz input clock */

#define CONFIG_SYS_CLK_FREQ		12000000

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

#define CONFIG_ARCH_NO_CPU_INIT

/*
 * Architecture magic and machine type
 */
#define CONFIG_MACH_TYPE		3425

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 1024 * 1024)

/*
 * Hardware drivers
 */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_SDHCI
#define CONFIG_S5P_SDHCI

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1			/* Use UART0 */
#define CONFIG_BAUDRATE			115200

#define CONFIG_CONSOLE_MUX

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/***********************************************************
 * Command definition
 ***********************************************************/
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_REGINFO
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_LOADB
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING

#define CONFIG_BOOTDELAY		2
#define CONFIG_AUTOBOOT_KEYED		/* Enable password protection	*/
#define CONFIG_AUTOBOOT_PROMPT		\
	"Press SPACE to abort autoboot in %d seconds\n", bootdelay
#define CONFIG_AUTOBOOT_DELAY_STR	"12"	/* 1st "password"	*/
#define CONFIG_AUTOBOOT_DELAY_STR2	"78"	/* 1st "password"	*/
#define CONFIG_AUTOBOOT_STOP_STR	" "
#define CONFIG_ZERO_BOOTDELAY_CHECK

#if (CONFIG_COMMANDS & CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200	/* speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	1	/* which serial port to use	 */
#endif

#define CONFIG_BOOTARGS			"Please use defined boot"
#define CONFIG_BOOTCOMMAND		"true"
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC2,115200n8\0"

#define CONFIG_ENV_UBIFS_OPTION		" rootflags=bulk_read,no_chk_data_crc "
#define CONFIG_ENV_FLASHBOOT		\
	CONFIG_ENV_UBI_MTD CONFIG_ENV_UBIFS_OPTION "${mtdparts}"

#define CONFIG_ENV_COMMON_BOOT		"${console} ${meminfo}"

#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"stdin=serial,gpio-keys\0"	\
	"stdout=serial,lcd\0"		\
	"stderr=serial,lcd\0"

/*
 * Miscellaneous configurable options
 */
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP		/* undef to save memory	      */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser	*/
#define CONFIG_SYS_PROMPT		"Mini6410 # "	/* Monitor Command Prompt     */
#define CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size    */
#define CONFIG_SYS_PBSIZE		384		/* Print Buffer Size          */
#define CONFIG_SYS_MAXARGS		16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size  */
#define CONFIG_SYS_MEMTEST_START	(CONFIG_SYS_SDRAM_BASE + 0x1000000)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0xd000000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x4000000)
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_LOAD_ADDR - 0x1000000)

#define CONFIG_SYS_HZ			1000

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* Spica has 2 banks of DRAM, but we use only one in U-Boot */
#define CONFIG_NR_DRAM_BANKS		2

/* 128 MB in Bank #1	*/
#define PHYS_SDRAM_1			CONFIG_SYS_SDRAM_BASE
#define PHYS_SDRAM_1_SIZE		0x08000000

/* 80 MB in Bank #2	*/
#define PHYS_SDRAM_2			CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE
#define PHYS_SDRAM_2_SIZE		0x08000000

#define BOARD_DRAM_CONFIG		\
	{ PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE }, \
	{ PHYS_SDRAM_2, PHYS_SDRAM_2_SIZE },

/*
 * Mini6410 board specific data
 */

/* Put environment copies after the end of U-Boot owned RAM */
#define CONFIG_NAND_ENV_DST		\
	(CONFIG_SYS_UBOOT_BASE + CONFIG_SYS_UBOOT_SIZE)

/* Settings as above boot configuration */
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			(256 << 10)	/* 256 KiB, 0x40000 */
#define CONFIG_ENV_ADDR			(1 << 20)	/* 1 MB, 0x100000 */

#define CONFIG_DOS_PARTITION		1

/*
 * DM9000
 */

#define CONFIG_DRIVER_DM9000		1
#define CONFIG_DM9000_BASE		0x18000300
#define DM9000_IO			CONFIG_DM9000_BASE
#define DM9000_DATA			(CONFIG_DM9000_BASE + 4)

/*
 * Input devices
 */

#define CONFIG_KEYBOARD
#define CONFIG_GPIO_KEYS

/*
 * Framebuffer
 */

#define CONFIG_EXYNOS_FB
#define CONFIG_LCD
#define LCD_BPP				LCD_COLOR16
#define CONFIG_LCD_LOGO
#undef LCD_TEST_PATTERN
#define CONFIG_SYS_WHITE_ON_BLACK
#define CONFIG_CMD_BMP
#define CONFIG_BMP_32BPP
#define CONFIG_FB_ADDR			0x54504000
#define CONFIG_VIDEO_BMP_GZIP
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE	((500 * 120 * 4) + (1 << 12))
#define CONFIG_MENU_SHOW
#define CONFIG_VISUAL_MENU

#define CONFIG_MINI6410_MENU		\
	{					\
		.label = "Boot normal kernel",	\
		.handler = "run normalboot",	\
	}, {					\
		.label = "Boot recovery kernel",\
		.handler = "run recoveryboot",	\
	}, {					\
		.label = "Boot from MMC",	\
		.handler = "run mmcboot",	\
	},

#endif	/* __CONFIG_H */
