@echo off
del ..\..\bin\UniversalSpeech.tlb
cd "C:\temp\Microsoft Visual Studio 9.0\VC\bin\"
copy C:\mingw\progq\universalspeech\src\windows\UniversalSpeech.idl .
midl /I ..\include UniversalSpeech.idl
copy UniversalSpeech.tlb C:\mingw\progq\universalspeech\bin\
cd C:\mingw\progq\universalspeech\src\windows\