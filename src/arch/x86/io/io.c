/**
 * @file io.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements I/O operations
 * @version 0.1
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include "io.h"

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void outb(uint16_t port, uint8_t data) {
    __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
}

void outw(uint16_t port, uint16_t data) {
    __asm__("out %%ax, %%dx" : : "a"(data), "d"(port));
}

uint32_t ind(uint16_t port) {
    uint32_t result;
    __asm__("in %%dx, %%eax" : "=a"(result) : "d"(port));
    return result;
}

void outd(uint16_t port, uint32_t data) {
    __asm__("out %%eax, %%dx" : : "a"(data), "d"(port));
}
