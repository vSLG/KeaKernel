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
#include <linker.h>

#include "boot/mb2.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "io/pic.h"
#include "io/tty.h"

CALIGN(64)
uint8_t kernel_stack[1 << 12];

static boot_info_t boot_info;

NORETURN void _centry(void *mb2_ptr) {
    mb2_ptr += KERNEL_VMA;

    init_tty(COM1);
    kprintf("kk eae men, KeaOS/3 booting\n");

    init_mb2(mb2_ptr, &boot_info);
    init_gdt();
    init_pic();
    init_idt(&boot_info.plat);

    for (int i = 0; i < boot_info.mmap_count; i++)
        kprintf("RAM: %#lx-%#lx type %d\n",
                boot_info.mmaps[i].start,
                boot_info.mmaps[i].start + boot_info.mmaps[i].len,
                boot_info.mmaps[i].type);

    boot_info.plat.arch = "x86_64";

    // Print OKAY to screen
    *((uint64_t *) (0xb8000 + KERNEL_VMA + 160)) = 0x2f592f412f4b2f4f;

    while (1)
        __asm__("hlt");
}
