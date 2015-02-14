/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include "../private.h"
#include<windows.h>


int export jfwIsAvailable (void) ;
int export jfwSayW (const wchar_t*, int) ;
int export jfwBrailleW (const wchar_t*) ;
int export jfwStopSpeech (void) ;
int export jfwUnload (void) ;
int export nvdaIsAvailable (void) ;
int export nvdaUnload (void) ;
int export nvdaSayW (const wchar_t*, int) ;
int export nvdaBraille (const wchar_t*) ;
int export nvdaStopSpeech (void) ;
int export weIsAvailable (void) ;
int export weSayW (const wchar_t*, int) ;
int export weBrailleW (const wchar_t*) ;
int export weStopSpeech (void) ;
int export weUnload (void) ;
int export saUnload (void) ;
int export saIsAvailable (void) ;
int export saSayW (const wchar_t*, int) ;
int export saBrailleW (const wchar_t*) ;
int export saStopSpeech (void) ;
int export dolUnload (void) ;
int export dolIsAvailable (void) ;
int export dolSay (const wchar_t*, int) ;
int export dolStopSpeech (void) ;
int export cbrUnload (void) ;
int export cbrIsAvailable (void) ;
int export cbrSayW (const wchar_t*, int) ;
int export cbrBrailleW (const wchar_t*) ;
int export cbrStopSpeech (void) ;
int export ztLoad (void) ;
int export ztUnload (void) ;
int export ztIsAvailable (void) ;
int export ztSayW (const wchar_t*, int);
int export ztStopSpeech (void) ;
int export sapiIsAvailable (void) ;
int export sapiSayW (const wchar_t*, int) ;
int export sapiStopSpeech (void) ;
int export sapiUnload (void) ;
int export sapiSetValue (int, int);
int export sapiGetValue (int) ;
const void* sapiGetString (int) ;

static int doNothing () { return 1; }

const engine engines[] = {
{ .name=L"Jaws", .isAvailable=jfwIsAvailable, .unload=jfwUnload, .say=jfwSayW, .stop=jfwStopSpeech, .braille=jfwBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Windows eye", .isAvailable=weIsAvailable, .unload=weUnload, .say=weSayW, .stop=weStopSpeech, .braille=weBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"NVDA", .isAvailable=nvdaIsAvailable, .unload=nvdaUnload, .say=nvdaSayW, .braille=nvdaBraille, .stop=nvdaStopSpeech, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"System access", .isAvailable=saIsAvailable, .unload=saUnload, .say=saSayW, .stop=saStopSpeech, .braille=saBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Supernova", .isAvailable=dolIsAvailable, .unload=dolUnload, .say=dolSay, .stop=dolStopSpeech, .braille=doNothing, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"ZoomText", .isAvailable=ztIsAvailable, .unload=ztUnload, .say=ztSayW, .stop=ztStopSpeech, .braille=doNothing, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"Cobra", .isAvailable=cbrIsAvailable, .unload=cbrUnload, .say=cbrSayW, .stop=cbrStopSpeech, .braille=cbrBrailleW, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL  },
{ .name=L"SAPI5", .isAvailable=sapiIsAvailable, .unload=sapiUnload, .say=sapiSayW, .stop=sapiStopSpeech, .braille=doNothing, .setValue=sapiSetValue, .getValue=sapiGetValue, .setString=NULL, .getString=sapiGetString  },
{ .name=NULL, .isAvailable=NULL, .unload=NULL, .stop=NULL, .say=NULL, .braille=NULL, .setValue=NULL, .getValue=NULL, .setString=NULL, .getString=NULL }
};
const int numEngines = sizeof(engines)/sizeof(engine) -1;



