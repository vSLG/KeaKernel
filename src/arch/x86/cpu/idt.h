// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file idt.h
 * @author vslg (slgf@protonmail.ch)
 * @brief IDT related structs and methods
 * @version 0.1
 * @date 2021-05-15
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

#include <linker.h>
#include <plat.h>

#define IDT_ENTRIES 256
#define IDT_USER    0b01100000
#define INT_GATE    0x8E
#define TRAP_GATE   0xEF

/**
 * @enum cpu_exception
 *
 * @brief All CPU interruption codes
 */
typedef enum cpu_exception {
    EXCEPT_DIVISION_BY_ZERO         = 0,
    EXCEPT_DEBUG                    = 1,
    EXCEPT_NMI                      = 2,
    EXCEPT_BREAKPOINT               = 3,
    EXCEPT_OVERFLOW                 = 4,
    EXCEPT_OUT_OF_BOUNDS            = 5,
    EXCEPT_INVALID_OPCODE           = 6,
    EXCEPT_DEV_NOT_AVAILABLE        = 7,
    EXCEPT_DOUBLE_FAULT             = 8,
    EXCEPT_COPROCESSOR_SO           = 9,
    EXCEPT_INVALID_TSS              = 10,
    EXCEPT_SEGMENT_NOT_PRESENT      = 11,
    EXCEPT_STACK_FAULT              = 12,
    EXCEPT_GENERAL_PROTECTION_FAULT = 13,
    EXCEPT_PAGE_FAULT               = 14,
    EXCEPT_X87_FP                   = 16,
    EXCEPT_ALIGNMENT_CHECK          = 17,
    EXCEPT_MACHINE_CHECK            = 18,
    EXCEPT_SIMD_FP                  = 19,
    EXCEPT_VIRTUALIZATION           = 20,
    EXCEPT_SECURITY                 = 30,

    __force_long = 0xFFFFFFFFFFFFFFFF,
} cpu_exception_t;

/**
 * @struct idt_entry
 *
 * @brief An interrupt gate entry
 */
typedef struct idt_entry {
    uint16_t base_low; ///< Lower address of jumping address
    uint16_t seg_sel;  ///< Kernel segment selector
    uint8_t  ist : 3;  ///< Interrupt stack table
    uint8_t  zero1 : 5;
    uint8_t  flags;     ///< Includes DLP, privilege bits and type
    uint16_t base_mid;  ///< Next 16 bits of jumping address
    uint32_t base_high; ///< Last 32 bits of jumping address
    uint32_t reserved;
} PACKED idt_entry_t;

/**
 * @struct regs
 *
 * @brief Represents all registers pushed by interrupt handler
 */
typedef struct regs {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14,
        r15;                                  ///< General purpose registers
    cpu_exception_t int_no;                   ///< Interuption code
    uint64_t        err_code;                 ///< Error code, if applicable
    uint64_t        rip, cs, rflags, rsp, ss; ///< Pushed automatically by CPU
} PACKED regs_t;

/**
 * @brief Load and use our IDT
 *
 * @param *plat Platform information to be filled
 */
void init_idt(plat_t *plat);

/**
 * @brief Tells CPU to use new IDT
 *
 * Defined in descriptors.asm
 *
 * @param idt_pointer Pointer to IDT
 */
extern void idt_flush(uint64_t idt_pointer);
