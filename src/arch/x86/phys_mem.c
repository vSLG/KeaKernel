/**
 * @file phys_mem.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Physical memory management
 * @version 0.1
 * @date 2021-05-16
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <stdint.h>

#include <kea/memory.h>
#include <memory.h>

#include "phys_mem.h"
#include "virt_mem.h"

/**
 * Private declarations
 */

#define MEM_BITMAP_SZ 1024 * 4

static uint8_t mem_bitmap[MEM_BITMAP_SZ] = {
    0}; // Can keep track up to 128GiB of RAM
static size_t total_mem = 0;
static size_t used_mem  = 0;
static size_t last_page = 0;

uint8_t phys_page_used(uintptr_t addr) {
    uint64_t page = addr / PAGE_SZ;
    return mem_bitmap[page / 8] & (1 << (page % 8));
}

uint8_t phys_used(memory_block_t block) {
    // TODO: align
    for (size_t i = 0; i < block.len / PAGE_SZ; i++)
        if (phys_page_used(block.start + i * PAGE_SZ))
            return 1;

    return 0;
}

void phys_page_set_free(uintptr_t addr) {
    uint64_t page = addr / PAGE_SZ;

    if (page < last_page)
        last_page = page;

    mem_bitmap[page / 8] &= ~(1 << (page % 8));
}

void phys_page_set_used(uintptr_t addr) {
    uint64_t page = addr / PAGE_SZ;

    if (page == last_page)
        last_page++;

    mem_bitmap[page / 8] |= 1 << (page % 8);
}

void phys_set_free(memory_block_t block) {
    // TODO: align
    uintptr_t addr;

    for (size_t i = 0; i < block.len / PAGE_SZ; i++) {
        addr = block.start + i * PAGE_SZ;
        if (phys_page_used(addr)) {
            phys_page_set_free(addr);
            used_mem -= PAGE_SZ;
        }
    }
}

size_t phys_total_mem() {
    return total_mem;
}

size_t phys_used_mem() {
    return used_mem;
}

/**
 * Public declarations
 */

void init_phys_mem(boot_info_t *boot_info) {
    memset(&mem_bitmap, 0xF, MEM_BITMAP_SZ);

    for (size_t i = 0; i < boot_info->mmap_count; i++) {
        memory_block_t block = boot_info->mmaps[i];

        if (block.type == MEM_AVAILABLE) {
            phys_set_free(block);
            total_mem += block.len;
        }
    }

    used_mem = 0;

    boot_info->plat.used_mem  = &phys_used_mem;
    boot_info->plat.total_mem = &phys_total_mem;
}

memory_block_t phys_alloc(size_t size) {
    // TODO: align

    memory_block_t block;
    block.len = size;

    for (size_t i = last_page; i < (total_mem - size) / PAGE_SZ; i++) {
        block.start = i * PAGE_SZ;

        if (!phys_used(block)) {
            phys_set_used(block);
            return block;
        }
    }

    block.len = 0;
    return block;
}

memory_block_t phys_next_free_space(size_t *offset, size_t size) {
    memory_block_t block;
    block.len = size;

    for (; *offset < (total_mem - size) / PAGE_SZ; (*offset)++) {
        block.start = *offset * PAGE_SZ;
        if (!phys_used(block))
            return block;
    }

    block.len = 0;
    return block;
}

void phys_set_used(memory_block_t block) {
    // TODO: align
    uintptr_t addr;

    for (size_t i = 0; i < block.len / PAGE_SZ; i++) {
        addr = block.start + i * PAGE_SZ;
        if (!phys_page_used(addr)) {
            phys_page_set_used(addr);
            used_mem += PAGE_SZ;
        }
    }
}
