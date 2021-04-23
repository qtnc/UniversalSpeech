/*
Copyright (c) 2011-2021, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
// Windows 10 narrator
#include "../../include/UniversalSpeech.h"
#include<windows.h>
#include "disphelper.h"
#include "encoding-conversion.h"

void dhAutoInit (void);
const char* composePath (const char* dll);
BOOL __declspec(dllexport) FindProcess (const char* needle, char* buf, size_t bufsize);

export BOOL narIsAvailable (void) {
static int found=0, last = 0;
int z = GetTickCount();
if (z-last<60000) return found;
last=z;
found = FindProcess("narrator.exe", NULL, 0);
return found;
}

