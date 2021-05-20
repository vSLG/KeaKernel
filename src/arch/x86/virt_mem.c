/**
 * @file virt_mem.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Virtual memory management
 * @version 0.1
 * @date 2021-05-16
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <kea/memory.h>
#include <kprintf.h>
#include <linker.h>
#include <memory.h>

#include "boot.h"
#include "phys_mem.h"
#include "virt_mem.h"

extern uintptr_t _kernel_end;
extern uintptr_t _kernel_start;

extern void load_directory(void *plm4t);

/**
 * Private declarations
 */

static table_t *kernel_pml4t;

void *virt_identity_alloc(table_t *pml4t, size_t size, mem_flags_t flags) {
    size = PAGE_ALIGN_UP(size);

    size_t         offset = 0;
    memory_block_t block  = phys_next_free_space(&offset, size);

    // Find next physical space that is not already mapped
    while (virt_to_phys(pml4t, block.start) && block.len)
        block = phys_next_free_space(&offset, size);

    // Means no physical space available for identity mapping
    if (!block.len)
        return 0;

    phys_set_used(block);

    if (flags & MEM_CLEAR)
        memset((void *) block.start, 0, block.len);

    return (void *) block.start;
}

uint8_t virt_map(table_t *      pml4t,
                 memory_block_t phys_block,
                 uintptr_t      virt_addr,
                 mem_flags_t    flags) {
    uint64_t addr, phys_addr;
    PAGE_ALIGN_BLOCK(phys_block);

    for (size_t i = 0; i < phys_block.len / PAGE_SZ; i++) {
        addr      = virt_addr + i * PAGE_SZ;
        phys_addr = phys_block.start + i * PAGE_SZ;

        page_entry_t *plm4t_entry = &pml4t->entries[pml4t_idx(addr)];
        table_t *     pdpt        = (void *) (plm4t_entry->addr * PAGE_SZ);

        if (!plm4t_entry->present) {
            pdpt = virt_identity_alloc(pml4t, PAGE_SZ, MEM_CLEAR);

            if (!pdpt)
                return 0;

            plm4t_entry->present = 1;
            plm4t_entry->wr      = 1;
            plm4t_entry->user    = 1;
            plm4t_entry->addr    = (uintptr_t) pdpt / PAGE_SZ;

            memory_block_t pdpt_block = {
                .len   = PAGE_SZ,
                .start = (uintptr_t) pdpt,
            };

            virt_map(pml4t, pdpt_block, pdpt_block.start, MEM_NONE);
        }

        page_entry_t *pdpt_entry = &pdpt->entries[pdpt_idx(addr)];
        table_t *     pdt        = (void *) (pdpt_entry->addr * PAGE_SZ);

        if (!pdpt_entry->present) {
            pdt = virt_identity_alloc(pml4t, PAGE_SZ, MEM_CLEAR);

            if (!pdt)
                return 0;

            pdpt_entry->present = 1;
            pdpt_entry->wr      = 1;
            pdpt_entry->user    = 1;
            pdpt_entry->addr    = (uintptr_t) pdt / PAGE_SZ;

            memory_block_t pdt_block = {
                .len   = PAGE_SZ,
                .start = (uintptr_t) pdt,
            };

            virt_map(pml4t, pdt_block, pdt_block.start, MEM_NONE);
        }

        page_entry_t *pdt_entry = &pdt->entries[pdt_idx(addr)];
        table_t *     pt        = (void *) (pdt_entry->addr * PAGE_SZ);

        if (!pdt_entry->present) {
            pt = virt_identity_alloc(pml4t, PAGE_SZ, MEM_CLEAR);

            if (!pt)
                return 0;

            pdt_entry->present = 1;
            pdt_entry->wr      = 1;
            pdt_entry->user    = 1;
            pdt_entry->addr    = (uintptr_t) pt / PAGE_SZ;

            memory_block_t pt_block = {
                .len   = PAGE_SZ,
                .start = (uintptr_t) pt,
            };

            virt_map(pml4t, pt_block, pt_block.start, MEM_NONE);
        }

        page_entry_t *pt_entry = &pt->entries[pt_idx(addr)];

        pt_entry->present = 1;
        pt_entry->wr      = 1;
        pt_entry->user    = flags & MEM_USER;
        pt_entry->addr    = phys_addr / PAGE_SZ;
    }

    // TODO: flush cr3?

    return 1;
}

/**
 * Public declarations
 */

uintptr_t virt_to_phys(table_t *pml4t, uintptr_t virt_addr) {
    page_entry_t *pml4t_entry = &pml4t->entries[pml4t_idx(virt_addr)];

    if (!pml4t_entry->present)
        return 0;

    table_t *     pdpt       = (void *) (pml4t_entry->addr * PAGE_SZ);
    page_entry_t *pdpt_entry = &pdpt->entries[pdpt_idx(virt_addr)];

    if (!pdpt_entry->present)
        return 0;

    if (pdpt_entry->huge)
        return (pdpt_entry->addr * P3_HUGE_PAGE_SZ) + (virt_addr & 0x3fffffff);

    table_t *     pdt       = (void *) (pdpt_entry->addr * PAGE_SZ);
    page_entry_t *pdt_entry = &pdt->entries[pdt_idx(virt_addr)];

    if (!pdt_entry->present)
        return 0;

    if (pdt_entry->huge)
        return (pdt_entry->addr * P2_HUGE_PAGE_SZ) + (virt_addr & 0x1fffff);

    table_t *     pt       = (void *) (pdt_entry->addr * PAGE_SZ);
    page_entry_t *pt_entry = &pt->entries[pt_idx(virt_addr)];

    if (!pt_entry->present)
        return 0;

    return (pt_entry->addr * PAGE_SZ) + (virt_addr & 0xfff);
}

void init_virt_mem(boot_info_t *boot_info) {
    // We can use real physical address since we're identity mapped for now
    memory_block_t pml4t_block = phys_alloc(sizeof(table_t));
    kernel_pml4t               = (void *) pml4t_block.start;

    memset(kernel_pml4t, 0, sizeof(table_t));
    virt_map(kernel_pml4t, pml4t_block, pml4t_block.start, MEM_NONE);

    memory_block_t kernel_block = {
        .start = (uintptr_t) &_kernel_start - KERNEL_VMA,
        .len   = (uintptr_t) &_kernel_end - (uintptr_t) &_kernel_start,
    };

    virt_map(kernel_pml4t, kernel_block, (uintptr_t) &_kernel_start, MEM_CLEAR);

    load_directory(kernel_pml4t);
}
