#include "globals.h"
#include "Zach.h"
#include "Omid.h"




void IF()
{

	if (IF_ID.opcode == HALT) return;

	if (IF_ID.branchPendingFlag == 1)
	{
		IF_ID.branchPendingFlag = 0;
		return;
	}

	if (IF_ID.branchPendingFlag == 2) return;
	if (IF_ID.writable == 0) return;

	int nextInstruction;

	nextInstruction = PC + branchDistance;

	branchDistance = 0;

	IF_ID.PC = nextInstruction;

	PC = (nextInstruction + 1);

	if (instructionMem[nextInstruction].opcode != HALT) IF_tally++;

	IF_ID.writable = 0;
}

void ID()
{
	OPCODE opcode = instructionMem[IF_ID.PC].opcode;
	int    rs = instructionMem[IF_ID.PC].rs;
	int    rt = instructionMem[IF_ID.PC].rt;
	int    rd = instructionMem[IF_ID.PC].rd;
	int    immediate = instructionMem[IF_ID.PC].immediate;

	//if (ID_EXE.halt == HALT) return;     //  halted
	if (IF_ID.halt == 1) return;     //  halted
	if (ID_EXE.writable == 0)    return;     //  not writable
	if (IF_ID.writable == 1) return;		//No new Data

	

	switch (opcode)
	{
	case(ADD) :
	case(SUB) :
	case(MULT) :
			  {
				  if (regfile[rs][1] == 1) return;
				  if (regfile[rt][1] == 1) return;

				  regfile[rd][1] = 1;

				  ID_EXE.opcode = opcode;
				  ID_EXE.regDest = rd;
				  ID_EXE.readData1 = regfile[rs][0];
				  ID_EXE.readData2 = regfile[rt][0];
				  ID_EXE.immediate = 0;
				  ID_EXE.writable = 0;

				  ID_tally++;
				  break;
		}
	case(ADDI) :
			   {
				   if (regfile[rs][1] == 1) return;

				   regfile[rt][1] = 1;

				   ID_EXE.opcode = opcode;
				   ID_EXE.regDest = rt;
				   ID_EXE.readData1 = regfile[rs][0];
				   ID_EXE.readData2 = 0;
				   ID_EXE.immediate = immediate;

				   IF_ID.writable = 1;
				   ID_EXE.writable = 0;

				   ID_tally++;
				   break;
		}
	case(LW) :
			 {
				 if (regfile[rs][1] == 1) return;

				 assert(regfile[rs][0] % 4 == 0);

				 regfile[rt][1] = 1;

				 ID_EXE.opcode = opcode;
				 ID_EXE.regDest = rt;
				 ID_EXE.readData1 = regfile[rs][0];
				 ID_EXE.readData2 = 0;
				 ID_EXE.immediate = immediate;

				 IF_ID.writable = 1;
				 ID_EXE.writable = 0;

				 ID_tally++;
				 break;
		}
	case(SW) :
			 {
				 if (regfile[rt][1] == 1)  return;
				 if (regfile[rs][1] == 1) return;

				 assert(regfile[rs][0] % 4 == 0);

				 ID_EXE.opcode = opcode;
				 ID_EXE.regDest = 0;
				 ID_EXE.readData1 = regfile[rt][0];		//	content to be stored
				 ID_EXE.readData2 = regfile[rs][0];		//	base address
				 ID_EXE.immediate = immediate;

				 IF_ID.writable = 1;
				 ID_EXE.writable = 0;

				 ID_tally++;
				 break;
		}
	case(BEQ) :
			  {
				  if (regfile[rs][1] == 1) return;
				  if (regfile[rt][1] == 1) return;

				  ID_EXE.opcode = opcode;
				  ID_EXE.regDest = 0;
				  ID_EXE.readData1 = regfile[rs][0];
				  ID_EXE.readData2 = regfile[rt][0];
				  ID_EXE.immediate = immediate;

				  IF_ID.writable = 1;
				  IF_ID.branchPendingFlag = 2;
				  ID_EXE.writable = 0;

				  ID_tally++;
				  break;
		}
	case(HALT) :
			   {
				   //assert(IF_ID.halt == 1);
				   //ID_EXE.halt = 1;

				   ID_EXE.opcode = opcode;
				   ID_EXE.regDest = 0;
				   ID_EXE.readData1 = 0;
				   ID_EXE.readData2 = 0;
				   ID_EXE.immediate = 0;

				   IF_ID.writable = 0;
				   IF_ID.branchPendingFlag = 0;
				   ID_EXE.writable = 0;

				   IF_ID.halt = 1;
				   break;
		}
	default:
		{
			printf("ERROR: No opcode match in ID stage. Terminating Program... \n");
			terminate();
		   }
	}

	IF_ID.writable = 1;

	//printf("IF_ID.opcode = %d \n", ID_EXE.opcode);
	//printf("IF_ID.regDest = %d \n", ID_EXE.regDest);
	//printf("IF_ID.readData1 = %d \n", ID_EXE.readData1);
	//printf("IF_ID.readData2 = %d \n", ID_EXE.readData2);
	//printf("IF_ID.immediate = %d \n", ID_EXE.immediate);
}

