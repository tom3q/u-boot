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
#define CONFIG_SPICA			1 /* SAMSUNG Spica board */

#include <asm/arch/cpu.h>		/* get chip and board defs */

#define CONFIG_SYS_SDRAM_BASE		0x50000000
#define CONFIG_SYS_MEM_TOP_HIDE		0x02800000

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

//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE	255
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
//#define ONENAND_DEBUG

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL3			/* Use UART2 */
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
#define CONFIG_CMD_MTDPARTS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_ONENAND
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS

#define CONFIG_BOOTDELAY		2
#define CONFIG_AUTOBOOT_KEYED		/* Enable password protection	*/
#define CONFIG_AUTOBOOT_PROMPT		\
	"Press ENTER to abort autoboot in %d seconds\n", bootdelay
#define CONFIG_AUTOBOOT_DELAY_STR	"kj"
#define CONFIG_AUTOBOOT_DELAY_STR2	"jk"
#define CONFIG_AUTOBOOT_STOP_STR	"\r"
#define CONFIG_AUTOBOOT_STOP_STR2	"\n"
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_BOOT_RETRY_TIME		3
#define CONFIG_RESET_TO_RETRY

#if (CONFIG_COMMANDS & CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE		115200	/* speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX		1	/* which serial port to use	 */
#endif

#define MTDIDS_DEFAULT			"onenand0=samsung-onenand"
#define MTDPARTS_DEFAULT		"mtdparts=samsung-onenand:" \
	"128k(pbl)ro,"			\
	"1280k(sbl)ro,"			\
	"128k(logo)ro,"			\
	"256k(param)ro,"		\
	"5m(u-boot)ro,"			\
	"256k(pad1)ro,"			\
	"462m(UBI),"			\
	"256k(pad2)ro,"			\
	"8m(efs)ro,"			\
	"16m(modem)ro,"			\
	"8m(efs_legacy)ro,"		\
	"10880k(reservoir)ro,"		\
	"128k(dgs)ro"

#define CONFIG_BOOTARGS			"Please use defined boot"
#define CONFIG_BOOTCOMMAND		"true"
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC2,115200n8\0"

#define CONFIG_ENV_UBI_MTD		" ubi.mtd=${ubiblock} ubi.mtd=7"
#define CONFIG_UBIBLOCK			"5"

#define CONFIG_ENV_UBIFS_OPTION		" rootflags=bulk_read,no_chk_data_crc "
#define CONFIG_ENV_FLASHBOOT		\
	CONFIG_ENV_UBI_MTD CONFIG_ENV_UBIFS_OPTION "${mtdparts}"

#define CONFIG_ENV_COMMON_BOOT		"${console} ${meminfo}"

#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"ubiblock=" CONFIG_UBIBLOCK "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"ubi=enabled\0" \
	"stdin=serial,samsung-keypad\0" \
	"stdout=serial,lcd\0" \
	"stderr=serial,lcd\0"

/*
 * Miscellaneous configurable options
 */
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP		/* undef to save memory	      */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser	*/
#define CONFIG_SYS_PROMPT		"Spica # "	/* Monitor Command Prompt     */
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

/* Spica has 2 banks of DRAM */
#define CONFIG_NR_DRAM_BANKS		2
#define CONFIG_BOARD_DRAM_CONFIG		\
	/* 128 MiB in bank 1 */ \
	{ CONFIG_SYS_SDRAM_BASE, 0x08000000 }, \
	/* 80 MiB in bank 2 */ \
	{ CONFIG_SYS_SDRAM_BASE + 0x08000000, 0x05000000 },

/*
 * Spica board specific data
 */

/* Put environment copies after the end of U-Boot owned RAM */
#define CONFIG_NAND_ENV_DST		\
	(CONFIG_SYS_UBOOT_BASE + CONFIG_SYS_UBOOT_SIZE)

/* Settings as above boot configuration */
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			(256 << 10)	/* 256 KiB, 0x40000 */
#define CONFIG_ENV_ADDR			(1 << 20)	/* 1 MB, 0x100000 */

#define CONFIG_USE_ONENAND_BOARD_INIT
#define CONFIG_SAMSUNG_ONENAND		1
#define CONFIG_SAMSUNG_ONENAND_BURST_READ
#define CONFIG_SAMSUNG_ONENAND_BURST_WRITE

#define CONFIG_DOS_PARTITION		1

/*
 * I2C Settings
 */
#include <asm/arch/gpio.h>

#define CONFIG_SOFT_I2C_GPIO_SCL	S3C64XX_GPE(3)
#define CONFIG_SOFT_I2C_GPIO_SDA	S3C64XX_GPE(4)

#define CONFIG_SOFT_I2C
#define I2C_DELAY			udelay(1)
#define CONFIG_SYS_I2C_SPEED		(250000)
#define CONFIG_SOFT_I2C_READ_REPEATED_START

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_MAX8698

/*
 * Input devices
 */

#define CONFIG_KEYBOARD
#define CONFIG_SAMSUNG_KEYPAD

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

#define CONFIG_SPICA_MENU		\
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
