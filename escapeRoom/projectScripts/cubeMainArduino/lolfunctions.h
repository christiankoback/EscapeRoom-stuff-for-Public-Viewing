#ifndef LOLFUNCTIONS
#define LOLFUNCTIONS


#include <Arduino.h>
#include <avr/pgmspace.h>  //AVR library for writing to ROM
#include <Charliplexing.h> 
//Imports the library, which needs to be
//Initialized in setup.


extern void setupLoLShield();
extern void ClearShield();
extern void turnOnLetter(char letter);
extern void turnOnSentence(String sentence);

#endif
