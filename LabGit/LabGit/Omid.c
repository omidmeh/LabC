#define _CRT_SECURE_NO_DEPRECATE

#include "globals.h"
#include "Omid.h"
#include "Zach.h"
#include "Shehzeen.h"



char* validInstruc[] = { "add", "sub", "addi", "mul", "lw", "sw", "beq", "haltSimulation" };
char* registers[] = { "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1",
					  "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3",
					  "$s4", "$s5", "$s6", "$s7", "$t8", "$t9" };

void testEXE(){
	//initialization
	int i;
	for (i = 0; i < 25; i++){
		regfile[i][0] = 2 * i;
	}

	ID_EXE.writable = 0;
	EXE_MEM.writable = 0;



	ID_EXE.branchPendingFlag = 0;
	ID_EXE.opcode = SUB;
	ID_EXE.readData1 = -2;
	ID_EXE.readData2 = 7;
	ID_EXE.regDest = 15;
	ID_EXE.immediate = 13;
	ID_EXE.PC = 0;
	ID_EXE.halt = 0;
	ID_EXE.ALUresult = 0;
	ID_EXE.processed = 0;

	printLatch(&EXE_MEM, 1);
	EXE_MEM.writable = 1;
	EXE();
	printLatch(&EXE_MEM, 0);
	EXE();
	printLatch(&EXE_MEM, 0);
	EXE();
	printLatch(&EXE_MEM, 0);
	EXE();
	printLatch(&EXE_MEM, 0);
	EXE();
	printLatch(&EXE_MEM, 0);
	EXE();
	printLatch(&EXE_MEM, 0);
	printLatch(&EXE_MEM, 0);
	EXE();
	printLatch(&EXE_MEM, 0);

}

void MEM(){
	if (EXE_MEM.writable == 1)	return; //Old Data
	if (EXE_MEM.halt == 1)		return;
	if (EXE_MEM.processed == 0){
		if (memTimeRemaining == 0)
		{
			switch (EXE_MEM.opcode)
			{
			case(SW) :
			case(LW) :
					 {
						 memTimeRemaining = MEM_ACCESS_TIME - 1;
						 MEM_tally++;
						 break;
					}
			case(ADD) :
			case(ADDI) :
			case(MULT) :
			case(BEQ) :
			case(SUB) :
			case(HALT) :
					  {memTimeRemaining = 0; break; }
			default:
				{printf("Unknown case in mem condition test."); break; }
			}
		}
		else{
			memTimeRemaining--;
			MEM_tally++;
		}


		if (memTimeRemaining == 0){
			if (MEM_WB.writable == 1){
				updateMEM_WB();
			}
			else{
				EXE_MEM.processed = 1;
				return;
			}
		}
	}
	else{
		if (MEM_WB.writable == 1) updateMEM_WB();
	}
	return;
}


char* progScanner(char* address) {
	FILE* fp;
	char* str = (char*)malloc(100 * sizeof(char));
	char* begPtr;
	instr* nextInstructPtr = instructionMem;

	fp = fopen(address, "r");

	while (fgets(str, 100, fp) != NULL) {
		begPtr = str;

		while (*begPtr == ' ')
			begPtr = begPtr + 1;

		if (begPtr[strlen(begPtr) - 1] == '\n')
			begPtr[strlen(begPtr) - 1] = '\0';

		if (*begPtr == '\0' || *begPtr == '\r') continue;

		if (begPtr[strlen(begPtr) - 1] == '\r')
			begPtr[strlen(begPtr) - 1] = '\0';

		if (*begPtr == '\0') continue;

		*nextInstructPtr = Parser(begPtr);
		instructionCount++;
		nextInstructPtr++;
	}
	fclose(fp);
	return "Scanning Completed.\n";
}

