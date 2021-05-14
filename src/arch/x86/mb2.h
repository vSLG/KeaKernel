/**
 * @file mb2.h
 * @author vslg (slgf@protonmail.ch)
 * @brief Multiboot2 related declarations
 * @version 0.1
 * @date 2021-05-13
 *
 * Copyright (c) 2021 vslg & KeaOS/3 developers
 *
 */

#pragma once

#include <stdint.h>

#include <kea/boot.h>
#include <kea/mem.h>
#include <linker.h>

typedef enum mb2_tag_type {
    MB2_TAG_END             = 0,
    MB2_TAG_CMDLINE         = 1,
    MB2_TAG_BOOTLOADER_NAME = 2,
    MB2_TAG_MMU             = 6,
    MB2_TAG_FB              = 8,
} mb2_tag_type_t;

/**
 * @struct mb2_mmap_entry
 *
 * @brief Mapped memory entry in mb2_tag_mmap.
 */
typedef struct mb2_mmap_entry {
    uint64_t      addr; ///< Address of mapped memory
    uint64_t      len;  ///< Length of mapped memory
    memory_type_t type; ///< Type of mapped memory
    uint32_t      zero;
} PACKED mb2_mmap_entry_t;

/**
 * @struct mb2_tag
 *
 * @brief MB2 tag generic header. Every MB2 tag contains this header.
 */
typedef struct mb2_tag {
    mb2_tag_type_t type; ///< Type of the tag
    uint32_t       size; ///< Size of the tag header
} PACKED mb2_tag_t;

/**
 * @struct mb2_tag_mmap
 *
 * @brief Memory map MB2 tag, MB2_TAG_TYPE_MMAP
 */
typedef struct mb2_tag_mmap {
    mb2_tag_t tag;
    uint32_t entry_size; ///< Size of this entry, guaranteed to be multiple of 8
    uint32_t entry_version;          ///< Version of the entry
    struct mb2_mmap_entry entries[]; ///< Array of mb2_mmap_entry entries
} PACKED mb2_tag_mmap_t;

/**
 * @struct mb2_tag_str
 *
 * @brief Multiboot2 string tag
 */
typedef struct mb2_tag_str {
    mb2_tag_t tag;
    char      str[]; ///< Null-terminated UTF-8 string
} PACKED mb2_tag_str_t;

/**
 * @brief Initialises mbi info into boot info
 *
 * @param *mbi Pointer to Multiboot2 info
 * @param *boot_info Boot info to be filled
 */
void init_mb2(void *mbi, boot_info_t *boot_info);
