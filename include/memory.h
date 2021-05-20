/**
 * @file mem.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Memory related structures & declarations
 * @version 0.1
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @enum memory_type
 *
 * @brief Type of memory map
 */
typedef enum memory_type {
    MEM_AVAILABLE = 1, ///< Memory is available for use
    MEM_RESERVED,      ///< Memory is reserved, thus it should not be used
    MEM_ACPI_RECLAIMABLE,
    MEM_NVS,
    MEM_BADRAM
} memory_type_t;

/**
 * @struct memory_block
 *
 * @brief Represents a memory map/block
 */
typedef struct memory_block {
    uint64_t      start; ///< Start address of memory block
    uint64_t      len;   ///< Lenght of the block
    memory_type_t type;  ///< Type of memory, as described in above enum
} memory_block_t;
