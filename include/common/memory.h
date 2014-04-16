#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <type.h>

namespace common {

template<class pointer>
void memset(pointer&& dest, byte value, dword elements) {
  byte *tmp_p = (byte*)&(*dest);
  for(dword x = 0; x < elements*sizeof(*dest); ++x) {
    tmp_p[x] = value;
  }
}

}
#endif
