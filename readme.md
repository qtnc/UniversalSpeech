# Universal speech

# 1. Description
Universal speech goal is to unify and simplify the access to speech in applications. Speech can be done through active screen readers, direct synthesis or native/OS speech engines, depending on what is available and supported.
Currently in its version 1.0, universal speech is only for windows, but I really hope that people will find it useful and port it to linux, macintosh, and why not mobile phones, tablets and other portable devices.
If you want to contribute, contact me using the contact form at <http://quentinc.net/contact>
This page is in french but should be sufficiently understandable.
Note: e-mail removed from readme since it appears in clear on the github page

A page of explanations in french is available on my website: <http://quentinc.net/universalspeech>

## Universal speech features :

* Universal set of functions that automatically connect to the best suitable screen reader or speech synthesis engine found, and automatically switch to another one when the current one become unavailable
* Set speech parameters when available: speech rate, pitch, voice, language, etc.
* Ability to query for supported or available engine names and capabilities such as speech rate, pitch, voices, languages, etc.
* Define if native speech engines that can in principle never fail such as SAPI on windows must be used if nothing else is found, or if speech must simply be ignored in that case

## Supported operating systems :

* Windows

Supported engines on windows :

* Jaws for windows, version 10 or above via FSAPI (installed by default), or vversion 5 to 9  via jfwapi.dll COM control
* NVDA 2011.1 or above, using nvdaControllerClient.dll
* Windows eye, using COM control provided by GWM
* System access, using saapi32.dll
* Supernova, using dolapi.dll
* Cobra, partially, using COM control provided by Baume
* ZangDu Screen reader (ZDSR)
* SAPI 5, using MS standard C/WIN32 API 

# 2. Using universal speech

## 2.1. From C/C++
The C/C++ distribution includes an header file UniversalSpeech.h and an import library libUniversalSpeech.a that can be used by MinGW compiler
To use universal speech in MSVC, simply rename libUniversalSpeech.a in UniversalSpeech.lib and import it normally.
The header file contains function prototypes and their documentation.

## 2.2. From Java
Import the provided class quentinc.UniversalSpeech. You can use it out of the box.
You can look at Test.java as an example .

## 2.3. From Python 3
Take the file UniversalSpeech.py and import it as usual. It works out of the box.

## 2.4. Using the COM interface on windows
Before you can use the COM interface to the screen reader API DLL, you must first register it using regsvr32.exe by issuing the command: regsvr32 UniversalSpeech.dll. The type library UniversalSpeech.tlb must be in the same directory when doing the registration and you must also distribute it with your application.
Then, use the ProgID "UniversalSpeech.Interface" to create an object. 
Note: The DLL registration mainly consists of creating the required registry entries in HKEY_LOCAL_MACHINE\Software\Classes\UniversalSpeech.Interface  and the corresponding CLSIDs. Once the DLL is registered, you aren't supposed to move or rename it. Place it in a consist folder before registering.

An HTML application, test.hta, is provided with an example call in JScript.
Some other examples would be welcome :

* WSF/WScript
* VB6/VBA
* C#/VB.net using COM

## 2.5. From a language with an FFI library
Most languages provide an FFI library to access native C/C++ DLLs. You can use them to access the API with your favorite language.

* Lua: example in test.lua that run with luajit 2.x with the included FFI library (does not work with standard lua 5.x)

Examples in one or more of these languages would be welcome :

* Ruby
* C#/VB.net,  using C wrappers/interop
* Other less popular languages ?

# 3. Download 

You can download latest precompiled DLLs here, containing both 32 and 64 bit versions:
http://vrac.quentinc.net/UniversalSpeech.zip

- 64 bit version has been built with MinGW-W64 / GCC 13 on 2024-04-08
- 32 bit version has been built with MinGW-W64 / GCC 8 on 2022-07-31

# 4. Known issues

## 4.1. Keyboard hook
Keyboard hook could be used to bypass jaws keyboard hook that could be problematic for games. The most known problem of that sort is arrow keys not working or with a delay.
The hook installed by your application must be explicitely uninstalled at exit. You should also uninstall it when the user switches to another application (Alt+Tab), and reinstall it when he comes back to it.
Other applications wont work correctly with jaws when the custom keyboard hook is active and average users wont understand what's going on if it remain active outside of your application.
Note that the hook may not work as expected. Use this feature as your own risk.

## 4.2. Jaws sleep mode
When jaws is running but in sleep mode, it is still detected by ScreenReaderAPI, allthough nothing can be spoken through it.  Its successful detection prevent another screen reader or SAPI5 from being used.
IN that case, the only solution is to force the use of another engine.
Sleep mode can't be detected properly by ScreenReaderAPI, and is highly discouraged by freedom scientific.

## 4.3. SAPI5 issue
For an unknown reason, it is impossible to retriev number of voices available, get voice names, or get or set current SAPI5 voice on certain computers.
That has already been noticed at least once on windows 7 64 bits. This is probably because UniversalSpeech is basicly compiled in 32 bits (you can probably recompile it in 64 bits if you want), and because there are conflicts between 32 and 64 bits voices.
Any other information about that problem is welcome.

## 4.4. Registering the COM server in an inno setup script
For an unknown reason, inno setup scripts fail to register UniversalSpeech when using the regserver flag in the files section (it returns an error #5)
A work around is to explicitly tell the script to run regsvr32.exe at the end of the installation, in the run section.
This problem concern both 32 and 64 bit versions of windows.

# 5. Copyright and license
Copyright � 2011-2018, Quentin Cosendey http://quentinc.net/

UniversalSpeech is using MIT license.
[Read full license texte in LICENSE.txt](LICENSE.txt).
