/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include<windows.h>
#include "disphelper.h"

static IDispatch* jfw = NULL;

void dhAutoInit (void) {
static BOOL init = FALSE;
if (!init) {
dhInitialize(TRUE);
init = TRUE;
}}

BOOL export jfwLoad () {
dhAutoInit();
if (jfw) SAFE_RELEASE(jfw);
if (!jfw) dhCreateObject( L"FreedomSci.JawsApi", NULL, &jfw);
if (!jfw) dhCreateObject( L"JFWApi", NULL, &jfw);
if (!jfw) dhCreateObject( L"JFWApi.1", NULL, &jfw);
return !!jfw;
}

void export jfwUnload (void) {
SAFE_RELEASE(jfw);
}

int export jfwIsAvailable () {
return !!FindWindow("JFWUI2", NULL);
}

BOOL export jfwSayA (const char* str, BOOL interrupt) {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".SayString(%s,%b)", str, interrupt);
return result;
}

BOOL export jfwSayW (const wchar_t* str, BOOL interrupt) {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".SayString(%S,%b)", str, interrupt);
return result;
}

BOOL export jfwRunScriptA (const char* scriptName) {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".RunScript(%s)", scriptName);
return result;
}

BOOL export jfwRunScriptW (const wchar_t* scriptName) {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".RunScript(%S)", scriptName);
return result;
}

BOOL export jfwRunFunctionA (const char* scriptName) {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".RunFunction(%s)", scriptName);
return result;
}

BOOL export jfwRunFunctionW (const wchar_t* scriptName) {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".RunFunction(%S)", scriptName);
return result;
}

BOOL export jfwBrailleA (const char* str) {
int len = strlen(str);
char buf[len+20];
buf[0] = 0;
strcat(buf, "BrailleString(\"");
strcat(buf, str);
strcat(buf, "\")");
wchar_t *c = strchr(buf,34), *end = strrchr(buf,34);
while (++c<end) {
if (*c==34 || *c==92 || *c<32) *c=32;
}
return jfwRunFunctionA(buf);
}

BOOL export jfwBrailleW (const wchar_t* str) {
int len = wcslen(str);
wchar_t buf[len+20];
buf[0] = 0;
wcscat(buf, L"BrailleString(\"");
wcscat(buf, str);
wcscat(buf, L"\")");
wchar_t *c = wcschr(buf,34), *end = wcsrchr(buf,34);
while (++c<end) {
if (*c==34 || *c==92 || *c<32) *c=32;
}
return jfwRunFunctionW(buf);
}

BOOL export jfwStopSpeech () {
if (!jfw && !jfwLoad()) return FALSE;
BOOL result = FALSE;
dhGetValue(L"%b", &result, jfw, L".StopSpeech()");
return result;
}

BOOL export jfwGetUserSettingsDirectory (char* buf, size_t bufmax) {
if (!FindProcess("jfw.exe", buf, bufmax)) return FALSE;
char sVer[16]={0}, *tok=NULL, *appdata = getenv("APPDATA");
tok = strtok(buf, "\\");
while(tok) {
int n;
if (sscanf(tok, "%d", &n)) break;
tok = strtok(NULL,"\\");
}
if (!tok || !appdata) return FALSE;
strncpy(sVer, tok, 15);
int pos = snprintf(buf, bufmax, "%s\\Freedom Scientific\\Jaws\\%s\\Settings\\???", appdata, sVer) -3;
WIN32_FIND_DATA fd;
HANDLE hfd = FindFirstFile(buf, &fd);
if (!hfd) return FALSE;
do {
if (!strcmp(fd.cFileName,".") && !strcmp(fd.cFileName,"..") && !strcmp(fd.cFileName,"enu")) break;
} while (FindNextFile(hfd, &fd));
FindClose(hfd);
snprintf(buf+pos, bufmax-pos, "%s\\", fd.cFileName);
return TRUE;
}

BOOL export jfwGetRunningVersion (char* buf, int bufmax) {
if (!FindProcess("jfw.exe", buf, bufmax)) return FALSE;
return GetProcessVersionInfo(buf, 1, buf, bufmax);
}


