/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include<windows.h>
#include "../../include/UniversalSpeech.h"
#include "../private.h"


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

