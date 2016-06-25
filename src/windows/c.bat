@echo off
del ..\..\obj\*.o
for %%i in (*.c) do gcc %%i -w -g -c -std=gnu99 -o ..\..\obj\%%~ni.o
for %%i in (..\*.c) do gcc %%i -w -g -Os -c -std=gnu99 -o ..\..\obj\%%~ni.o
gcc ..\..\obj\*.o main.def -shared -static-libgcc -g -o ..\..\bin\UniversalSpeech.dll -Wl,--out-implib,..\..\lib\libUniversalSpeech.a -Wl,--add-stdcall-alias -Wl,--enable-stdcall-fixup -lole32 -loleaut32 -luuid -lpsapi -lversion
