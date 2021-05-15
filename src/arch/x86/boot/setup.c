/**
 * @file setup.c
 * @author vslg (slgf@protonmail.ch)
 * @brief C entry point of the kernel, set up everything
 * @version 0.1
 * @date 2021-05-11
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <stdint.h>

#include <kea/boot.h>
#include <kea/kprintf.h>
#include <kea/mem.h>
#include <linker.h>

#include "../cpu/gdt.h"
#include "../io/tty.h"
#include "mb2.h"

CALIGN(64)
uint8_t kernel_stack[1 << 12];

static boot_info_t boot_info;

NORETURN void _centry(void *mb2_ptr) {
    mb2_ptr += KERNEL_VMA;

    init_tty(COM1);
    kprintf("kk eae men, KeaOS/3 booting\n");

    init_mb2(mb2_ptr, &boot_info);
    kprintf("%s\n", boot_info.bootloader_name);
    kprintf("%s\n", boot_info.cmdline);
    kprintf("Memory blocks count: %d\n", boot_info.mmap_count);

    init_gdt();

    uint64_t max_mem = 0;
    for (uint32_t i = 0; i < boot_info.mmap_count; i++)
        if (boot_info.mmaps[i].type == MEM_AVAILABLE)
            max_mem += boot_info.mmaps[i].len;

    max_mem = max_mem & ~1023;
    kprintf("Total available memory: %ld bytes\n", max_mem);

    // Print OKAY to screen
    *((uint64_t *) (0xb8000 + KERNEL_VMA + 160)) = 0x2f592f412f4b2f4f;

    while (1)
        __asm__("hlt");
}
