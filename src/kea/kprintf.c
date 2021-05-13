/**
 * @file kprintf.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Kernel logging, borrowed from KeaOS/2
 * @version 0.1
 * @date 2021-05-12
 *
 * Copyright (c) 2020 Iago, (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <kea/kprintf.h>
#include <kea/stdlib.h>
#include <kea/string.h>

static struct {
    kputchar_handler_t handlers[4];
    int                n_handlers;
    kprintf_state_t    state;
} kp_info = {0};

static char formats_l[] = "xdspcXpoib%";
static char flags_l[]   = "0-+ #";

static void kputchar(char chr);

void register_kputchar(kputchar_handler_t handler) {
    kp_info.handlers[kp_info.n_handlers++] = handler;
}

void kprintf(const char *format, ...) {
    if (kp_info.state.foreground_color == KPRINTF_COLOR_INVALID) {
        kp_info.state.foreground_color = KPRINTF_COLOR_GRAY;
    }
    if (kp_info.state.background_color == KPRINTF_COLOR_INVALID) {
        kp_info.state.background_color = KPRINTF_COLOR_BLACK;
    }

    char buf[512];

    va_list ap;
    va_start(ap, format);

    int parse_format = 1;

    while (*format != '\0') {
        if (*format == '%' && parse_format) {

            // Check for valid format specifier
            uint32_t i, j;
            for (i = 1; format[i] != '\0'; i++) {
                for (j = 0; formats_l[j] != '\0'; j++) {
                    if (format[i] == formats_l[j])
                        break;
                }
                // A format specifier was found
                if (formats_l[j] != '\0')
                    break;
            }

            char format_c = format[i];

            // No format specifier found
            if (format_c == '\0') {
                parse_format = 0;
                continue;
            }

            // Flags bitfield
            // "0-+ #"
            // Bit 1: 0         Left-pad number with 0s by width
            // Bit 2: -         Left-justify by width
            // Bit 3: +         Force sign for positive numbers
            // Bit 4: <space>   If no sign, put a space
            // Bit 5: #         Precede xXo value with 0x, 0X or 0
            uint8_t flags = 0;
            int     width = 0;

            // Flags or width present
            // TODO Add length prefix parsing
            if (i > 1) {

                // Which flag index to check out next
                // Used by the width checking block to skip the flags
                uint32_t f_idx = 1;

                // Check all flags
                for (; f_idx < i; f_idx++) {
                    uint32_t k;
                    for (k = 0; flags_l[k] != '\0'; k++) {
                        if (format[f_idx] == flags_l[k])
                            break;
                    }

                    // There is a flag
                    if (flags_l[k] != '\0') {
                        flags |= 1 << k;
                    }
                    // Probably a width specifier and we should break
                    else {
                        break;
                    }
                }

                // Specifies the end of the width specifier
                uint32_t wid_idx = f_idx;

                // Easy route: the width is an argument
                if (format[wid_idx] == '*') {
                    width = va_arg(ap, int);
                    wid_idx++;
                }
                // Not so easy route: need to convert the number to an uint
                else {
                    for (; format[wid_idx] >= '0' && format[wid_idx] <= '9';
                         wid_idx++) {
                        width *= 10;
                        width += format[wid_idx] - '0';
                    }
                }
            }

            char *  str;
            uint8_t base;
            switch (format_c) {
                case 'd':
                case 'i':
                    base = 10;
                    break;
                case 'X':
                case 'x':
                case 'p':
                    base = 16;
                    break;
                case 'o':
                    base = 8;
                    break;
                case 'b':
                    base = 2;
                    break;
                default:
                    base = 0;
                    break;
            }
            switch (format_c) {
                case 'd':
                case 'i':
                case 'x':
                case 'X':
                case 'o':
                case 'p':
                case 'b':
                    str = itoa(va_arg(ap, uint32_t), &buf[3], base);
                    break;
                case 's':
                    str = va_arg(ap, char *);
                    break;
                case 'c':
                    buf[0] = va_arg(ap, int);
                    buf[1] = '\0';
                    str    = buf;
                    break;
                default:
                    str = "?";
            }
            if (format_c == 'p' || (flags & 16) == 16) {
                switch (format_c) {
                    case 'p':
                    case 'x':
                    case 'X':
                        buf[1] = '0';
                        buf[2] = 'x';
                        str    = &buf[1];
                        break;
                    case 'o':
                        buf[2] = '0';
                        str    = &buf[2];
                        break;
                    default:
                        break;
                }
            }

            if (format_c == 'X') {
                for (uint32_t i = 0; str[i] != '\0'; i++) {
                    str[i] = toupper(str[i]);
                }
            }

            if ((flags & 2) == 2) {
                do {
                    kputchar(*str++);
                } while (*str != '\0');
            }

            if (width > 0) {
                width -= strlen(str);
            }
            if (width < 0) {
                width = 0;
            }

            for (; width > 0; width--) {
                // Use 0s
                if ((flags & 1) == 1) {
                    kputchar('0');
                } else {
                    kputchar(' ');
                }
            }

            if ((flags & 2) == 0) {
                do {
                    kputchar(*str++);
                } while (*str != '\0');
            }

            format += i;
        } else {
            if (!parse_format)
                parse_format = 1;
            kputchar(*format);
        }
        format++;
    }

    va_end(ap);
}

static void kputchar(char chr) {
    kp_info.state.current_char = chr;
    for (int i = 0; i < kp_info.n_handlers; i++) {
        kp_info.handlers[i](&kp_info.state);
    }
}
