/**
 * @file mem.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Defines memory operations
 * @version 0.1
 * @date 2021-05-15
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <kea/mem.h>

/**
 * Public definitions
 */

void memset(void *addr, uint8_t val, size_t len) {
    for (; len > 0; len--)
        *(uint8_t *) addr++ = val;
}
