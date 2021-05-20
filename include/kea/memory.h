/**
 * @file mem.h
 * @author vslg (slgf@protonmail.ch)
 * @brief System memory management
 * @version 0.1
 * @date 2021-05-18
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum mem_flags {
    MEM_NONE,
    MEM_USER  = 1,
    MEM_CLEAR = 2,
} mem_flags_t;

void memset(void *addr, uint8_t val, size_t len);