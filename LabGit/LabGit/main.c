// main.c : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_DEPRECATE


#include "globals.h"
#include "Zach.h"
#include "Shehzeen.h"
#include "Omid.h"

//OS BASED DEFINITIONS
#ifdef OS_WINDOWS
#include "stdafx.h"
#endif


char* file = "file.txt";
MEM_ACCESS_TIME = 1;
EXE_MULT_TIME = 1;
EXE_OP_TIME = 1;

//Function Declaration
void pipeline();


int main(int argc, char* argv[]) {
	instructionMem = malloc(sizeof(instr*)* 512);

	//Constants infor print
	printf("MEM Access Time = %d \n", MEM_ACCESS_TIME);
	printf("EXE Multip Time = %d \n", EXE_MULT_TIME);
	printf("EXE Others Time = %d \n", EXE_OP_TIME);

	puts(progScanner(file));

	instr* ptr = instructionMem;
	while ((*ptr).opcode != HALT) {
		printf("OPCODE: %2d, RS: %2d, RT: %2d, RD: %2d, IMM: %d\n",
			(*ptr).opcode, (*ptr).rs, (*ptr).rt, (*ptr).rd, (*ptr).immediate);
		ptr++;
	}

	//testEXE();
	//zachTest();
	testMEM();

	scanf("press enter");
	return 0;
}

void pipeline(){
	while (eoSimulation == 0){
		WB();
		MEM();
		EXE();
		ID();
		IF();
	}

	return;
}