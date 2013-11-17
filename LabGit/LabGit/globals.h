#ifndef GLOBALS_H
#define GLOBALS_H

//Includes
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

//Structs
typedef enum opcde{ ADD, SUB, ADDI, MULT, LW, SW, BEQ, HALT } OPCODE;
typedef struct instruction {
	OPCODE opcode;
	int rs;
	int rd;
	int rt;
	int immediate;
} instr;

typedef struct latch_structure {
	char branchPendingFlag;
	char halt;
	char writable; //enabled
	OPCODE opcode;

	int readData1;
	int readData2;
	int regDest;
	int immediate;

	int ALUresult;
	int processed;
	int PC;
} latch;





//Globals
int clk;
int branchDistance;
char eoSimulation;
int PC;
int memTimeRemaining;
int exeTimeRemaining;

int IF_tally;
int ID_tally;
int EXE_tally;
int MEM_tally;
int WB_tally;

latch IF_ID;
latch ID_EXE;
latch EXE_MEM;
latch MEM_WB;

instr* instructionMem;
int regfile[25][2];
int memory[512];

int instructionCount;

//Constants
int MEM_ACCESS_TIME;
int EXE_MULT_TIME;
int EXE_OP_TIME;




#endif // !GLOBALS_H
