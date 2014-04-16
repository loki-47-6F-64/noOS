#include <system.h>

/* These are own ISRs that point to our special IRQ handler
*  instead of the regular 'fault_handler' function */

/* dirty define */
#define IRQ(x) extern void irq##x()
extern "C" {
IRQ(0);
IRQ(1);
IRQ(2);
IRQ(3);
IRQ(4);
IRQ(5);
IRQ(6);
IRQ(7);
IRQ(8);
IRQ(9);
IRQ(10);
IRQ(11);
IRQ(12);
IRQ(13);
IRQ(14);
IRQ(15);
}

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(regs *r)) {
  irq_routines[irq] = (void*)handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq) {
  irq_routines[irq] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
void irq_remap(void)
{
  outportb(0x20, 0x11);
  outportb(0xA0, 0x11);
  outportb(0x21, 0x20);
  outportb(0xA1, 0x28);
  outportb(0x21, 0x04);
  outportb(0xA1, 0x02);
  outportb(0x21, 0x01);
  outportb(0xA1, 0x01);
  outportb(0x21, 0x0);
  outportb(0xA1, 0x0);
}

/* another dirty define */
#define SET_GATE(x) idt_set_gate(x + 32, (unsigned)irq##x, 0x08, 0x8E)
/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_install() {
  irq_remap();

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
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
extern "C" {
void irq_handler(regs *r) {
  /* This is a blank function pointer */
  void (*handler)(regs *r);

  /* Find out if we have a custom handler to run for this
  *  IRQ, and then finally, run it */
  handler = (decltype(handler))irq_routines[r->int_no - 32];
  if (handler) {
      handler(r);
  }

  /* If the IDT entry that was invoked was greater than 40
  *  (meaning IRQ8 - 15), then we need to send an EOI to
  *  the slave controller */
  if (r->int_no >= 40) {
      outportb(0xA0, 0x20);
  }

  /* In either case, we need to send an EOI to the master
  *  interrupt controller too */
  outportb(0x20, 0x20);
}
}
