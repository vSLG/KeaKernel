// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file boot.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Declares boot related structs
 * @version 0.1
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

#include <memory.h>
#include <plat.h>

#define BOOTLOADER_NAME_LEN 64
#define CMDLINE_LEN         256
#define MAX_MMAPS           32

/**
 * @struct boot_info
 *
 * @brief Gathers general information got when booting
 */
typedef struct boot_info {
    char bootloader_name[BOOTLOADER_NAME_LEN]; ///< Name of the bootloader we
                                               ///< were booted
    char cmdline[CMDLINE_LEN]; ///< Commandline passed to our kernel

    memory_block_t mmaps[MAX_MMAPS]; ///< Available physical memory maps
    uint8_t        mmap_count;

    plat_t plat; ///< Platform specific properties
} boot_info_t;
