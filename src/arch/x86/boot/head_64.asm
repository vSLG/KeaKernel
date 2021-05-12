; Copyright (c) 2021 KeaOS/3 developers and contributors

; Entry code, we start from 32 bits without paging
[bits 32]
section .boot.text

; Defined in link_64.lds
extern boot_stack_top
extern kernel_stack

; main.c
extern _centry

global _entry

%define _KERNEL_VMA 0xffffffff80000000

%macro trl 2
    mov     %1, %2 - _KERNEL_VMA
%endmacro

_entry:
    mov     edi, eax  ; Multiboot2 magic
    mov     esi, ebx  ; Multiboot2 info pointer

    ; Initialize temporary stack
    lea     esp, boot_stack_top

    ; Clear EFLAGS
    push    dword 0
    popfd

    call    vga_clear

    cmp     edi, 0x36d76289  ; Check multiboot2 magic
    jne     .err

    push    0
    push    esi  ; Save info pointer for calling C function later

    call    common_init

    ; At this point we alrealy have GDT and paging set up
    jmp     _gdt64.code:_start64
.err:
    mov     esi, no_mb2_msg
    call    vga_println
    jmp     spin


common_init:
    call    cpuid_check
    call    extended_check
    call    long_mode_check
    call    setup_paging
    call    enter_long

    lgdt    [_gdt64.pointer]

    ret


cpuid_check:
    ; Copy EFLAGS to eax
    pushfd
    pop     eax
    mov     ecx, eax      ; Copy it to ecx for comparing later
    xor     eax, 1 << 21  ; Flip ID bit

    ; Try loading EFLAGS with flipped bit
    push    eax
    popfd
    pushfd
    pop     eax  ; Bit will be fliped if CPUID present

    ; Now restore old EFLAGS
    push    ecx
    popfd

    ; Test if bit was flipped
    xor     eax, ecx
    jz      .err
    ret
.err:
    mov     esi, no_cpuid_msg
    call    vga_println
    jmp     spin


extended_check:
    mov     eax, 0x80000000
    cpuid
    cmp     eax, 0x80000001  ; Extended unsupported if less
    jb      .err
    ret
.err:
    mov     esi, no_extended_msg
    call    vga_println
    jmp     spin


long_mode_check:
    mov     eax, 0x80000001
    cpuid
    test    edx, 1 << 29  ; Long mode bit is set on bit 29
    jz      .err
    ret
.err:
    mov     esi, no_long_mode_msg
    call    vga_println
    jmp     spin


setup_paging:
    ; Disable paging in any case
    mov     eax, cr0
    and     eax, 0x7fffffff
    mov     cr0, eax

    ; Clear tables
    trl     edi, pml4t
    xor     eax, eax
    mov     ecx, 3072
    rep     stosd

    ; Identity map and map to _KERNEL_VMA
    ; First map pdpt
    trl     edi, pml4t
    trl     ecx, pdpt
    or      ecx, 3                  ; Present + writable (change to user accessible?)
    mov     dword [edi], ecx        ; Identity map
    mov     dword [edi + 4088], ecx ; _KERNEL_VMA map

    ; Now map pdt
    trl     edi, pdpt
    trl     ecx, pdt
    or      ecx, 3
    mov     dword [edi], ecx
    mov     dword [edi + 4080], ecx

    ; And finally map page entries
    trl     edi, pdt
    mov     ecx, 512  ; Counter, map only 512 entries (1GiB total)
    mov     edx, 131  ; Entry attribute: present + writable + huge
.map:
    mov     dword [edi], edx
    add     edx, 0x200000    ; 2MiB
    add     edi, 8           ; Each entry is 8 bytes
    loop    .map
    ret


enter_long:
    ; Point control register 3 to our table
    trl     eax, pml4t
    mov     cr3, eax

    ; Enable PAE (bit 5) in cr4
    mov     eax, cr4
    or      eax, 1 << 5
    mov     cr4, eax

    ; Set Long Mode (bit 8) on msr
    mov     ecx, 0xc0000080
    rdmsr
    or      eax, 1 << 8
    wrmsr

    ; Enable paging and protected mode (bit 31 & 0)
    mov     eax, cr0
    or      eax, 1 << 31 | 1 << 0
    mov     cr0, eax

    ret


; Clears first page
vga_clear:
    push    eax
    mov     [vga_pos], dword 0
    mov     eax, 0
.loop:
    mov     [eax + 0xb8000], word 0
    add     eax, 1
    cmp     eax, 25*80*2
    jnz     .loop
    pop     eax
    ret


; Prints string pointed by esi (null terminated) and append new line
vga_println:
    push    eax
    push    ebx
    push    ecx
    push    edx

    call    vga_print

    ; Print newline
    mov     edx, 0
    mov     eax, [vga_pos]
    mov     ecx, 160
    div     ecx
    add     eax, 1
    mul     ecx,
    mov     [vga_pos], eax

    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

    ret


; Prints string pointed by esi (null terminated)
; clobber: ebx eax
vga_print:
.loop:
    lodsb
    cmp     al, 0
    je      .done
    call    vga_print_char
    jmp     .loop
.done:
    ret


; al: character to print
; clobber: ebx ah
vga_print_char:
    mov     ebx, [vga_pos]
    mov     ah,  0x0f
    mov     [ebx + 0xb8000], ax
    add     ebx, 2
    mov     [vga_pos], ebx
    ret


spin:
    hlt
    jmp spin


greeting:  db 'kk eae men',0
greeting2: db 'a sam eh brabo',0

no_cpuid_msg:     db 'cpuid instruction unsupported',0
no_extended_msg:  db 'Extended mode unsupported by CPU',0
no_long_mode_msg: db 'Long mode unsupported by CPU',0
no_mb2_msg:       db 'Error: not using multiboot2 protocol',0


section .boot.data

vga_pos: dd 0

_gdt64:                ; Global Descriptor Table (64-bit).
.null:  equ $ - _gdt64 ; The null descriptor.
    dw  0xFFFF         ; Limit (low).
    dw  0              ; Base (low).
    db  0              ; Base (middle)
    db  0              ; Access.
    db  1              ; Granularity.
    db  0              ; Base (high).
.code:  equ $ - _gdt64 ; The code descriptor.
    dw  0              ; Limit (low).
    dw  0              ; Base (low).
    db  0              ; Base (middle)
    db  10011010b      ; Access (exec/read).
    db  10101111b      ; Granularity, 64 bits flag, limit19:16.
    db  0              ; Base (high).
.data:  equ $ - _gdt64 ; The data descriptor.
    dw  0              ; Limit (low).
    dw  0              ; Base (low).
    db  0              ; Base (middle)
    db  10010010b      ; Access (read/write).
    db  00000000b      ; Granularity.
    db  0              ; Base (high).
.pointer:              ; The GDT-pointer.
    dw  $ - _gdt64 - 1 ; Limit.
    dq  _gdt64         ; Base.


section .bss

align 4096
pml4t:
    resb 4096
pdpt:
    resb 4096
pdt:
    resb 4096


[bits 64]
section .boot.text

_start64:
    lea     rax, _entry64
    jmp     rax


section .text

_entry64:
    cli  ; Clear interrupt flag

    ; Reload data segments
    mov     ax, _gdt64.data
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    ; Pop multiboot2 info ptr and setup new stack
    pop     rax
    mov     rsp, kernel_stack
    add     rsp, 1 << 12      ; Move to the top
    push    rax               ; Push mb2 ptr into stack again

    ; Clear identity map
    mov     qword [pml4t], 0
    mov     qword [pdpt], 0

    call    _centry
.spin:
    hlt
    jmp     .spin