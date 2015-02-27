/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include<windows.h>

static HINSTANCE systemaccess = NULL;
static BOOL(*SA_IsRunning)(void) = NULL;
static void(*SA_StopAudio)(void) = NULL;
static void(*SA_SayW)(const wchar_t*) = NULL;
static void(*SA_SayA)(const char*) = NULL;
static void(*SA_BrlShowTextW)(const wchar_t*) = NULL;
static void(*SA_BrlShowTextA)(const char*) = NULL;

void export saUnload (void) {
SA_IsRunning = NULL;
SA_StopAudio = NULL;
SA_SayW = NULL;
SA_SayA = NULL;
SA_BrlShowTextW = NULL;
SA_BrlShowTextA = NULL;
if (systemaccess) FreeLibrary(systemaccess);
systemaccess = NULL;
}

BOOL export saLoad (void) {
saUnload();
systemaccess = LoadLibrary(composePath("SAAPI32.DLL"));
if (!systemaccess) return FALSE;
#define LOAD(f) { f = GetProcAddress(systemaccess,#f); if (!f) { saUnload(); return FALSE; }}
LOAD(SA_IsRunning) LOAD(SA_StopAudio)
LOAD(SA_SayW) LOAD(SA_SayA)
LOAD(SA_BrlShowTextW) LOAD(SA_BrlShowTextA)
#undef LOAD
return TRUE;
}

BOOL export saIsAvailable () {
if (!SA_IsRunning) saLoad();
if (SA_IsRunning) return SA_IsRunning();
else return FALSE;
}

BOOL export saSayA (const char* str) {
if (!SA_SayA && !saLoad()) return FALSE;
if (SA_SayA) SA_SayA(str);
return TRUE;
}

BOOL export saSayW (const wchar_t* str) {
if (!SA_SayW && !saLoad()) return FALSE;
if (SA_SayW) SA_SayW(str);
return TRUE;
}

BOOL export saBrailleA (const char* str) {
if (!SA_BrlShowTextA && !saLoad()) return FALSE;
if (SA_BrlShowTextA) SA_BrlShowTextA(str);
return TRUE;
}

BOOL export saBrailleW (const wchar_t* str) {
if (!SA_BrlShowTextW && !saLoad()) return FALSE;
if (SA_BrlShowTextW) SA_BrlShowTextW(str);
return TRUE;
}

BOOL export saStopSpeech (void) {
if (!SA_StopAudio && !saLoad()) return FALSE;
if (SA_StopAudio) SA_StopAudio();
return TRUE;
}

