/**
 * @file descriptors.h
 * @author vslg (slgf@protonmail.ch)
 * @brief GDT & IDT related structs
 * @version 0.1
 * @date 2021-05-15
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

#include <linker.h>

/**
 * @struct descriptor_ptr
 *
 * @brief A struct describing a pointer to an array of idt_entry or gdt_entry.
 *
 * This is in a format suitable for giving to 'lidt' or 'lgdt'.
 */
typedef struct descriptor_ptr {
    uint16_t limit; ///< Size of the array minus 1
    uint64_t
        base; ///< The address of the first element in our descriptor array.
} PACKED descriptor_ptr_t;
