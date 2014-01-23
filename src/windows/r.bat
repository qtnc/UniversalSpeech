@echo off
del ..\..\obj\*.o
for %%i in (*.c) do gcc %%i -w -s -O3 -c -std=gnu99 -o ..\..\obj\%%~ni.o
for %%i in (..\*.c) do gcc %%i -w -s -O3 -Os -c -std=gnu99 -o ..\..\obj\%%~ni.o
gcc ..\..\obj\*.o main.def -shared -s -O3 -o ..\..\bin\UniversalSpeech.dll -Wl,--out-implib,..\..\lib\libUniversalSpeech.a -Wl,--add-stdcall-alias -Wl,--enable-stdcall-fixup -lsapi -lole32 -loleaut32 -luuid -lpsapi -lversion
