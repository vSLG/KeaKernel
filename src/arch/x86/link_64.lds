/* Copyright (c) 2021 LabOS developers and contributors */

_KERNEL_BASE_ = 0x100000;
_KERNEL_VMA   = 0xfffffff800000000;

OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_entry)

SECTIONS {
    . = _KERNEL_BASE_;

    .mb2 ALIGN(64) : {
        *(.mb2)
    }

    .mb2.text ALIGN(64) : {
        *(.mb2.text)
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
