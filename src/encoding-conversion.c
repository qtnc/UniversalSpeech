/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include<stdio.h>
#include<string.h>
#include<wchar.h>

static void* conv = NULL;
static int convlen = 0;

const char* unicode2ansi (const wchar_t* unicode) {
if (!unicode) return NULL;
int len = wcslen(unicode);
if (convlen<len*sizeof(char)) {
conv = realloc(conv, (len+2) * sizeof(char));
convlen = len*sizeof(char);
}
len = wcstombs(conv, unicode, len+1);
if (len<0) return NULL;
((char*)conv)[len]=0;
return conv;
}

const wchar_t* ansi2unicode (const char* ansi) {
if (!ansi) return NULL;
int len = strlen(ansi);
if (convlen<len*sizeof(wchar_t)) {
conv = realloc(conv, (len+2) * sizeof(wchar_t));
convlen = sizeof(wchar_t)*len;
}
len = mbstowcs(conv, ansi, len+1);
if (len<0) return NULL;
((wchar_t*)conv)[len]=0;
return conv;
}

