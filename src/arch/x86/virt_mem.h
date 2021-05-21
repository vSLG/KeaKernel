// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file virt_mem.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Virtual memory management
 * @version 0.1
 * @date 2021-05-16
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <boot.h>
#include <kea/memory.h>

#include <linker.h>
#include <memory.h>

#define pml4t_idx(addr) (addr >> 39) & 0x1ff
#define pdpt_idx(addr)  (addr >> 30) & 0x1ff
#define pdt_idx(addr)   (addr >> 21) & 0x1ff
#define pt_idx(addr)    (addr >> 12) & 0x1ff

#define PAGE_SZ         0x1000
#define P2_HUGE_PAGE_SZ 0x200000
#define P3_HUGE_PAGE_SZ 0x40000000

#define PAGE_ALIGN_DOWN(addr) addr - (addr % PAGE_SZ)
#define PAGE_ALIGN_UP(addr) \
    addr % PAGE_SZ == 0 ? addr : addr + PAGE_SZ - (addr % PAGE_SZ)
#define PAGE_ALIGN_BLOCK(block)             \
    block.len   = PAGE_ALIGN_UP(block.len); \
    block.start = PAGE_ALIGN_DOWN(block.start)

/**
 * @struct page_entry
 *
 * @brief Represents a page entry for page tables
 */
typedef struct page_entry {
    uint8_t  present : 1;       ///< Page is currently in memory
    uint8_t  wr : 1;            ///< Write permission
    uint8_t  user : 1;          ///< If not set, only kernel can access
    uint8_t  wtc : 1;           ///< Write through cache
    uint8_t  cache_disable : 1; ///< Disable caching
    uint8_t  accessed : 1;      ///< Set by CPU automatically
    uint8_t  dirty : 1;         ///< Set by CPU automatically
    uint8_t  huge : 1;          ///< Must be 0 in P1 & P4. P2 = 2MiO, P3 = 1GiO
    uint8_t  global : 1;        ///< Don't flush page on addr space switch
    uint8_t  avail1 : 3;        ///< Can be used freely
    uint64_t addr : 40;         ///< Actual address of next entry/phys mem
    uint16_t avail2 : 11;       ///< Can be used freely
    uint8_t  nxe : 1;           ///< Forbid code execution. Requires EFER.NXE
} PACKED page_entry_t;

/**
 * @struct table
 *
 * @brief Represents a table
 */
typedef struct table {
    page_entry_t entries[512];
} PACKED table_t;

/**
 * @brief Translates virtual address to physical address
 *
 * @param addr_space Pml4t
 * @param virt_addr
 * @return uintptr_t physical address
 */
uintptr_t virt_to_phys(table_t *addr_space, uintptr_t virt_addr);

/**
 * @brief Initializes virtual memory, remaps kernel
 *
 */
void init_virt_mem(boot_info_t *boot_info);
