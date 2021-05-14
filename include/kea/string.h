/**
 * @file string.h
 * @author vslg (slgf@protonmail.ch)
 * @brief String utilities
 * @version 0.1
 * @date 2021-05-12
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stddef.h>

/**
 * @brief Returns the size of null-terminated string
 *
 * @param buf
 * @return size_t
 */
size_t strlen(const char *buf);

/**
 * @brief Uppercase version of character if it's a letter
 *
 * @param c
 * @return char
 */
char toupper(char character);

/**
 * @brief Copy null-terminated string to dest
 *
 * @param *dest Destination
 * @param *src Source
 */
void strcpy(char *dest, const char *src, size_t max_len);
