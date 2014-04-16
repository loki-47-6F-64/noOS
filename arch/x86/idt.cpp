#include <system.h>

struct idt_entry {
  uword base_lo;
  uword sel;
  ubyte alwaysNULL;
  ubyte flags;
  uword base_hi;
} __attribute__((packed));

struct idt_ptr {
  uword limit;
  udword base;
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_entry idt[256];

extern "C" {
struct idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();
}

void idt_set_gate(ubyte num, uqword base, uword sel, ubyte flags) {
  idt[num].base_lo = (base & 0xFFFF);
  idt[num].base_hi = (base >> 16) & 0xFFFF;

  idt[num].sel   = sel;
  idt[num].flags = flags;
}

void idt_install() {
  /* Sets the special IDT pointer up, just like in 'gdt.c' */
  idtp.limit = (sizeof (idt_entry) * 256) - 1;
  idtp.base = (udword)&idt;

  /* Clear out the entire IDT, initializing it to zeros */
  common::memset(idt, 0, 256);

  /* Add any new ISRs to the IDT here using idt_set_gate */

  /* Points the processor's internal register to the new IDT */
  idt_load();
}
