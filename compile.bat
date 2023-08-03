@ECHO OFF

x86_64-w64-mingw32-gcc main.c helpers.c -lmingwex -Wall -m64 -ffunction-sections -fno-asynchronous-unwind-tables -nostdlib -fno-ident -O2 -o pic_minidump.exe -Wl,--no-seh