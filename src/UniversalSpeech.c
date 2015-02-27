/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../include/UniversalSpeech.h"
#include "private.h"
#include<stdlib.h> //For NULL

extern const engine engines[];
extern const int numEngines;
int useDefault = 1;
int current = -1;
int nativeSpeechEnabled = 1;

static void periodicRetry (void) {
if (!useDefault) return;
static long sapiLastSpeak = 0;
long l = GetTickCount();
if (l-sapiLastSpeak>=10000) {
sapiLastSpeak=l;
current = -1;
}}

static int detect (void) {
for (int i=0; engines[i].name; i++) {
if (engines[i].isAvailable()) return i;
}
return -1;
}

int export speechSay (const void* str, int interrupt) {
begin : if (current<0 && (current=detect())<0) return 0;
if (!engines[current].say(str,interrupt) && !engines[current].isAvailable()) {
engines[current].unload();
current = -1;
goto begin;
}
else periodicRetry();
return 1;
}

int export speechStop (void) {
begin : if (current<0 && (current=detect())<0) return 0;
if (!engines[current].stop() && !engines[current].isAvailable()) {
engines[current].unload();
current = -1;
goto begin;
}
else periodicRetry();
return 1;
}

int export brailleDisplay (const void* str) {
begin : if (current<0 && (current=detect())<0) return 0;
if (!engines[current].braille(str) && !engines[current].isAvailable()) {
engines[current].unload();
current = -1;
goto begin;
}
else periodicRetry();
return 1;
}

int export speechGetValue (int what) {
switch(what){
case SP_ENGINE: 
if (current<0) current = detect();
return current;
case SP_ENABLE_NATIVE_SPEECH: 
return nativeSpeechEnabled;
case SP_AUTO_ENGINE: return useDefault;
default: break;
}
if (what>=SP_ENGINE_AVAILABLE && what<SP_ENGINE_AVAILABLE+numEngines) return engines[what - SP_ENGINE_AVAILABLE].isAvailable();
if (current<0) current=detect();
if (current>=0 && engines[current].getValue) return engines[current].getValue(what);
return 0;
}

int export speechSetValue (int what, int value) {
switch(what){
case SP_ENGINE: 
if (value<0) {
current = -1;
useDefault = 1;
return 1;
}
else if (value<numEngines) {
useDefault = 0;
current = value;
return 1;
}
else return 0;
case SP_ENABLE_NATIVE_SPEECH : 
nativeSpeechEnabled = !!value; 
return nativeSpeechEnabled;
default: 
break;
}
if (current<0) current=detect();
if (current>=0 && engines[current].setValue) return engines[current].setValue(what, value);
return 0;
}

const void* export speechGetString (int what) {
if (what>=SP_ENGINE && what<SP_ENGINE+numEngines) return engines[what - SP_ENGINE].name;
if (current<0) current=detect();
if (current>=0 && engines[current].getString) return engines[current].getString(what);
return NULL;
}

int export speechSetString (int what, const void* value) {
if (current<0) current=detect();
if (current>=0 && engines[current].setString) return engines[current].setString(what, value);
return 0;
}

int export speechSayA (const char* str, int interrupt) {
return speechSay(ansi2unicode(str), interrupt);
}

int export brailleDisplayA (const char* str) {
return brailleDisplay(ansi2unicode(str));
}

const char* export speechGetStringA (int what) {
return unicode2ansi(speechGetString(what));
}

int export speechSetStringA (int what, const char* value) {
return speechSetString(what, ansi2unicode(value));
}


