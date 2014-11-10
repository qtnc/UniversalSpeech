/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include<windows.h>
#include "../../include/UniversalSpeech.h"

BOOL export __stdcall sayStringA (const char* str, BOOL interrupt) {
return speechSayA(str,interrupt);
}

BOOL export __stdcall sayStringW (const void* str, BOOL interrupt) {
return speechSay(str,interrupt);
}

BOOL export __stdcall brailleMessageA (const char* str) {
return brailleDisplayA(str);
}


BOOL export __stdcall brailleMessageW (const char* str) {
return brailleDisplay(str);
}

BOOL export __stdcall stopSpeech (void) {
return speechStop();
}

int export __stdcall getCurrentScreenReader (void) {
return speechGetValue(SP_ENGINE);
}


int export __stdcall setCurrentScreenReader (int n) {
return speechSetValue(SP_ENGINE, n);
}

