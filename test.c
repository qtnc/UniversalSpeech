#include<stdio.h>
#include "include/UniversalSpeech.h"
#include<windows.h>

int main (int argc, char** argv) {
speechSay(L"Hello world", 1);
HANDLE dll = LoadLibrary("UniversalSpeech.dll");
typedef BOOL(*gvptr)(char*,int);
gvptr func = (gvptr)GetProcAddress(dll, "jfwGetRunningVersion");
printf("gvptr=%p\n", func);
char buf[1024] = {0};
BOOL re = func(buf, 1023);
printf("getVersion: %d, %s, error=%d\n", re, buf, GetLastError());
return 0;
}
