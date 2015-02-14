/*
Copyright (c) 2011-2012, Quentin Cosendey
This code is part of universal speech which is under multiple licenses.
Please refer to the readme file provided with the package for more information.
*/
#ifndef _____SRAPI_PRIVATE____
#define _____SRAPI_PRIVATE____

typedef struct {
const void* name;
int(*isAvailable)(void) ;
int(*say)(const void*, int) ;
int(*stop)(void);
int(*braille)(const void*);
int(*setValue)(int, int) ;
int(*getValue)(int) ;
int(*setString)(int, const void*);
const void*(*getString)(int);
int(*unload)(void);
} engine;

#endif
