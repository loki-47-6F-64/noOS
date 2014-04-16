;nasm directive - 32 bit
bits 32

extern fault_handler

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

; Divide by zero exception
isr0:
  cli
  push byte 0
  push byte 0
  jmp isr_common_stub

; Debug exception
isr1:
  cli
  push byte 0
  push byte 1
  jmp isr_common_stub

; Non Maskable Interrupt Exception
isr2:
  cli
  push byte 0
  push byte 2
  jmp isr_common_stub

; Breakpoint Exception
isr3:
  cli
  push byte 0
  push byte 3
  jmp isr_common_stub

; Into Detected Overflow Exception
isr4:
  cli
  push byte 0
  push byte 4
  jmp isr_common_stub

; Out of Bounds Exceptio
isr5:
  cli
  push byte 0
  push byte 5
  jmp isr_common_stub

; Invalid Opcode Exception
isr6:
  cli
  push byte 0
  push byte 6
  jmp isr_common_stub

; No Coprocessor Exception
isr7:
  cli
  push byte 0
  push byte 7
  jmp isr_common_stub

; Double Fault Exception
isr8:
  cli
  push byte 8
  jmp isr_common_stub

; Coprocessor Segment Overrun Exception
isr9:
  cli
  push byte 0
  push byte 9
  jmp isr_common_stub

; Bad TSS Exception
isr10:
  cli
  push byte 10
  jmp isr_common_stub

; Segment Not Present Exception
isr11:
  cli
  push byte 11
  jmp isr_common_stub

; Stack Fault Exception
isr12:
  cli
  push byte 12
  jmp isr_common_stub

; General Protection Fault Exception
isr13:
  cli
  push byte 13
  jmp isr_common_stub

; Page Fault Exception
isr14:
  cli
  push byte 14
  jmp isr_common_stub

; Unknown Interrupt Exception
isr15:
  cli
  push byte 0
  push byte 15
  jmp isr_common_stub

; Coprocessor Fault Exception
isr16:
  cli
  push byte 0
  push byte 16
  jmp isr_common_stub

; Alignment Check Exception (486+)
isr17:
  cli
  push byte 0
  push byte 17
  jmp isr_common_stub

; Machine Check Exception (Pentium/586+)
isr18:
  cli
  push byte 0
  push byte 18
  jmp isr_common_stub
