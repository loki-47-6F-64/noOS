;;kernel.asm

;nasm directive - 32 bit
bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start
global gdt_flush
global idt_load

extern idtp
extern gp
extern kmain          ;kmain is defined in the cpp file

start:
  cli       ;block interrupts
  call kmain
  hlt     ;halt the CPU

gdt_flush:
  lgdt [gp] ;load the register with our '_gp' wich is a special pointer
  mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:end_gdt_flush   ; 0x08 is the offset to our code segment: Far jump!
end_gdt_flush:
  ret

idt_load:
  lidt[idtp]
  ret
