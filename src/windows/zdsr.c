/*
Copyright (c) 2011-2022, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
// zdsr.c: ZDSR chinese screen reader, using zdsrapi.dll
#include "../../include/UniversalSpeech.h"
#include<windows.h>
#include "encoding-conversion.h"

const wchar_t* composePath (const wchar_t* dll);

static HINSTANCE zdsr = NULL;
static int(*__stdcall InitTTS)(int, const wchar_t*)  = NULL;
static int(*__stdcall Speak)(const wchar_t*, BOOL)  = NULL;
static int(*__stdcall GetSpeakState)(void)  = NULL;
static int(*__stdcall StopSpeak)(void) = NULL;

export void zdsrUnload (void) {
InitTTS = NULL;
StopSpeak = NULL;
GetSpeakState = NULL;
Speak = NULL;
if (zdsr) FreeLibrary(zdsr);
zdsr = NULL;
}

export BOOL zdsrLoad (void) {
zdsrUnload();
zdsr = LoadLibraryW(composePath(L"zdsrapi.dll"));
if (!zdsr) return FALSE;
#define LOAD(f) { *(void**)(&f) = (void*)GetProcAddress(zdsr, #f); if (!f) { zdsrUnload(); return FALSE; }}
LOAD(InitTTS) LOAD(GetSpeakState)
LOAD(Speak) LOAD(StopSpeak)
#undef LOAD
return InitTTS && !InitTTS(0, NULL);
}

export BOOL zdsrIsAvailable (void) {
if (!GetSpeakState) zdsrLoad();
if (!GetSpeakState) return FALSE;
int r = GetSpeakState();
return r==3 || r==4;
}

export BOOL zdsrIsSpeaking (void) {
if (!GetSpeakState) zdsrLoad();
if (!GetSpeakState) return FALSE;
return GetSpeakState()==3;
}

export BOOL zdsrStopSpeech (void) {
if (!StopSpeak) zdsrLoad();
if (!StopSpeak) return FALSE;
StopSpeak();
return TRUE;
}

export int zdsrSayW (const wchar_t* str, int interrupt) {
if (!Speak) zdsrLoad();
return Speak && !Speak(str, interrupt);
}

