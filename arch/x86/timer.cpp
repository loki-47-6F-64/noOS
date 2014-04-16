#include <system.h>
#include <screen.h>

namespace kernel {
dword timer_ticks = 0;

void timer_handler(regs *r) {
  ++timer_ticks;

  if(timer_ticks % 18 == 0) {
    kernel::getScreen().write("|- One second has passed -|\n");
    kernel::getScreen().refresh();
  }
}

void timer_install() {
  irq_install_handler(0, timer_handler);
}

}
