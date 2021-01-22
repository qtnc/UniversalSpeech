/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#include "../../include/UniversalSpeech.h"
#include "quentinc_UniversalSpeech.h"
#include<wchar.h>
#include<string.h>

#define getString(x) (*env)->GetStringChars(env, x, False)
#define releaseString(x,y) (*env)->ReleaseStringChars(env, x, y)
#define newString(x) (*env)->NewString(env, x, wcslen(x))
static jboolean JNIFalse = JNI_FALSE;
static jboolean* False = &JNIFalse;



JNIEXPORT jboolean JNICALL Java_quentinc_UniversalSpeech_say   (JNIEnv *env, jclass c, jstring str, jboolean interrupt) {
const jchar* wstr = getString(str);
jboolean re = speechSay(wstr, interrupt);
releaseString(str, wstr);
return re;
}

JNIEXPORT jboolean JNICALL Java_quentinc_UniversalSpeech_braille   (JNIEnv *env, jclass c, jstring str) {
const jchar* wstr = getString(str);
jboolean re = brailleDisplay(wstr);
releaseString(str, wstr);
return re;
}


JNIEXPORT jboolean JNICALL Java_quentinc_UniversalSpeech_stop   (JNIEnv *env, jclass c) {
return speechStop();
}

JNIEXPORT jboolean JNICALL Java_quentinc_UniversalSpeech_setValue   (JNIEnv *env, jclass c, jint what, jint value) {
return speechSetValue(what, value);
}

JNIEXPORT jint JNICALL Java_quentinc_UniversalSpeech_getValue   (JNIEnv *env, jclass c, jint what) {
return speechGetValue(what);
}

JNIEXPORT jboolean JNICALL Java_quentinc_UniversalSpeech_setString   (JNIEnv *env, jclass c, jint what, jstring value) {
const jchar* wstr = getString(value);
jboolean re = speechSetString(what, wstr);
releaseString(value, wstr);
return re;
}

JNIEXPORT jstring JNICALL Java_quentinc_UniversalSpeech_getString   (JNIEnv *env, jclass c, jint what) {
const wchar_t* w = speechGetString(what);
if (!w) return NULL;
int wlen = wcslen(w);
wchar_t* w2 = malloc(sizeof(wchar_t) * (wlen+1));
memcpy(w2, w, sizeof(wchar_t)*(wlen+1));
jstring s = newString(w2);
free(w2);
return s;
}