instr Parser(char* instruction) {
	int i = 0;
	char* delims = " ,";
	char* tempstr = (char*)malloc(300 * sizeof(char));
	instr tempinstruct;

	tempinstruct.opcode = (OPCODE)-1;
	strcpy(tempstr, instruction);


	strtok(tempstr, delims);

	for (i = 0; i < 8; i++)
	if (strcmp(validInstruc[i], tempstr) == 0) {
		tempinstruct.opcode = (OPCODE)i;
		break;
	}

	if (tempinstruct.opcode == -1) {
		printf("Error: Invalid OPCODE. (%s) %s\n", tempstr, instruction);
		terminate();
	}

	switch (tempinstruct.opcode) {
	case(ADD) :
	case(SUB) :
	case(MULT) : {
		assert(tempstr != NULL);
		tempstr = strtok(NULL, delims);
		tempinstruct.rd = regToInt(tempstr);

		tempstr = strtok(NULL, delims);
		tempinstruct.rs = regToInt(tempstr);

		tempstr = strtok(NULL, delims);
		tempinstruct.rt = regToInt(tempstr);

		tempinstruct.immediate = -1;
		break;
	}

	case(ADDI) : {
		assert(tempstr != NULL);

		tempstr = strtok(NULL, delims);
		tempinstruct.rt = regToInt(tempstr);

		tempstr = strtok(NULL, delims);
		tempinstruct.rs = regToInt(tempstr);

		tempinstruct.rd = -1;

		tempstr = strtok(NULL, delims);
		if (isNumeric(tempstr) == 0){
			printf("immediate is not integer: (%s) in \"%s\"", tempstr, instruction);
			terminate();
		}
		int temp = (int)strtol(tempstr, NULL, 10);
		if (temp > 32767 || temp < -32768) {
			printf("immediate out of bound: %d", temp);
			terminate();
		}
		tempinstruct.immediate = temp;
		break;
	}


	case(LW) :
	case(SW) : {
		assert(tempstr != NULL);
		tempstr = strtok(NULL, delims);
		tempinstruct.rt = regToInt(tempstr);

		tempstr = strtok(NULL, " ,()");
		if (isNumeric(tempstr) == 0){
			printf("immediate is not integer: %s", tempstr);
			terminate();
		}

		int temp = (int)strtol(tempstr, NULL, 10);
		if (temp > 32767 || temp < -32768) {
			printf("immediate out of bound: %d", temp);
			terminate();
		}
		tempinstruct.immediate = temp;

		tempstr = strtok(NULL, " ,()");
		tempinstruct.rs = regToInt(tempstr);

		tempinstruct.rd = -1;
		break;
	}

	case(BEQ) : {
		assert(tempstr != NULL);
		tempstr = strtok(NULL, delims);
		tempinstruct.rs = regToInt(tempstr);

		tempstr = strtok(NULL, delims);
		tempinstruct.rt = regToInt(tempstr);

		tempstr = strtok(NULL, delims);
		if (isNumeric(tempstr) == 0)
			printf("immediate is not integer: %s", tempstr);

		int temp = (int)strtol(tempstr, NULL, 10);
		if (temp > 32767 || temp < -32768) {
			printf("immediate out of bound: %d", temp);
			terminate();
		}
		tempinstruct.immediate = temp;
		tempinstruct.rd = 0;
		break;
	}

	case(HALT) : {
		tempinstruct.rs = 0;
		tempinstruct.rd = 0;
		tempinstruct.rt = 0;
		tempinstruct.immediate = 0;
		break;
	}

	default: {
		puts("case Undefined.\n");
		terminate();
			 }

	}

	return tempinstruct;
}

int isNumeric(const char * s) {
	if (s == NULL || *s == '\0' || *s == ' ')
		return 0;
	char * p;
	strtod(s, &p);
	return (*p == '\0' || *p == '\n');
}

int regToInt(char* s) {
	if (*s != '$'){
		printf("regiter name undefined: (%s)", s);
		terminate();
	}
	int temp;
	assert(s != NULL);
	//if (s == NULL) return -1;

	if (isNumeric(s + 1)) {								//ignore the $ sign
		temp = (int)strtol(s + 1, NULL, 10);
		if (temp<0 || temp>25) {
			printf("Rigister out of bound: %s", s);
			terminate();
		}
		return temp;
	}


	int i = 0;
	for (i; i < 26; i++) {
		if (strcmp(registers[i], s) == 0) return i;
	}

	printf("Register name undefined: %s", s);
	terminate();
	return -1;
}

void printLatch(latch* lat, short printTitle){


	if (printTitle == 1)
		printf("BPend  halt  wable  op  rData1  rData2  Dest  imm  ALURes  Pr  PC\n");
	printf("%5d  %4d  %5d  %2d  %6d  %6d  %4d %3d  %6d  %3d  %2d\n",
		(*lat).branchPendingFlag,
		(*lat).halt,
		(*lat).writable,
		(*lat).opcode,
		(*lat).readData1,
		(*lat).readData2,
		(*lat).regDest,
		(*lat).immediate,
		(*lat).ALUresult,
		(*lat).processed,
		(*lat).PC
		);

}

void printAllLatch(){
	printLatch(&IF_ID, 1);
	printLatch(&ID_EXE, 0);
	printLatch(&EXE_MEM, 0);
	printLatch(&MEM_WB, 0);
}

int terminate(){
	scanf("Program terminated. Press any key to exit.");
	exit(1);
	return 0;
}
