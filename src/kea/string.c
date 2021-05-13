/**
 * @file string.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements string operations
 * @version 0.1
 * @date 2021-05-12
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <stdint.h>

#include <kea/string.h>

void *memcpy(void *dest, const void *src, size_t count) {
    uint8_t *      dest_u8 = dest;
    const uint8_t *src_u8  = src;
    for (size_t i = 0; i < count; i++)
        dest_u8[i] = src_u8[i];
    return dest;
}

size_t strlen(const char *buf) {
    size_t i = 0;
    while (*buf++ != '\0')
        i++;
    return i;
}

char toupper(char character) {
    if (character >= 'a' && character <= 'z')
        return character - 0x20;
    return character;
}