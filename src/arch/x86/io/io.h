// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file io.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Declares input/output ports methods
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

/**
 * @brief Gets byte from specified port
 *
 * @param port
 * @return uint8_t
 */
uint8_t inb(uint16_t port);

/**
 * @brief Writes byte to specified port
 *
 * @param port
 * @param data
 */
void outb(uint16_t port, uint8_t data);

/**
 * @brief Gets word from specified port
 *
 * @param port
 * @return uint16_t
 */
uint16_t inw(uint16_t port);

/**
 * @brief Writes word to specified port
 *
 * @param port
 * @param data
 */
void outw(uint16_t port, uint16_t data);

/**
 * @brief Gets dword from specified port
 *
 * @param port
 * @return uint32_t
 */
uint32_t ind(uint16_t port);

/**
 * @brief Writes dword to specified port
 *
 * @param port
 * @param data
 */
void outd(uint16_t port, uint32_t data);
