/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include<windows.h>
#include "../../include/UniversalSpeech.h"

extern const int numEngines;

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

const wchar_t* export __stdcall getScreenReaderNameW (int n) {
return speechGetString(SP_ENGINE +n);
}

const char* export __stdcall getScreenReaderNameA (int n) {
return toEncoding(getScreenReaderNameW(n),0);
}

const wchar_t* export __stdcall getCurrentScreenReaderNameW (void) {
return getScreenReaderNameW(getCurrentScreenReader());
}

const char* export __stdcall getCurrentScreenReaderNameA (void) {
return getScreenReaderNameA(getCurrentScreenReader());
}

int export __stdcall getScreenReaderIdW (const wchar_t* name) {
int i=-1;
const wchar_t* s=NULL;
while (s = getScreenReaderNameW(++i)) {
if (s && name && !wcscmp(name,s)) return i;
}
return -1;
}

int export __stdcall getScreenReaderIdA (const char* name) {
return getScreenReaderIdW(fromEncoding(name,0));
}

int export __stdcall setCurrentScreenReaderNameW (const wchar_t* name) {
return setCurrentScreenReader(getScreenReaderIdW(name));
}

int export __stdcall setCurrentScreenReaderNameA (const wchar_t* name) {
return setCurrentScreenReader(getScreenReaderIdA(name));
}

BOOL export __stdcall sapiIsEnabled (void) {
return speechGetValue(SP_ENABLE_NATIVE_SPEECH);
}

BOOL export __stdcall sapiEnable (BOOL enable) {
speechSetValue(SP_ENABLE_NATIVE_SPEECH, enable);
}

int __stdcall sapiGetRate2 (void) {
return sapiGetRate();
}

int __stdcall sapiSetRate2 (int rate) {
return sapiSetRate(rate);
}

int export __stdcall getSupportedScreenReadersCount (void) {
return numEngines;
}
