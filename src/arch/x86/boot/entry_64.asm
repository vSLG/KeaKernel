; Copyright (c) 2021 LabOS developers and contributors

; Entry code, we start from 32 bits without paging
[bits 32]
section .mb2.text

global _entry

_entry:
    mov dword [0xb8000], 0x2f4b2f4f
    hlt
    jmp _entry
