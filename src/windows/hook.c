/*
Copyright (c) 2011-2021, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
// Jaws keyboard hook
#include "../../include/UniversalSpeech.h"
#include<windows.h>

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define IsKeyDown(k) (GetAsyncKeyState(k)<0)

export void uninstallKeyboardHook (void);

static HHOOK hook = NULL;

static LRESULT kbHook (int code, WPARAM wp, LPARAM lp) {
if (code == HC_ACTION) {
int k = ((KBDLLHOOKSTRUCT*)lp) ->vkCode;
switch(k){
case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT:
case VK_HOME: case VK_END: 
case VK_PRIOR: case VK_NEXT:
case VK_DELETE: case VK_BACK: case VK_ESCAPE:
case 'T':
if (IsKeyDown(VK_INSERT)) break;
return FALSE;
case VK_TAB : case VK_SPACE:
if (IsKeyDown(VK_MENU) ||  IsKeyDown(VK_INSERT) || IsKeyDown(VK_LMENU) || IsKeyDown(VK_RMENU)) break;
return FALSE;
default: break;
}}
return CallNextHookEx(hook, code, wp, lp);
}

export BOOL installKeyboardHook (void) {
if (hook) uninstallKeyboardHook();
if (hook) return TRUE;
hook = SetWindowsHookEx( WH_KEYBOARD_LL, kbHook, GetModuleHandle(NULL), 0);
return !!hook;
}

export void uninstallKeyboardHook (void) {
if (!hook) return;
UnhookWindowsHookEx(hook);
hook = NULL;
}

