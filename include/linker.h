/**
 * @file linker.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Defines linker quirks
 * @date 2021-05-11
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#define KERNEL_VMA 0xffffffff80000000

#define PACKED     __attribute__((__packed__))
#define SECTION(s) __attribute__((__section__(sec)))
#define CALIGN(n)  __attribute__((__aligned__(n)))
#define NORETURN   __attribute__((noreturn))
