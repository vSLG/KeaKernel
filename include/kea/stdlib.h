// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file stdlib.h
 * @author vslg (slgf@protonmail.ch)
 * @brief KeaOS/3 standard library
 * @version 0.1
 * @date 2021-05-12
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

/**
 * @brief Writes char representation of num in buf and return it
 *
 * @param num
 * @param buf
 * @param base base of num
 * @return char* buf
 */
char *itoa(long num, char *buf, int base);
