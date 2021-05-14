/**
 * @file main.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Entry point of the kernel, set up everything
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

#include "io/tty.h"
#include "mb2.h"

CALIGN(64)
uint8_t kernel_stack[1 << 12];

static boot_info_t boot_info;

NORETURN void _centry(void *mb2_ptr) {
    mb2_ptr += KERNEL_VMA;

    init_tty(COM1);
    init_mb2(mb2_ptr, &boot_info);

    kprintf("kk eae men\n");
    kprintf("%s\n", boot_info.bootloader_name);
    kprintf("%s\n", boot_info.cmdline);

    // Print OKAY to screen
    *((uint64_t *) (0xb8000 + KERNEL_VMA)) = 0x2f592f412f4b2f4f;

    while (1)
        __asm__("hlt");
}
