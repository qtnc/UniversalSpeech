/*
Copyright (c) 2011-2021, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
//Encoding conversion using windows routines
#ifndef _____ENCODING_CONVERSION
#define _____ENCODING_CONVERSION
#include<stdio.h>
#include<string.h>
#include<wchar.h>
#include<windows.h>

wchar_t* mb2wc (const char* str, int inCP) ;
char* wc2mb (const wchar_t* wstr, int outCP);
char* toEncoding (const wchar_t* unicode, int targetEncoding);
wchar_t* fromEncoding (const char* str, int encoding);

#endif
