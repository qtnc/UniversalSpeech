/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include<windows.h>
#include "../../include/UniversalSpeech.h"
#include "../private.h"

const wchar_t* mb2wc (const char* str, int inCP) {
int len = MultiByteToWideChar(inCP, 0, str, -1, NULL, 0);
wchar_t* wstr = malloc(sizeof(wchar_t) * (len+1));
MultiByteToWideChar(inCP, 0, str, -1, wstr, len);
wstr[len]=0;
return wstr;
}

const char* wc2mb (const wchar_t* wstr, int outCP) {
int len = WideCharToMultiByte(outCP, 0, wstr, -1, NULL, 0, NULL, NULL);
char* str = malloc(len+1);
WideCharToMultiByte(outCP, 0, wstr, -1, str, len, NULL, NULL);
str[len]=0;
return str;
}

int wstrlen (const wchar_t* w) {
const wchar_t* x = w;
while (*x) x++;
return x-w;
}

const char* copy (const char* str) {
int l = strlen(str);
char* c = malloc(l+1);
memcpy(c,str,l+1);
c[l]=0;
return c;
}

const wchar_t* wcopy (const wchar_t* str) {
int l = wstrlen(str);
wchar_t* c = malloc(sizeof(wchar_t)*(l+1));
memcpy(c,str,sizeof(wchar_t)*(l+1));
c[l]=0;
return c;
}

const char* composePath (const char* dll) {
static char path[300];
GetModuleFileNameA(GetModuleHandleA("UniversalSpeech.dll"), path, 300);
char* c = strrchr(path,'\\');
sprintf(c+1, "%s", dll);
return path;
}

const char* stristr (const char* str, const char* fnd) {
const char* needle = fnd;
while (*str) {
if (tolower(*str)==tolower(*needle)) {
needle++;
if (!*needle) return str + (needle -fnd);
}
else needle = fnd;
str++;
}
return NULL;
}

