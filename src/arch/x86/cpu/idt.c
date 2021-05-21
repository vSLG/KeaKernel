// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file idt.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements IDT routines
 * @version 0.1
 * @date 2021-05-15
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <kea/memory.h>
#include <kprintf.h>

#include "descriptors.h"
#include "idt.h"

/**
 * Private declarations
 */

extern void *interrupt_array[];

static idt_entry_t      idt_entries[IDT_ENTRIES] = {0};
static descriptor_ptr_t idt_ptr                  = {
    .limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1,
    .base  = (uint64_t) &idt_entries,
};

/* To print exception names */
static char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Instruction",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void sti() {
    asm volatile("sti");
}

void cli() {
    asm volatile("cli");
}

void isr_handler(regs_t *regs) {
    kprintf("Received interrupt: %s, int_no=%ld\n",
            exception_messages[regs->int_no],
            regs->int_no);

    switch (regs->int_no) {
        /* Disable interrupts and die when an unhandable exception occurs */
        case EXCEPT_DIVISION_BY_ZERO:
        case EXCEPT_OUT_OF_BOUNDS:
        case EXCEPT_INVALID_OPCODE:
        case EXCEPT_DEV_NOT_AVAILABLE:
        case EXCEPT_DOUBLE_FAULT:
        case EXCEPT_COPROCESSOR_SO:
        case EXCEPT_INVALID_TSS:
        case EXCEPT_SEGMENT_NOT_PRESENT:
        case EXCEPT_STACK_FAULT:
        case EXCEPT_GENERAL_PROTECTION_FAULT:
        case EXCEPT_PAGE_FAULT:
        case EXCEPT_X87_FP:
        case EXCEPT_ALIGNMENT_CHECK:
        case EXCEPT_SIMD_FP:
        case EXCEPT_VIRTUALIZATION:
        case EXCEPT_SECURITY:
            kprintf("Unrecoverable interrupt :)\n");
            cli();
            while (1) {}
        default:
            break;
    }
}

uint64_t interrupt_handler(uint64_t rsp) {
    regs_t *regs = (regs_t *) rsp;

    if (regs->int_no < 32)
        isr_handler(regs);

    return rsp;
}

void idt_set_gate(uint8_t int_no, uint64_t base, uint8_t flags, uint8_t ist) {
    idt_entries[int_no].base_low  = base & 0xFFFF;
    idt_entries[int_no].base_mid  = (base >> 16) & 0xFFFF;
    idt_entries[int_no].base_high = (base >> 32) & 0xFFFFFFFF;

    idt_entries[int_no].seg_sel = 0x8;
    idt_entries[int_no].ist     = ist;
    idt_entries[int_no].flags   = flags;

    idt_entries[int_no].zero1 = idt_entries[int_no].reserved = 0;
}

/**
 * Public declarations
 */

void init_idt(plat_t *plat) {
    memset(&idt_entries, 0, sizeof(idt_entry_t) * IDT_ENTRIES);

    for (int i = 0; i < 48; i++)
        idt_set_gate(i, (uint64_t) interrupt_array[i], INT_GATE, 0);

    idt_flush((uint64_t) &idt_ptr);

    plat->enable_int  = sti;
    plat->disable_int = cli;
}
