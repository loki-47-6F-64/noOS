#ifndef KERNEL_SYSTEM_H
#define KERNEL_SYSTEM_H

#include <type.h>
#include <memory.h>

struct regs {
  udword gs, fs, es, ds;      /* pushed the segs last */
  udword edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
  udword int_no, err_code;    /* our 'push byte #' and ecodes do this */
  udword eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

void gdt_set_gate(dword num, uqword base, uqword limit, ubyte access, ubyte gran);
void gdt_install();

void idt_set_gate(ubyte num, uqword base, uword sel, ubyte flags);
void idt_install();


void irs_install();

void irq_install();
void irq_install_handler(int irq, void (*handler)(regs *r));
void irq_uninstall_handler(int irq);

void timer_install();

ubyte inportb (uword _port);
void outportb (uword _port, ubyte _data);

#endif
