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
	//testMEM();

	pipeline();

	scanf("press enter");
	return 0;
}

void pipeline(){

	IF_ID.writable = 1;
	ID_EXE.writable = 1;
	EXE_MEM.writable = 1;
	MEM_WB.writable = 1;

	printAllLatch();
	while (eoSimulation == 0){
		//printAllLatch();
		WB();
		printAllLatch();
		MEM();
		printAllLatch();
		EXE();
		printAllLatch();
		ID();
		printAllLatch();
		IF();
		//ID_EXE.writable = 1;
		printAllLatch();
		puts("\n\n\n");
	}

	return;
}