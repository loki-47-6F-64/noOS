#include <system.h>
#include <screen.h>

/* dirty define */
#define ISR(x) extern void isr##x()
extern "C" {
ISR(0);
ISR(1);
ISR(2);
ISR(3);
ISR(4);
ISR(5);
ISR(6);
ISR(7);
ISR(8);
ISR(9);
ISR(10);
ISR(11);
ISR(12);
ISR(13);
ISR(14);
ISR(15);
ISR(16);
ISR(17);
ISR(18);
}

/* another dirty define */
#define SET_GATE(x) idt_set_gate(x, (unsigned) isr##x, 0x08, 0x8E)
void irs_install() {
  SET_GATE(0);
  SET_GATE(1);
  SET_GATE(2);
  SET_GATE(3);
  SET_GATE(4);
  SET_GATE(5);
  SET_GATE(6);
  SET_GATE(7);
  SET_GATE(8);
  SET_GATE(9);
  SET_GATE(10);
  SET_GATE(11);
  SET_GATE(12);
  SET_GATE(13);
  SET_GATE(14);
  SET_GATE(15);
  SET_GATE(16);
  SET_GATE(17);
  SET_GATE(18);
}

const byte *exception_message[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint Exception",
  "Into Detected Overflow Exception",
  "Out of Bounds Exception",
  "Invalid Opcode Exception",
  "No Coprocessor Exception",
  "Double Fault Exception",
  "Coprocessor Segment Overrun Exception",
  "Bad TSS Exception",
  "Segment Not Present Exception",
  "Stack Fault Exception",
  "General Protection Fault Exception",
  "Page Fault Exception",
  "Unknown Interrupt Exception",
  "Coprocessor Fault Exception",
  "Alignment Check Exception (486+)",
  "Machine Check Exception (Pentium/586+)"
};

extern "C" {
void fault_handler(regs *r) {
  /* Is this a fault whose number is from 0 to 18? */
  if (r->int_no < 19) {
      /* Display the description for the Exception that occurred.
      *  In this tutorial, we will simply halt the system using an
      *  infinite loop */
      kernel::getScreen().write(exception_message[r->int_no]);
      kernel::getScreen().write(" Exception. System Halted!");
      for (;;);
  }
}
}
