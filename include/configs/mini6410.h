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
 * (C) Copyright 2013
 * Łukasz Majewski, <l.majewski@majess.pl>
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

/* High Level Configuration Options */
#define CONFIG_ARM1176	/* This is an ARM1176 CPU Core */
#define CONFIG_S3C64XX	/* in a SAMSUNG S3C64xx SoC */

#define CONFIG_PERIPORT_REMAP
#define CONFIG_PERIPORT_BASE	0x70000000
#define CONFIG_PERIPORT_SIZE	0x13

/* No need for low level init in u-boot, because SPL handles it. */
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF

#define CONFIG_SYS_SDRAM_BASE	0x50000000
#define CONFIG_SYS_TEXT_BASE	0x57e00000
#define CONFIG_SYS_UBOOT_BASE	CONFIG_SYS_TEXT_BASE

/* input clock of PLL: Mini6410 has 12MHz input clock */
#define CONFIG_SYS_CLK_FREQ	12000000

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

/*
 * Architecture magic and machine type
 */
#define CONFIG_MACH_TYPE	2520

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	(CONFIG_ENV_SIZE + 1024 * 1024)

/*
 * Hardware drivers
 */
#define CONFIG_DRIVER_DM9000
#define CONFIG_DRIVER_DM9000_NO_EEPROM
#define CONFIG_DM9000_BASE	0x18000000
#define DM9000_IO		CONFIG_DM9000_BASE
#define DM9000_DATA		(CONFIG_DM9000_BASE + 4)

/*
 * select serial console configuration
 */
#define CONFIG_S5P_SERIAL	1
#define CONFIG_SERIAL0		1	/* we use SERIAL 0 on Mini6410	*/
#define CONFIG_BAUDRATE		115200

#define CONFIG_CMDLINE_EDITING

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_SYS_NO_FLASH		1

/***********************************************************
 * Command definition
 ***********************************************************/
#include <config_cmd_default.h>

#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_MISC
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_XIMG
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_REGINFO
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2

#define CONFIG_BOOTDELAY	3
#define CONFIG_ZERO_BOOTDELAY_CHECK

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200	/* speed to run kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	1	/* which serial port to use	 */
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP	1	/* undef to save memory	      */
#define CONFIG_SYS_HUSH_PARSER	1	/* use "hush" command parser	*/
#define CONFIG_AUTO_COMPLETE	1
#define CONFIG_SYS_PROMPT	"Mini6410 # "
#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size    */
#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size          */
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
/* Boot Argument Buffer Size  */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
/* Memtest configuration */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5000000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x4000000)

#define CONFIG_SYS_HZ			1000


/* Mini6410 has 2 banks of DRAM, but we use only one in U-Boot */
#define CONFIG_NR_DRAM_BANKS	2
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE
#define PHYS_SDRAM_1_SIZE	(128 * 1024 * 1024)
#define PHYS_SDRAM_2		(CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE)
#define PHYS_SDRAM_2_SIZE	(128 * 1024 * 1024)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 KiB */

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_LOAD_ADDR - 0x1000000)

/* Environment parameters */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(128 * 1024)

/* U-boot copy size from boot Media to DRAM.*/
#define	CONFIG_BL2_SIZE			(256 * 1024)

/*
 * Mini6410 board specific data
 */

#define CONFIG_BOOTCOMMAND	"nand read 0x50018000 0x60000 0x1c0000;" \
				"bootm 0x50018000"

#define CONFIG_BOOTARGS		"console=ttySAC0,115200n8"

#define CONFIG_CMD_USB				1
#define CONFIG_USB_OHCI_NEW			1
#define CONFIG_SYS_USB_OHCI_REGS_BASE		0x74300000
#define CONFIG_SYS_USB_OHCI_SLOT_NAME		"s3c6400"
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	3
#define CONFIG_SYS_USB_OHCI_CPU_INIT		1

#define CONFIG_USB_STORAGE	1
#define CONFIG_DOS_PARTITION	1

/* SD/MMC configuration */
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_SDHCI
#define CONFIG_S5P_SDHCI
#define CONFIG_CMD_MMC

/* MMC SPL */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_MAX_FOOTPRINT	(8 * 1024)
#define CONFIG_SPL_TEXT_BASE		0x0c000000
#define CONFIG_SPL_STACK		0x0c005000
#define CONFIG_SPL_LDSCRIPT	"arch/arm/cpu/arm1176/s3c64xx/u-boot-spl.lds"
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_RAM_DEVICE

/*
 * Supported Clock Settings
 *---------------------------------
 * Setting	SYNC	ASYNC
 *---------------------------------
 * 667_133_66	 X	  O
 * 533_133_66	 O	  O
 * 532_133_66	 O	  O
 * 400_133_66	 X	  O
 * 400_100_50	 O	  O
 */

/*
#define CONFIG_CLK_667_133_66
#define CONFIG_CLK_533_133_66
*/
#define CONFIG_CLK_532_133_66
/*
#define CONFIG_CLK_400_100_50
#define CONFIG_CLK_400_133_66
*/
#define CONFIG_SYNC_MODE

#endif	/* __CONFIG_H */
