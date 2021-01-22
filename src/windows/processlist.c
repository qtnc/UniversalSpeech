/*
Copyright (c) 2011-2015, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
// Process list - useful to find out which screen reader is running
#define _WIN32_WINNT 0x0501
#undef UNICODE
#include <windows.h>
#include <psapi.h>
#include<tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

BOOL FileExists (const char* fn) {
return GetFileAttributes(fn)!=INVALID_FILE_ATTRIBUTES;
}

BOOL DriverPathToNormalPath (const char* path, char* buf, int buflen) {
if (strnicmp(path, "\\Device\\HarddiskVolume", 22)!=0) {
strncpy(buf, path, buflen);
return FALSE;
}
int pos = strchr(path+22, '\\') -path;
strncpy(buf+2, path+pos, buflen);
buf[1]=':';
for (pos = 'C'; pos<='Z'; pos++) {
buf[0]=pos;
if (FileExists(buf)) return TRUE;
}
return FALSE;
}

HANDLE __declspec(dllexport) getProcessHandle (const char* needle, char* pfn, size_t pfnsz) {
static BOOL(*WINAPI LPQueryFullProcessImageNameA)(HANDLE,DWORD,LPSTR,PDWORD) = NULL;
if (!LPQueryFullProcessImageNameA) {
HANDLE kernel32 = LoadLibrary("kernel32");
LPQueryFullProcessImageNameA = GetProcAddress(kernel32, "QueryFullProcessImageNameA");
FreeLibrary(kernel32);
}
char name[512]={0};
DWORD nPids=0, pids[1024]={0};
EnumProcesses(pids, 1024*sizeof(DWORD) ,&nPids);
nPids/=sizeof(DWORD);
DWORD i; for (i=0; i<nPids; i++) {
HANDLE h = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION,0,pids[i]);
if (!h) continue;
DWORD namelen = 512;
BOOL result = FALSE;
if (LPQueryFullProcessImageNameA) result =  LPQueryFullProcessImageNameA(h, 0, name, &namelen);
else result = GetProcessImageFileName(h,name,512);
if (!result) continue;
if(stristr(name, needle)) {
if (pfn&&pfnsz>0) {
if (LPQueryFullProcessImageNameA) snprintf(pfn, pfnsz, "%s", name);
else DriverPathToNormalPath(name, pfn, pfnsz);
}
return h;
}
CloseHandle(h);
}
	return NULL;
}

BOOL __declspec(dllexport) FindProcess (const char* needle, char* buf, size_t bufsize) {
HANDLE h = getProcessHandle(needle, buf, bufsize);
if (h) CloseHandle(h);
return !!h;
}

BOOL VerGetField (LPVOID verbuf, const char* field, char** buf, int* buflen) {
DWORD dwSize=0, *dwLang=NULL;
if (!VerQueryValue(verbuf, "\\VarFileInfo\\Translation", &dwLang, &dwSize)) return FALSE;
char tmp[300]={0};
snprintf(tmp, 299, "\\StringFileInfo\\%04x%04x\\%s", LOWORD(*dwLang), HIWORD(*dwLang), field);
return VerQueryValue(verbuf, tmp, buf, buflen);
}

BOOL GetProcessVersionInfo (const char* pfn, int mode, char* buf, int buflen) {
DWORD dwHandle = 0, dwSize=0;
dwSize = GetFileVersionInfoSize(pfn, &dwHandle);
if (dwSize<=0) return FALSE;
char verbuf[dwSize];
if (!GetFileVersionInfo(pfn, dwHandle, dwSize, verbuf)) return FALSE;
switch(mode) {
case 0 : case 1 : {
VS_FIXEDFILEINFO* ver = NULL;
if (!VerQueryValue(verbuf, "\\", &ver, &dwSize)) return FALSE;
if (mode==0) memcpy(buf, ver, dwSize);
else snprintf(buf, buflen, "%d.%d.%d.%d\r\n", HIWORD(ver->dwProductVersionMS), LOWORD(ver->dwProductVersionMS), HIWORD(ver->dwProductVersionLS), LOWORD(ver->dwProductVersionLS));
return TRUE;
}
case 2 : {
char* ver = NULL;
if (!VerGetField(verbuf, "ProductVersion", &ver, &dwSize)) return FALSE;
strncpy(buf, ver, buflen);
return TRUE;
}
default: return FALSE;
}}
