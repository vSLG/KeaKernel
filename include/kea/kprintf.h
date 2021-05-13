/**
 * @file kprintf.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Kernel logging, borrowed from KeaOS/2
 * @version 0.1
 * @date 2021-05-12
 *
 * Copyright (c) 2020 Iago, (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

typedef enum kprintf_color {
    KPRINTF_COLOR_INVALID,
    KPRINTF_COLOR_RED,
    KPRINTF_COLOR_GREEN,
    KPRINTF_COLOR_BLUE,
    KPRINTF_COLOR_CYAN,
    KPRINTF_COLOR_MAGENTA,
    KPRINTF_COLOR_YELLOW,
    KPRINTF_COLOR_WHITE,
    KPRINTF_COLOR_GRAY,
    KPRINTF_COLOR_BLACK,
} kprintf_color_t;

typedef struct kprintf_state {
    char current_char;

    kprintf_color_t foreground_color;
    kprintf_color_t background_color;
} kprintf_state_t;

typedef void (*kputchar_handler_t)(kprintf_state_t *);

/**
 * @brief Register a kputchar handler to print kernel messages through different
          media.
 *
 * @param handler kputchar handler.
 */
void register_kputchar(kputchar_handler_t handler);

/**
 * @brief Print a formatted message through different handlers.
 *
 * @param format Format string.
 * @param ... Arguments for using with the format string.
 */
void kprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));
