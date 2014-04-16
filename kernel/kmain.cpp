#include <system.h>
#include <screen.h>

kernel::Screen *_screen;

namespace kernel {
Screen &getScreen() {
  return *_screen;
}

extern "C" {
void kmain(void)
{
  kernel::Screen screen;
  _screen = &screen;


  gdt_install();
  idt_install();
  irs_install();
  irq_install();
  timer_install();
  keyboard_install();
  __asm__ __volatile__ ("sti");

  getScreen().write("System booted!\n");

  for(;;);
  return;
}
}

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
ubyte inportb (uword _port) {
  ubyte rv;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
  return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (uword _port, ubyte _data) {
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

}
