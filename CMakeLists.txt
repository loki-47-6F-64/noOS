cmake_minimum_required (VERSION 2.6)

#################################################################
#define macros
function(sub_dir IN DIR FILES)
  foreach(_file ${FILES})
    set(result ${result} "${DIR}${_file}")
  endforeach(_file)
  set(${IN} ${result} PARENT_SCOPE)
  message("${result}")
endfunction(sub_dir)

#################################################################
#include compiler flags
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

enable_language(ASM_NASM)
project (Kernel)


#################################################################
#include libraries

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -ffreestanding -fno-rtti -fno-exceptions -nostdlib -nostdinc -fno-stack-protector -std=c++11")

SET (CMAKE_EXE_LINKER_FLAGS "-m elf_i386 -T${CMAKE_CURRENT_LIST_DIR}/link.ld")
SET (CMAKE_ASM_NASM_OBJECT_FORMAT "elf")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

##################################################################
#Source files
FILE (GLOB_RECURSE kernel_C_SOURCES "kernel/*.c")
FILE (GLOB_RECURSE kernel_CPP_SOURCES "kernel/*.cpp")
FILE (GLOB_RECURSE kernel_ASM_SOURCES "arch/x86/asm/*.*asm")
FILE (GLOB_RECURSE kernel_ARCH_C_SOURCES "arch/x86/*.c")
FILE (GLOB_RECURSE kernel_ARCH_CPP_SOURCES "arch/x86/*cpp")


##################################################################
#Include dirs
set(INCLUDE_DIR include/)
set(INCLUDE_COMMON_DIR ${INCLUDE_DIR}/common/)
set(INCLUDE_KERNEL_DIR ${INCLUDE_DIR}/kernel/)
set(INCLUDE_ARCH_X86_DIR ${INCLUDE_DIR}/arch/x86/)

include_directories(
  ${INCLUDE_DIR}
  ${INCLUDE_COMMON_DIR}
  ${INCLUDE_KERNEL_DIR}
  ${INCLUDE_ARCH_X86_DIR}
)

###################################################################
#Dependencies

ADD_LIBRARY (kernel.img STATIC
            ${kernel_C_SOURCES} 
            ${kernel_CPP_SOURCES} 
            ${kernel_ARCH_C_SOURCES} 
            ${kernel_ARCH_CPP_SOURCES} 
            ${kernel_ASM_SOURCES})

ADD_CUSTOM_COMMAND (TARGET kernel.img
    POST_BUILD
    COMMAND ld -m elf_i386 -T${CMAKE_CURRENT_LIST_DIR}/link.ld -o kernel.img ${CMAKE_BINARY_DIR}/libkernel.img.a
)
