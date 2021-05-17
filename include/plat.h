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

#include <kea/mem.h>

typedef uint8_t (*plat_vmap_t)(void *         address_space,
                               memory_block_t phys_block,
                               uintptr_t      vaddr,
                               uint8_t        flags);

typedef void (*plat_enable_int_t)();
typedef void (*plat_disable_int_t)();

/**
 * @struct plat
 *
 * @brief Platfom specific properties and methods
 */
typedef struct plat {
    char *            arch; ///< Arch name
    plat_vmap_t       vmap; ///< Method to map physical memory to virtual memory
    plat_enable_int_t enable_int;   ///< Method to enable interrupts
    plat_disable_int_t disable_int; ///< Method to disable interrupts
} plat_t;
