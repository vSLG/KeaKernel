// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file phys_mem.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Physical memory management
 * @version 0.1
 * @date 2021-05-16
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <boot.h>
#include <memory.h>

/**
 * @brief Allocates specified size on physical memory
 *
 * @param size Size of memory to be allocated, must be 4KiB aligned
 * @return memory_block_t Block of memory allocated
 */
memory_block_t phys_alloc(size_t size);

/**
 * @brief Sets memory block as free
 *
 * @param block
 */
void phys_free(memory_block_t block);

/**
 * @brief initializes physical memory management
 *
 * @param boot_info_t* Boot information
 */
void init_phys_mem(boot_info_t *boot_info);

/**
 * @brief Gets next non-used physical space with specified size without allocing
 *
 * @param size_t* Offset where to start, gets modified
 * @param size_t  Wanted size to search
 * @return memory_block_t Memory block of free space. Length will be 0 if no
                          enough free space.
 */
memory_block_t phys_next_free_space(size_t *offset, size_t size);

/**
 * @brief Sets specified physical region as used
 *
 * @param memory_block_t
 */
void phys_set_used(memory_block_t block);
