#ifndef OMID_H
#define OMID_H

#include "globals.h"

instr Parser(char* instruction);
int isNumeric(const char * s);
char* progScanner(char* address);
int regToInt(char* s);
int terminate();

void testEXE();
void MEM();

void printLatch(latch* lat, short printTitle);
void printAllLatch();


#endif // !OMID_H