void EXE()
{
	//if (EXE_MEM.opcode == HALT) return;
	if (ID_EXE.halt == 1) return;
	if (ID_EXE.writable == 1)    return;
	if (ID_EXE.processed == 0)
	{
		if (exeTimeRemaining == 0)
		{
			switch (ID_EXE.opcode)
			{
			case(MULT) :
					   {
						   exeTimeRemaining = EXE_MULT_TIME - 1;
						   EXE_tally++;
						   break;
				}
			case(ADD) :
			case(SUB) :
			case(ADDI) :
			case(LW) :
			case(SW) :
			case(BEQ) :
					  {
						  exeTimeRemaining = EXE_OP_TIME - 1;
						  EXE_tally++;
						  break;
				}
			case(HALT) :
					   {
						   //assert(IF_ID.halt == 1);
						   //assert(ID_EXE.halt == 1);
						   //EXE_MEM.halt = 1;
						   //ID_EXE.halt = 1;
						   exeTimeRemaining = 0;
						   break;
				}
			default:
				{
					printf("ERROR: No opcode match in EXE stage. Terminating Program... \n");
					terminate();
				   }
			}
		}
		else
		{
			exeTimeRemaining--;
			EXE_tally++;
		}


		if (exeTimeRemaining == 0)
		{
			if (EXE_MEM.writable == 1)
				updateEXE_MEM();
			else
			{
				ID_EXE.processed = 1;
				return;
			}
		}
		//EXE_tally++;
	}
	else
	{
		if (EXE_MEM.writable == 1)
			updateEXE_MEM();
	}
}

