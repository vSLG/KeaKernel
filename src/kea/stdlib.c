// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file stdlib.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements KeaOS/3 standard library
 * @version 0.1
 * @date 2021-05-12
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <stdint.h>

#include <kea/stdlib.h>

char *itoa(long num, char *buf, int base) {
    uint8_t neg   = 0;
    char *  start = buf;

    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    *buf++ = '\0';

    if (num < 0 && base == 10) {
        neg = 1;
        num = -num;
    }

    uint64_t unum = num;
    while (unum > 0) {
        *buf++ = "0123456789abcdef"[unum % base];
        unum   = unum / base;
    }

    if (neg) {
        *buf++ = '-';
    }

    char *orig = start;
    char  s;
    while (start < buf) {
        s        = *(--buf);
        *buf     = *start;
        *start++ = s;
    }

    return orig;
}
