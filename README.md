# pic_minidump

### Shellcode Transform:
```
for i in $(objdump -d dumper.exe | grep "^ " | cut -f 2); do echo -n ",0x$i"; done; echo
```