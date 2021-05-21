// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file tty.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Declares TTY functions
 * @version 0.1
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

typedef enum serial_port {
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
} serial_port_t;

/**
 * @brief Initializes a TTY on specified port
 *
 * @param port
 */
void init_tty(serial_port_t port);
