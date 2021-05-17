/**
 * @file tty.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements serial TTY
 * @version 0.1
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <stdint.h>

#include <kprintf.h>

#include "io.h"
#include "tty.h"

/**
 * Private definitions
 **/

#define trace_tty(fmt, ...) kprintf("TTY: " fmt "\n", ##__VA_ARGS__)

typedef struct {
    serial_port_t enabled_ttys[4];
    uint8_t       tty_count;
    uint8_t       kprintf;
} tty_info_t;

static tty_info_t tty_info = {0};

uint8_t is_transmit_empty(uint16_t port) {
    return inb(port + 5) & 0x20;
}

void tty_kputchar(kprintf_state_t *state) {
    for (int i = 0; i < tty_info.tty_count; i++) {
        serial_port_t port = tty_info.enabled_ttys[i];

        // Wait for transmission to be ready
        while (is_transmit_empty(port) == 0)
            ;

        outb(port, state->current_char);
    }
}

/**
 * Public definitions
 */

void init_tty(serial_port_t port) {
    outb(port + 1, 0x00); // Disable all interrupts
    outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00); //                  (hi byte)
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE); // Test serial chip (send byte 0xAE and check if
                          // serial returns same byte)

    if (inb(port) != 0xAE)
        return;

    outb(port + 4, 0x0F);

    tty_info.enabled_ttys[tty_info.tty_count++] = port;

    if (!tty_info.kprintf) {
        register_kputchar(&tty_kputchar);
        tty_info.kprintf = 1;
    }

    trace_tty("initialised a terminal on serial port %#x", port);
}
