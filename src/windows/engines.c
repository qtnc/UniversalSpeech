/*
Copyright (c) 2011-2021, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
// List of available engines under windows
#include "../../include/UniversalSpeech.h"
#include "../private.h"
#include<windows.h>

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"


export int jfwIsAvailable (void) ;
export int jfwSayW (const wchar_t*, int) ;
export int jfwBrailleW (const wchar_t*) ;
export int jfwStopSpeech (void) ;
export int jfwUnload (void) ;
export int nvdaIsAvailable (void) ;
export int nvdaUnload (void) ;
export int nvdaSayW (const wchar_t*, int) ;
export int nvdaBraille (const wchar_t*) ;
export int nvdaStopSpeech (void) ;
export int weIsAvailable (void) ;
export int weSayW (const wchar_t*, int) ;
export int weBrailleW (const wchar_t*) ;
export int weStopSpeech (void) ;
export int weUnload (void) ;
export int saUnload (void) ;
export int saIsAvailable (void) ;
export int saSayW (const wchar_t*, int) ;
export int saBrailleW (const wchar_t*) ;
export int saStopSpeech (void) ;
export int dolUnload (void) ;
export int dolIsAvailable (void) ;
export int dolSay (const wchar_t*, int) ;
export int dolStopSpeech (void) ;
export int cbrUnload (void) ;
export int cbrIsAvailable (void) ;
export int cbrSayW (const wchar_t*, int) ;
export int cbrBrailleW (const wchar_t*) ;
export int cbrStopSpeech (void) ;
export int ztLoad (void) ;
export int ztUnload (void) ;
export int ztIsAvailable (void) ;
export int ztSayW (const wchar_t*, int);
export int ztStopSpeech (void) ;
export int sapiIsAvailable (void) ;
export int sapiSayW (const wchar_t*, int) ;
export int sapiStopSpeech (void) ;
export int sapiUnload (void) ;
export int sapiSetValue (int, int);
export int sapiGetValue (int) ;
export const void* sapiGetString (int) ;
export int narIsAvailable (void) ;

static int doNothing () { return 1; }

const engine engines[] = {
{ .name=L"Jaws", .isAvailable=jfwIsAvailable, .unload=jfwUnload, .say=jfwSayW, .stop=jfwStopSpeech, .braille=jfwBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Windows eye", .isAvailable=weIsAvailable, .unload=weUnload, .say=weSayW, .stop=weStopSpeech, .braille=weBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"NVDA", .isAvailable=nvdaIsAvailable, .unload=nvdaUnload, .say=nvdaSayW, .braille=nvdaBraille, .stop=nvdaStopSpeech, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"System access", .isAvailable=saIsAvailable, .unload=saUnload, .say=saSayW, .stop=saStopSpeech, .braille=saBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Supernova", .isAvailable=dolIsAvailable, .unload=dolUnload, .say=dolSay, .stop=dolStopSpeech, .braille=doNothing, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"ZoomText", .isAvailable=ztIsAvailable, .unload=ztUnload, .say=ztSayW, .stop=ztStopSpeech, .braille=doNothing, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Cobra", .isAvailable=cbrIsAvailable, .unload=cbrUnload, .say=cbrSayW, .stop=cbrStopSpeech, .braille=cbrBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Narrator", .isAvailable=narIsAvailable, .unload=doNothing, .say=doNothing, .stop=doNothing, .braille=doNothing, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"SAPI5", .isAvailable=sapiIsAvailable, .unload=sapiUnload, .say=sapiSayW, .stop=sapiStopSpeech, .braille=doNothing, .setValue=sapiSetValue, .getValue=sapiGetValue, .setString=NULL, .getString=sapiGetString  },
{ .name=NULL, .isAvailable=NULL, .unload=NULL, .stop=NULL, .say=NULL, .braille=NULL, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL }
};
const int numEngines = sizeof(engines)/sizeof(engine) -1;



