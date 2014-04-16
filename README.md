noOS
====
Created a small kernel that prints "A second has passed" every second.

Compile:
cmake -DCMAKE_BUILD_TYPE=debug
make

Run:
qemu-system-i386 -kernel kernel.img
