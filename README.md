# pic_minidump
Shellcode that executes MiniDumpWriteDump. Update pid variable in main.c with pid of process you want to dump, then compile.
Transform into shellcode with:
```
for i in $(objdump -d pic_minidump.exe | grep "^ " | cut -f 2); do echo -n ",0x$i"; done; echo
```
