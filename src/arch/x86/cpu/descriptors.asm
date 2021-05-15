; Copyright (c) 2021 KeaOS/3 developers and contributors

[bits 64]

global gdt_flush

gdt_flush:
    cli
    lgdt    [rdi]

    ; cdecl
    push    rbp
    mov     rbp, rsp

    ; Use iretq trick to come back
    push    qword 0x10    ; Kernel data offset (ss)
    push    rbp           ; New rsp
    pushfq                ; RFLAGS
    push    qword 0x08    ; Kernel code offset (cs)
    push    .flush        ; New rip

    iretq  ; Jump to .flush :)
.flush:
    ; We don't need to set rsp, because it was already set by iretq
    pop     rbp ; Saved

    ; Reload segment registers just in case
    mov     ax, 0x10  ; Offset to kernel data
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    mov     ax, 0x28  ; Offset to TSS
    ltr     ax

    ret
