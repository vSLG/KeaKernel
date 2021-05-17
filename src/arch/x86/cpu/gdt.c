/**
 * @file gdt.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements GDT routines
 * @version 0.1
 * @date 2021-05-15
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

/**
 * Private declarations
 */

#include "gdt.h"
#include "descriptors.h"

static tss_t   tss = {0};
static gdt64_t gdt = {
    .entries = {0},
    .tss =
        {
            .entry =
                {
                    .limit_low  = sizeof(tss_t),
                    .limit_high = 0,
                    .access     = 0b10001001,
                    .flags      = 0,
                },
            .reserved = 0,
        },
};
static descriptor_ptr_t gdt_ptr = {
    .limit = sizeof(gdt64_t) - 1,
    .base  = (uint64_t) &gdt,
};

/**
 * @brief Set up a GDT entry. @see gdt_entry
 */
static void gdt_set_gate(uint32_t idx,
                         uint32_t base,
                         uint32_t limit,
                         uint8_t  access,
                         uint8_t  flags) {
    gdt.entries[idx].base_low    = base & 0xFFFF;
    gdt.entries[idx].base_middle = (base >> 16) & 0xFF;
    gdt.entries[idx].base_high   = (base >> 24) & 0xFF;

    gdt.entries[idx].limit_low  = limit & 0xFFFF;
    gdt.entries[idx].limit_high = (limit >> 16) & 0xF;

    gdt.entries[idx].access = access;
    gdt.entries[idx].flags  = flags & 0b1110;
}

/**
 * Public declarations
 */

void init_gdt() {
    gdt_ptr.limit = sizeof(gdt64_t) - 1;
    gdt_ptr.base  = (uint64_t) &gdt;

    gdt_set_gate(0, 0, 0, 0, 0); // Null descriptor
    gdt_set_gate(1,
                 0,
                 0,
                 GDT_PRESENT | GDT_SEGMENT | GDT_EXECUTABLE | GDT_READWRITE,
                 GDT_LONG_MODE_GRANULARITY); // Kernel code
    gdt_set_gate(2,
                 0,
                 0,
                 GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE,
                 0); // Kernel data
    gdt_set_gate(3,
                 0,
                 0,
                 GDT_PRESENT | GDT_SEGMENT | GDT_EXECUTABLE | GDT_READWRITE |
                     GDT_USER,
                 GDT_LONG_MODE_GRANULARITY); // User code
    gdt_set_gate(4,
                 0,
                 0,
                 GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER,
                 0); // User data

    gdt_entry_t *tss_entry = &gdt.tss.entry;
    tss_entry->base_low    = (uint64_t) &tss & 0xFFFF;
    tss_entry->base_middle = ((uint64_t) &tss >> 16) & 0xFF;
    tss_entry->base_high   = ((uint64_t) &tss >> 24) & 0xFF;
    gdt.tss.base_upper     = ((uint64_t) &tss >> 32) & 0xFFFFFFFF;

    gdt_flush((uint64_t) &gdt_ptr);
}