void updateEXE_MEM()
{
	OPCODE opcode = ID_EXE.opcode;
	int regDest = ID_EXE.regDest;
	int readData1 = ID_EXE.readData1;
	int readData2 = ID_EXE.readData2;
	int immediate = ID_EXE.immediate;

	switch (opcode)
	{
	case(ADD) :
			  {
				  //add contents of rs and rt and store in rd (rd = rs + rt)
				  EXE_MEM.opcode = opcode;
				  EXE_MEM.regDest = regDest;    //  rd
				  EXE_MEM.readData1 = 0;
				  EXE_MEM.readData2 = 0;
				  EXE_MEM.immediate = 0;
				  EXE_MEM.ALUresult = (readData1 + readData2);

				  ID_EXE.writable = 1;
				  EXE_MEM.writable = 0;

				  //EXE_tally++;
				  break;
		}
	case(SUB) :
			  {
				  //subtract contents of rt from rs and store in rd (rd = rs - rt)
				  EXE_MEM.opcode = opcode;
				  EXE_MEM.regDest = regDest;    //  rd
				  EXE_MEM.readData1 = 0;
				  EXE_MEM.readData2 = 0;
				  EXE_MEM.immediate = 0;
				  EXE_MEM.ALUresult = (readData1 - readData2);

				  ID_EXE.writable = 1;
				  EXE_MEM.writable = 0;

				  //EXE_tally++;
				  break;
		}
	case(MULT) :
			   {
				   //multiply contents of rt and rs and store in rd (rd = rs * rt)
				   EXE_MEM.opcode = opcode;
				   EXE_MEM.regDest = regDest;
				   EXE_MEM.readData1 = 0;
				   EXE_MEM.readData2 = 0;
				   EXE_MEM.immediate = 0;
				   EXE_MEM.ALUresult = (readData1*readData2);

				   ID_EXE.writable = 1;
				   EXE_MEM.writable = 0;

				   //EXE_tally++;
				   break;
		}
	case(ADDI) :
			   {
				   //add contents of rs and immediate and store in rt (rt = rs + immediate)
				   EXE_MEM.opcode = opcode;
				   EXE_MEM.regDest = regDest;
				   EXE_MEM.readData1 = 0;
				   EXE_MEM.readData2 = 0;
				   EXE_MEM.immediate = 0;
				   EXE_MEM.ALUresult = (readData1 + immediate);

				   ID_EXE.writable = 1;
				   EXE_MEM.writable = 0;

				   //EXE_tally++;
				   break;
		}
	case(LW) :
			 {
				 EXE_MEM.opcode = opcode;
				 EXE_MEM.regDest = regDest;                // rt
				 EXE_MEM.readData1 = readData1 + immediate;    // M[rs]+immediate
				 EXE_MEM.readData2 = 0;
				 EXE_MEM.immediate = 0;

				 ID_EXE.writable = 1;
				 EXE_MEM.writable = 0;

				 //EXE_tally++;
				 break;
		}
	case(SW) :
			 {
				 EXE_MEM.opcode = opcode;
				 EXE_MEM.regDest = 0;
				 EXE_MEM.readData1 = readData1;              //	M[rt]
				 EXE_MEM.readData2 = readData2 + immediate;    //	M[rs]+immediate
				 EXE_MEM.immediate = 0;

				 ID_EXE.writable = 1;
				 EXE_MEM.writable = 0;

				 //EXE_tally++;
				 break;
		}
	case(BEQ) :
			  {
				  EXE_MEM.opcode = opcode;
				  EXE_MEM.regDest = 0;
				  EXE_MEM.readData1 = readData1;              //  M[rs]
				  EXE_MEM.readData2 = readData2;              //  M[rt]
				  EXE_MEM.immediate = immediate;

				  if (EXE_MEM.readData1 == EXE_MEM.readData2)
				  if (readData1 == readData2)
				  {
					  //set branch offset to immediate
					  branchDistance = immediate;
				  }
				  else
				  {
					  //set branch offset to 0
					  branchDistance = 0;
				  }

				  IF_ID.branchPendingFlag = 1;
				  ID_EXE.writable = 1;
				  EXE_MEM.writable = 0;

				  //EXE_tally++;
				  break;
		}
	case(HALT) :
			   {
				   assert(IF_ID.halt == 1);
				   //assert(ID_EXE.halt == 1);
				   //EXE_MEM.halt = 1;
				   EXE_MEM.opcode = opcode;
				   EXE_MEM.regDest = 0;
				   EXE_MEM.readData1 = 0;
				   EXE_MEM.readData2 = 0;
				   EXE_MEM.immediate = 0;
				   EXE_MEM.ALUresult = 0;

				   ID_EXE.writable = 1;
				   EXE_MEM.writable = 0;
				   ID_EXE.halt = 1;
				   break;
		}
	default:
		{
			printf("ERROR: No opcode match in the update EXE stage. Terminating Program... \n");
			terminate();
		   }
	}
}

void zachTest()
{
	//initialize all latches to writable
	IF_ID.writable = 1;
	ID_EXE.writable = 1;
	EXE_MEM.writable = 1;
	MEM_WB.writable = 1;

	//initialize all register file values
	regfile[0][0] = 0;
	regfile[1][0] = 1;
	regfile[2][0] = 2;
	regfile[3][0] = 3;
	regfile[4][0] = 4;
	regfile[5][0] = 5;
	regfile[6][0] = 6;
	regfile[7][0] = 7;
	regfile[8][0] = 8;
	regfile[9][0] = 9;
	regfile[10][0] = 10;
	regfile[11][0] = 11;
	regfile[12][0] = 12;
	regfile[13][0] = 13;
	regfile[14][0] = 14;
	regfile[15][0] = 15;
	regfile[16][0] = 16;
	regfile[17][0] = 17;
	regfile[18][0] = 18;
	regfile[19][0] = 19;
	regfile[20][0] = 20;
	regfile[21][0] = 21;
	regfile[22][0] = 22;
	regfile[23][0] = 23;
	regfile[24][0] = 24;
	regfile[25][0] = 25;

	IF();
	ID();
}
