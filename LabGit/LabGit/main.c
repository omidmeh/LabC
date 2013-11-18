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


char* file = "prog1.asy";
int EXE_MULT_TIME   = 1;	//m
int EXE_OP_TIME     = 1;	//n
int MEM_ACCESS_TIME = 1;	//c

//Function Declaration
void pipeline();


int main(int argc, char* argv[]) {
	if (argc == 2) file = argv[1];
	if (argc == 5) {
		file			= argv[1];
		EXE_MULT_TIME	= strtol(argv[2], NULL, 10);
		EXE_OP_TIME		= strtol(argv[3], NULL, 10);
		MEM_ACCESS_TIME = strtol(argv[4], NULL, 10);
	}
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
	printf("OPCODE: %2d, RS: %2d, RT: %2d, RD: %2d, IMM: %d\n",
		(*ptr).opcode, (*ptr).rs, (*ptr).rt, (*ptr).rd, (*ptr).immediate);

	//testEXE();
	//zachTest();
	//testMEM();

	pipeline();

	terminate();
	return 0;
}

void pipeline(){

	IF_ID.writable = 1;
	ID_EXE.writable = 1;
	EXE_MEM.writable = 1;
	MEM_WB.writable = 1;


	regfile[4][0] = 5;
	regfile[5][0] = 5;

	clk = 0;
	while (eoSimulation == 0){
		WB(); 
		MEM();
		EXE();
		ID(); 
		IF(); 
		printAllLatch();

		//puts("\n\n\n");
		if (MEM_WB.halt == 0) clk++;
	}

	printf("IF_tally  = %d\n" , IF_tally);
	printf("ID_tally  = %d\n" , ID_tally);
	printf("EXE_tally = %d\n", EXE_tally);
	printf("MEM_tally = %d\n", MEM_tally);
	printf("WB_tally  = %d\n", WB_tally);
	printf("Cycle #   = %d\n", clk);
	return;
}