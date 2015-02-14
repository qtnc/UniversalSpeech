/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include<windows.h>

static HHOOK hook = NULL;

static LRESULT kbHook (int code, WPARAM wp, LPARAM lp) {
//if (!win || !IsWindow(win)) uninstallHook();
//else if (code == HC_ACTION) {
//KBDLLHOOKSTRUCT* k = lp;
//if (k->vkCode==VK_INSERT && (wp == WM_KEYDOWN || wp == WM_SYSKEYDOWN)) PostMessage(win, WM_KEYDOWN, VK_INSERT, 0);
//if (k->vkCode==VK_INSERT && (wp == WM_KEYUP || wp==WM_SYSKEYUP)) PostMessage(win, WM_USER, k->vkCode, 0);
//}
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

