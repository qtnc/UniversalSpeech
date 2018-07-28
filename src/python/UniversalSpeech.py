import ctypes as __ctypes
__uspeech = __ctypes.cdll.UniversalSpeech

VOLUME, VOLUME_MAX, VOLUME_MIN, VOLUME_SUPPORTED, \
RATE, RATE_MAX, RATE_MIN, RATE_SUPPORTED, \
PITCH, PITCH_MAX, PITCH_MIN, PITCH_SUPPORTED, \
INFLEXION, INFLEXION_MAX, INFLEXION_MIN, INFLEXION_SUPPORTED, \
PAUSED, PAUSE_SUPPORTED, \
BUSY, BUSY_SUPPORTED, \
WAIT, WAIT_SUPPORTED \
	= range(0,22)

ENABLE_NATIVE_SPEECH = 0xFFFF
VOICE = 0x10000
LANGUAGE = 0x20000
SUBENGINE = 0x30000
ENGINE = 0x40000
ENGINE_AVAILABLE = 0x50000
AUTO_ENGINE = 0xFFFE
USER_PARAM = 0x1000000


def say (msg, interrupt=True): return __uspeech.speechSay(msg, interrupt)
def sayA(msg, interrupt=True): return __uspeech.speechSayA(msg, interrupt)

def braille (msg): return __uspeech.brailleDisplay(msg)

#Added by Paulius LEveris <paulius.leveris@gmail.com>:
def speech(msg):
	"""Allows speech and braille in one place"""
	say(msg);braille(msg)

def speechA(msg):
	"""Allows to use sayA and braille in one place."""
	sayA(msg);braille(msg)

def stop () : return __uspeech.speechStop()

def getValue (what) : return __uspeech.speechGetValue(what)

def setValue (what, value): return __uspeech.speechSetValue(what, value)

def getString (what):
	__uspeech.speechGetString.restype = __ctypes.c_wchar_p
	return __uspeech.speechGetString(what)	
