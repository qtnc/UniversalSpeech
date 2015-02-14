#define UNICODE
#include<stdio.h>
#include<wchar.h>
#include<windows.h> // Sleep
#include "../include/UniversalSpeech.h"

int main (int argc, char** argv) {
speechSetValue(SP_ENABLE_NATIVE_SPEECH, TRUE);

printf("Here is a list of supported engines : \r\n");
printf("%-6.6s. %-50.50s, %-10.10s\r\n", "ID", "Name", "AVailable");

const wchar_t* name;
int i;
for (i=0; name = speechGetString(SP_ENGINE+i); i++) {
int avail = speechGetValue(SP_ENGINE_AVAILABLE+i);
printf("%-5d. %-50.50ls, %-10.10s\r\n", i, name, avail?"yes":"no");
}

while(1){
wchar_t msg[256];
const wchar_t* engineName = speechGetString(SP_ENGINE + speechGetValue(SP_ENGINE));
snwprintf(msg, 255, L"You are using %s.", engineName);
speechSay(msg, TRUE);
Sleep(3000);
}
return 0;
}

