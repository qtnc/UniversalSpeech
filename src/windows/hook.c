/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include<windows.h>

static HHOOK hook = NULL;

static LRESULT kbHook (int code, WPARAM wp, LPARAM lp) {
if (code == HC_ACTION) {
int k = ((KBDLLHOOKSTRUCT*)lp) ->vkCode;
switch(k){
case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT:
case VK_HOME: case VK_END: 
case VK_DELETE: case VK_TAB:
case VK_PRIOR: case VK_NEXT:
case VK_BACK: case VK_ESCAPE: 
return FALSE;
default: break;
}}
return CallNextHookEx(hook, code, wp, lp);
}

BOOL export installKeyboardHook (void) {
if (hook) uninstallKeyboardHook();
if (hook) return;
hook = SetWindowsHookEx( WH_KEYBOARD_LL, kbHook, GetModuleHandle(NULL), NULL);
return !!hook;
}

void export uninstallKeyboardHook (void) {
if (!hook) return;
UnhookWindowsHookEx(hook);
hook = NULL;
}

