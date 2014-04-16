#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include <type.h>
namespace common { namespace parse {

void  to_string(const byte *buf, dword value);
dword to_dword(const byte *value);
}}

#endif
