; Copyright (c) 2021 LabOS developers and contributors

section .mb2

%define MB2_MAGIC       0xE85250D6
%define MB2_ARCH        0x00000000
%define MB2_HEADERLEN   mb2_end - mb2_start
%define MB2_CHECKSUM    -(MB2_MAGIC+MB2_ARCH+MB2_HEADERLEN)

extern _text_start_
extern _data_end_
extern _bss_end_
extern _entry

align 8
mb2_start:

dd  MB2_MAGIC
dd  MB2_ARCH
dd  MB2_HEADERLEN
dd  MB2_CHECKSUM

align 8
mb2_entry_tag:
    dw  3
    dw  0
    dd  .tag_end - mb2_entry_tag
    dd  __entry
.tag_end:

align 8
mb2_end_tag:
    dw  0
    dw  0
    dd  .tag_end - mb2_end_tag
.tag_end:

mb2_end:

section .mb2.text
__entry:
    cmp     eax, 0x36d76289 ; Check multiboot2 magic
    je      _entry
    mov     al, "0"         ; If not booted from mb2, show error
    mov     dword [0xb8000], 0x4f524f45
    mov     dword [0xb8004], 0x4f3a4f52
    mov     dword [0xb8008], 0x4f204f20
    mov     byte  [0xb800a], al
    hlt
