/*
 * (C) Copyright 2009 SAMSUNG Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_MMC_BOOT_H_
#define __ASM_ARCH_MMC_BOOT_H_

#include <asm/arch/cpu.h>

#define MMC_BLKSIZE		512
#define MMC_TOTAL_BLKCNT_REG	(S3C64XX_TCM1_BASE - 0x4)
#define MMC_TOTAL_BLKCNT	(*(volatile unsigned int *)MMC_TOTAL_BLKCNT_REG)

#if defined(CONFIG_S3C6400)
#define BL1_BLOCKS		((4 * 1024) / MMC_BLKSIZE)
#define RSVD_BLOCKS		((2 * 1024) / MMC_BLKSIZE)
#else
#define BL1_BLOCKS		((8 * 1024) / MMC_BLKSIZE)
#define RSVD_BLOCKS		((1 * 1024) / MMC_BLKSIZE)
#endif

#define MMC_ENV_BLKCNT		(CONFIG_ENV_SIZE / MMC_BLKSIZE)
#define MMC_BL2_BLKCNT		(CONFIG_BL2_SIZE / MMC_BLKSIZE)

#define MMC_LAST_BLKPOS		(MMC_TOTAL_BLKCNT - RSVD_BLOCKS)
#define MMC_BL1_BLKPOS		(MMC_LAST_BLKPOS - BL1_BLOCKS)
#define MMC_ENV_BLKPOS		(MMC_BL1_BLKPOS - MMC_ENV_BLKCNT)
#define MMC_BL2_BLKPOS		(MMC_ENV_BLKPOS - MMC_BL2_BLKCNT)

#endif
