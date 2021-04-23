/*
Copyright (c) 2011-2021, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
// we.c: Windows eyes, using GWMicro COM object
#include "encoding-conversion.h"
#include "../../include/UniversalSpeech.h"
#include<windows.h>
#include "disphelper.h"
#include "encoding-conversion.h"

void dhAutoInit (void);

static IDispatch* we = NULL;

export int weIsAvailable () {
return !!FindWindow("GWMExternalControl", "External Control");
}

export BOOL weLoad () {
dhAutoInit();
if (we) SAFE_RELEASE(we);
dhCreateObject(L"GWSpeak.Speak", NULL, &we);
return !!we;
}

export void weUnload () {
SAFE_RELEASE(we);
}

export BOOL weSayA (const char* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".SpeakString(%s)", str);
return TRUE;
}

export BOOL weSayW (const wchar_t* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".SpeakString(%S)", str);
return TRUE;
}

export BOOL weBrailleA (const char* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".Display(%s)", str);
return TRUE;
}

export BOOL weBrailleW (const wchar_t* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".Display(%S)", str);
return TRUE;
}

export BOOL weStopSpeech (void) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".Silence()");
return TRUE;
}

