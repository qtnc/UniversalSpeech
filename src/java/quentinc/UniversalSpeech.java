/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
package quentinc;
public class UniversalSpeech {
public static final int
VOLUME=0, VOLUME_MAX=1, VOLUME_MIN=2, VOLUME_SUPPORTED=3,
RATE=4, RATE_MAX=5, RATE_MIN=6, RATE_SUPPORTED=7,
PITCH=8, PITCH_MAX=9, PITCH_MIN=10, PITCH_SUPPORTED=11,
INFLEXION=12, INFLEXION_MAX=13, INFLEXION_MIN=14, INFLEXION_SUPPORTED=15,
PAUSED=16, PAUSE_SUPPORTED=17,
BUSY=18, BUSY_SUPPORTED=19,
WAIT=20, WAIT_SUPPORTED=21,
ENABLE_NATIVE_SPEECH = 0xFFFF,
VOICE = 0x10000,
LANGUAGE = 0x20000,
SUBENGINE = 0x30000,
ENGINE = 0x40000,
ENGINE_AVAILABLE = 0x50000,
USER_PARAM = 0x1000000;

static {
System.loadLibrary("UniversalSpeech");
}

public static boolean say (String str) { return say(str, true); }
public static boolean setValue (int what, boolean value)  { return setValue(what, value?1:0); }

public static native boolean say (String str, boolean interrupt) ;
public static native boolean braille (String str) ;
public static native boolean stop () ;
public static native boolean setValue (int what, int value) ;
public static native int getValue (int what) ;
public static native boolean setString (int what, String value) ;
public static native String getString (int what) ;
}
