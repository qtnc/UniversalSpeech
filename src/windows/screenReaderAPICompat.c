/*
Copyright (c) 2011-2015, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
//ScreenReaderCompat.c: attempt a compatibility with old ScreenReaderAPI.dll
#include "encoding-conversion.h"
#include<windows.h>
#include "../../include/UniversalSpeech.h"

#undef export
#define export __declspec(dllexport) __stdcall

extern const int numEngines;

export BOOL sayStringA (const char* str, BOOL interrupt) {
return speechSayA(str,interrupt);
}

export BOOL sayStringW (const void* str, BOOL interrupt) {
return speechSay(str,interrupt);
}

export BOOL brailleMessageA (const char* str) {
return brailleDisplayA(str);
}

export BOOL brailleMessageW (const wchar_t* str) {
return brailleDisplay(str);
}

export BOOL stopSpeech (void) {
return speechStop();
}

export int getCurrentScreenReader (void) {
return speechGetValue(SP_ENGINE);
}

export int setCurrentScreenReader (int n) {
return speechSetValue(SP_ENGINE, n);
}

export const wchar_t* getScreenReaderNameW (int n) {
return speechGetString(SP_ENGINE +n);
}

export const char* getScreenReaderNameA (int n) {
return toEncoding(getScreenReaderNameW(n),0);
}

export const wchar_t* getCurrentScreenReaderNameW (void) {
return getScreenReaderNameW(getCurrentScreenReader());
}

export const char* getCurrentScreenReaderNameA (void) {
return getScreenReaderNameA(getCurrentScreenReader());
}

export int getScreenReaderIdW (const wchar_t* name) {
int i=-1;
const wchar_t* s=NULL;
while (s = getScreenReaderNameW(++i)) {
if (s && name && !wcscmp(name,s)) return i;
}
return -1;
}

export int getScreenReaderIdA (const char* name) {
return getScreenReaderIdW(fromEncoding(name,0));
}

export int setCurrentScreenReaderNameW (const wchar_t* name) {
return setCurrentScreenReader(getScreenReaderIdW(name));
}

export int setCurrentScreenReaderNameA (const char* name) {
return setCurrentScreenReader(getScreenReaderIdA(name));
}

export BOOL sapiIsEnabled (void) {
return speechGetValue(SP_ENABLE_NATIVE_SPEECH);
}

export BOOL sapiEnable (BOOL enable) {
speechSetValue(SP_ENABLE_NATIVE_SPEECH, enable);
}

export int getSupportedScreenReadersCount (void) {
return numEngines;
}

BOOL sapiSetRate (int rate);
int sapiGetRate (void);

int __stdcall sapiGetRate2 (void) {
return sapiGetRate();
}

int __stdcall sapiSetRate2 (int rate) {
return sapiSetRate(rate);
}


