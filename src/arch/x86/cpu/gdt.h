// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file gdt.h
 * @author vslg (slgf@protonmail.ch)
 * @brief GDT related structs and functions
 * @version 0.1
 * @date 2021-05-15
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

#include <linker.h>

#define GDT_ENTRIES 5 // null + 4 code/data + TSS

#define GDT_SEGMENT    0b00010000
#define GDT_PRESENT    0b10000000
#define GDT_USER       0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE  0b00000010

#define GDT_LONG_MODE_GRANULARITY 0b0010

/**
 * @struct gdt_entry
 *
 * @brief This structure contains the value of one GDT entry.
 */
typedef struct gdt_entry {
    uint16_t limit_low;   ///< The lower 16 bits of the limit.
    uint16_t base_low;    ///< The lower 16 bits of the base.
    uint8_t  base_middle; ///< The next 8 bits of the base.

    // Access byte
    uint8_t access; ///< bit 7   → Is segment present?\n
                    ///< bit 6:5 → Privilege level (ring 0 or 3)\n
                    ///< bit 4   → Descriptor type, set for code/data,\n
                    ///<           clear if system\n
                    ///< bit 3   → Executable bit, clear if data\n
                    ///< bit 2   → Direction bit for data, conforming\n
                    ///<           for code\n
                    ///< bit 1   → Readable for code, writable for data\n
                    ///< bit 0   → Accessed bit

    uint8_t limit_high : 4; /// Upper 4 bits of the limit.

    // Flags field
    uint8_t flags : 4; ///< bit 3   → Granularity (0 = 1 byte, 1 = 4KiB)\n
                       ///< bit 2   → Operand size (0 = 16bit, 1 = 32bit)\n
                       ///< bit 1   → Long mode bit, sz must be 0 if set\n
                       ///< bit 0   → Always 0

    uint8_t base_high; ///< The last 8 bits of the base.
} PACKED gdt_entry_t;

/**
 * @struct tss
 *
 * @brief IA-32e tss struct
 */
typedef struct tss {
    uint32_t reserved;
    uint64_t rsp[3]; ///< Full canonical forms of stack pointers
    uint64_t reserved2;
    uint64_t ist[8]; ///< Full canonical forms of interrupt stack table
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iomapbase; ///< Offset to IO permission bitmap from TSS base
} PACKED tss_t;

typedef struct gdt_tss_entry_t {
    gdt_entry_t entry;
    uint32_t    base_upper;
    uint32_t    reserved;
} PACKED gdt_tss_entry_t;

/**
 * @struct gdt64
 *
 * @brief Represents a full IA-32e GDT table
 */
typedef struct gdt64 {
    gdt_entry_t     entries[GDT_ENTRIES];
    gdt_tss_entry_t tss;
} PACKED gdt64_t;

/**
 * @brief Initializes new GDT
 */
void init_gdt();

/**
 * @brief Tells CPU to use new GDT
 *
 * Defined in descriptors.asm
 *
 * @param gdt_pointer Pointer to GDT
 */
extern void gdt_flush(uint64_t gdt_pointer);
