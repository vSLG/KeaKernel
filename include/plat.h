// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file plat.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Platform abstraction
 * @version 0.1
 * @date 2021-05-16
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

#include <memory.h>

typedef size_t (*plat_total_mem_t)();
typedef size_t (*plat_used_mem_t)();
typedef void (*plat_enable_int_t)();
typedef void (*plat_disable_int_t)();

/**
 * @struct plat
 *
 * @brief Platfom specific properties and methods
 */
typedef struct plat {
    char *arch; ///< Arch name

    plat_enable_int_t  enable_int;  ///< Method to enable interrupts
    plat_disable_int_t disable_int; ///< Method to disable interrupts

    plat_used_mem_t  used_mem;  ///< Used physical memory (raw)
    plat_total_mem_t total_mem; ///< Total physical memory available
} plat_t;
