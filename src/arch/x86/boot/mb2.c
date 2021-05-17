/**
 * @file mb2.c
 * @author vslg (slgf@protonmail.ch)
 * @brief Implements mb2 info parser
 * @version 0.1
 * @date 2021-05-11
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#include <kea/string.h>
#include <kprintf.h>

#include "mb2.h"

#define trace_mb2(fmt, ...) kprintf("MB2: " fmt "\n", ##__VA_ARGS__)

/**
 * Private definitions
 */

void parse_mmap(mb2_tag_mmap_t *tag, boot_info_t *boot_info) {
    mb2_mmap_entry_t *map = tag->entries;
    boot_info->mmap_count = 0;

    while ((void *) map < (void *) tag + tag->tag.size) {
        boot_info->mmaps[boot_info->mmap_count].type    = map->type;
        boot_info->mmaps[boot_info->mmap_count].len     = map->len;
        boot_info->mmaps[boot_info->mmap_count++].start = map->addr;
        map = (void *) map + tag->entry_size;
    }
}

/**
 * Public definitions
 */

void init_mb2(void *mbi, boot_info_t *boot_info) {
    strcpy(boot_info->bootloader_name, "no-name", BOOTLOADER_NAME_LEN);
    strcpy(boot_info->cmdline, "none", CMDLINE_LEN);

    void *     header_ptr = mbi + 8;
    mb2_tag_t *tag        = header_ptr;

    while (tag->type != MB2_TAG_END) {
        trace_mb2("found tag at %p, type %d", (void *) tag, tag->type);

        switch (tag->type) {
            case MB2_TAG_BOOTLOADER_NAME:
                strcpy(boot_info->bootloader_name,
                       ((mb2_tag_str_t *) tag)->str,
                       BOOTLOADER_NAME_LEN);
                break;

            case MB2_TAG_CMDLINE:
                strcpy(boot_info->cmdline,
                       ((mb2_tag_str_t *) tag)->str,
                       CMDLINE_LEN);
                break;
            case MB2_TAG_MMU:
                parse_mmap((mb2_tag_mmap_t *) tag, boot_info);
                break;
            default:
                break;
        }

        tag = (mb2_tag_t *) ((void *) tag + ((tag->size + 7) & ~7));
    }
}
