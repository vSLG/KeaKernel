; Copyright (c) 2021 KeaOS/3 developers and contributors

[bits 64]

extern interrupt_handler

; Non-exception interrupt handler macro
%macro isr 1
    global  __int_%1
__int_%1:
    push    byte 0
    push    byte %1
    jmp     common_stub
%endmacro

; Exception interrupt handler macro
%macro isr_e 1
    global  __int_%1
__int_%1:
    push    byte %1
    jmp     common_stub
%endmacro

; Generic IRQ handler macro
%macro irq 2
    global  __int_%2
__int_%2:
    push    byte %1
    push    byte %2
    jmp     common_stub
%endmacro

%macro int_name 1
    dq      __int_%1
%endmacro

%macro pushall 0
    push    r15
    push    r14
    push    r13
    push    r12
    push    r11
    push    r10
    push    r9
    push    r8
    push    rbp
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    push    rbx
    push    rax
%endmacro

%macro popall 0
    pop     rax
    pop     rbx
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     rbp
    pop     r8
    pop     r9
    pop     r10
    pop     r11
    pop     r12
    pop     r13
    pop     r14
    pop     r15
%endmacro


common_stub:
    cld

    pushall

    mov     rdi, rsp
    call    interrupt_handler
    mov     rsp, rax

    popall

    add     rsp, 8 * 2 ; Clean err and int num

    sti

    iretq


isr     0  ; Divide by 0 exception
isr     1  ; Debug exception
isr     2  ; Non maskable interrupt exception
isr     3  ; Int 3 exception (Breakpoint)
isr     4  ; INTO exception
isr     5  ; Out of bounds exception
isr     6  ; Invalid Opcode Exception
isr     7  ; Coprocessor Not Available Exception
isr_e   8  ; Double Fault Exception (With Error Code!)
isr     9  ; Coprocessor Segment Overrun Exception
isr_e   10 ; Bad TSS Exception (With Error Code!)
isr_e   11 ; Segment Not Present Exception (With Error Code!)
isr_e   12 ; Stack Fault Exception (With Error Code!)
isr_e   13 ; General Protection Fault Exception (With Error Code!)
isr_e   14 ; Page Fault Exception (With Error Code!)
isr     15 ; Reserved Exception
isr     16 ; Floating Point Exception
isr     17 ; Alignment Check Exception
isr     18 ; Machine Check Exception

; 19-31: Reserved
isr     19
isr     20
isr     21
isr     22
isr     23
isr     24
isr     25
isr     26
isr     27
isr     28
isr     29
isr     30
isr     31

irq     0,  32 ; System timer
irq     1,  33 ; PS/2 Keyboard
irq     2,  34 ; Slave PIC
irq     3,  35 ; Serial port 2 controller
irq     4,  36 ; Serial port 1 controller
irq     5,  37 ; Parallel port 2 and 3 or sound card
irq     6,  38 ; Floppy disk controllerr
irq     7,  39 ; Parallel port 1

irq     8,  40 ; RTC
irq     9,  41 ; Intel ACPI
irq     10, 42 ; Free
irq     11, 43 ; Free
irq     12, 44 ; PS/2 Mouse
irq     13, 45 ; CPU co-processor or FPU or inter processor interrupt
irq     14, 46 ; Primary ATA
irq     15, 47 ; Secondary ATA


section .data

global interrupt_array

interrupt_array:
    int_name 1
    int_name 2
    int_name 3
    int_name 4
    int_name 5
    int_name 6
    int_name 7
    int_name 8
    int_name 9
    int_name 10
    int_name 11
    int_name 12
    int_name 13
    int_name 14
    int_name 15
    int_name 16
    int_name 17
    int_name 18
    int_name 19
    int_name 20
    int_name 21
    int_name 22
    int_name 23
    int_name 24
    int_name 25
    int_name 26
    int_name 27
    int_name 28
    int_name 29
    int_name 30
    int_name 31
    int_name 32
    int_name 33
    int_name 34
    int_name 35
    int_name 36
    int_name 37
    int_name 38
    int_name 39
    int_name 40
    int_name 41
    int_name 42
    int_name 43
    int_name 44
    int_name 45
    int_name 46
    int_name 47
