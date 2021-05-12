/* Copyright (c) 2021 KeaOS/3 developers and contributors */

#include <linker.h>

_KERNEL_PHYS = 0x100000;
_KERNEL_VMA  = KERNEL_VMA;

OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_entry)

SECTIONS {
    /* Bootstrap code, only ASM allowed */

    . = _KERNEL_PHYS;

    .boot ALIGN(64) : {
        *(.mb2)
        *(.boot.text)
        *(.boot.data)
    }

    .boot.bss ALIGN(64) : {
        boot_stack_bottom = .;
        . += 128;
        boot_stack_top = .;
        *(.boot.bss)
    }

    . += _KERNEL_VMA;

    /* Add a symbol that indicates the start address of the kernel. */
    _kernel_start = .;

    .text ALIGN (4K) : AT (ADDR (.text) - _KERNEL_VMA) {
		_text_start = .;
        *(.text)
		_text_end = .;
    }

    .rodata ALIGN (4K) : AT (ADDR (.rodata) - _KERNEL_VMA) {
		_rodata_start = .;
        *(.rodata)
		_rodata_end = .;
    }

    .data ALIGN (4K) : AT (ADDR (.data) - _KERNEL_VMA) {
        *(.data)
    }

    .bss ALIGN (4K) : AT (ADDR (.bss) - _KERNEL_VMA) {
        _bss_start_ = .;
        *(COMMON)
        *(.bss)
        _bss_end_ = .;
    }

    /* Add a symbol that indicates the end address of the kernel. */
    _kernel_end = .;
}
