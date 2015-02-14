/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include<windows.h>

static HINSTANCE dolphin = NULL;
static int(*__stdcall DolAccess_GetSystem)(void) = NULL;
static void(*__stdcall DolAccess_Action)(int) = NULL;
static void(*__stdcall DolAccess_Command)(const wchar_t*, int, int)  = NULL;


void export dolUnload (void) {
DolAccess_GetSystem = NULL;
DolAccess_Action = NULL;
DolAccess_Command = NULL;
if (dolphin) FreeLibrary(dolphin);
dolphin = NULL;
}

BOOL export dolLoad (void) {
dolUnload();
dolphin = LoadLibrary(composePath("dolapi.dll"));
if (!dolphin) return FALSE;
DolAccess_GetSystem = GetProcAddress(dolphin, "_DolAccess_GetSystem@0");
DolAccess_Action = GetProcAddress(dolphin, "_DolAccess_Action@4");
DolAccess_Command = GetProcAddress(dolphin, "_DolAccess_Command@12");
return TRUE;
}

BOOL export dolIsAvailable (void) {
if (!DolAccess_GetSystem) dolLoad();
if (DolAccess_GetSystem) {
int result = DolAccess_GetSystem();
return (result==1 || result==4 || result==8); 
}
else return FALSE;
}

BOOL export dolSay (const wchar_t* str) {
if (dolIsAvailable() && DolAccess_Command) {
DolAccess_Command(str, (wstrlen(str)+1)*sizeof(wchar_t), 1);
return TRUE;
}
else return FALSE;
}

BOOL export dolStopSpeech (void) {
if (!DolAccess_Action) dolLoad();
if (DolAccess_Action) {
DolAccess_Action(141);
return TRUE;
}
else return FALSE;
}


