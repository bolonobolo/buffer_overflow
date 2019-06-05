This program should be compiled using the MinGW C compiler for Windows.<br>

First compile the dll file:<br>
```gcc.exe -c basicfunc.c```<br>
```gcc.exe -shared -o basicfunc.dll -Wl,--out-implib=libbasicfunc.a -Wl,--image-base=0x62500000 basicfunc.o```<br>

Now compile the main program:<br>
```gcc.exe evilserv.c -o evilserv.exe -lws2_32 ./libbasicfunc.a```
