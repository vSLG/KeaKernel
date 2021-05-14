; Copyright (c) 2021 KeaOS/3 developers and contributors

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

;align 8
;mb2_entry_tag:
;    dw  3
;    dw  0
;    dd  .tag_end - mb2_entry_tag
;    dd  _entry
;.tag_end:

align 8
mb2_end_tag:
    dw  0
    dw  0
    dd  .tag_end - mb2_end_tag
.tag_end:

mb2_end:
