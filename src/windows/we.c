/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include<windows.h>
#include "disphelper.h"

static IDispatch* we = NULL;

int export weIsAvailable () {
return !!FindWindow("GWMExternalControl", "External Control");
}

BOOL export weLoad () {
dhAutoInit();
if (we) SAFE_RELEASE(we);
dhCreateObject(L"GWSpeak.Speak", NULL, &we);
return !!we;
}

void export weUnload () {
SAFE_RELEASE(we);
}

BOOL export weSayA (const char* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".SpeakString(%s)", str);
return TRUE;
}

BOOL export weSayW (const wchar_t* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".SpeakString(%S)", str);
return TRUE;
}

BOOL export weBrailleA (const char* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".Display(%s)", str);
return TRUE;
}

BOOL export weBrailleW (const wchar_t* str) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".Display(%S)", str);
return TRUE;
}

BOOL export weStopSpeech (void) {
if (!we && !weLoad()) return FALSE;
dhCallMethod(we, L".Silence()");
return TRUE;
}

