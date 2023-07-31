@ECHO OFF

@REM gcc main.c helpers.c -lmingwex -o pic_minidump.exe
x86_64-w64-mingw32-gcc main.c helpers.c -lmingwex -Wall -m64 -ffunction-sections -fno-asynchronous-unwind-tables -nostdlib -fno-ident -O2 -o dumper.exe -Wl,--no-seh


@REM cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tc *.c /link /OUT:implant.exe
@REM del *.obj