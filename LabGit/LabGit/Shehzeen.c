#include "globals.h"
#include "Shehzeen.h"
#include "Omid.h"
//


void updateMEM_WB(){
	switch (EXE_MEM.opcode)
	{
	case(SW) :
			 {
				 assert(EXE_MEM.readData2 % 4 == 0);
				 assert((EXE_MEM.readData2 / 4) < 512 &&
						(EXE_MEM.readData2 / 4) >= 0);

				 memory[EXE_MEM.readData2 / 4] = EXE_MEM.readData1;

				 MEM_WB.opcode = EXE_MEM.opcode;
				 MEM_WB.regDest = 0;
				 MEM_WB.readData1 = 0;
				 MEM_WB.readData2 = 0;
				 MEM_WB.immediate = 0;
				 MEM_WB.ALUresult = 0;

				 EXE_MEM.writable = 1;
				 MEM_WB.writable = 0;



				 break;
			}
	case(LW) :
			 {
				
				 assert((EXE_MEM.readData1) % 4 == 0);
				 assert((EXE_MEM.readData1 / 4) < 512 &&
						(EXE_MEM.readData1 / 4) >= 0);

				 MEM_WB.opcode = EXE_MEM.opcode;
				 MEM_WB.regDest = EXE_MEM.regDest;
				 MEM_WB.readData1 = memory[(EXE_MEM.readData1)/4];
				 MEM_WB.readData2 = 0;
				 MEM_WB.immediate = 0;
				 MEM_WB.ALUresult = 0;

				 EXE_MEM.writable = 1;
				 MEM_WB.writable = 0;



				 break;

				 break;
					}
	case(ADD) :
			  {
				  MEM_WB.opcode = EXE_MEM.opcode;
				  MEM_WB.regDest = EXE_MEM.regDest;
				  MEM_WB.readData1 = 0;
				  MEM_WB.readData2 = 0;
				  MEM_WB.immediate = 0;
				  MEM_WB.ALUresult = EXE_MEM.ALUresult;

				  EXE_MEM.writable = 1;
				  MEM_WB.writable = 0;

				  break;
			}
	case(ADDI) :
			   {
				   EXE_MEM.opcode = EXE_MEM.opcode;
				   MEM_WB.regDest = EXE_MEM.regDest;
				   MEM_WB.readData1 = 0;
				   MEM_WB.readData2 = 0;
				   MEM_WB.immediate = 0;
				   MEM_WB.ALUresult = EXE_MEM.ALUresult;

				   EXE_MEM.writable = 1;
				   MEM_WB.writable = 0;

				   break;
			}
	case(MULT) :
			   {
				   MEM_WB.opcode = EXE_MEM.opcode;
				   MEM_WB.regDest = EXE_MEM.regDest;
				   MEM_WB.readData1 = 0;
				   MEM_WB.readData2 = 0;
				   MEM_WB.immediate = 0;
				   MEM_WB.ALUresult = EXE_MEM.ALUresult;

				   EXE_MEM.writable = 1;
				   MEM_WB.writable = 0;

				   // MEM_tally++;
				   break;
			}
	case(BEQ) :
			  {
				  MEM_WB.opcode = EXE_MEM.opcode;
				  MEM_WB.regDest = 0;
				  MEM_WB.readData1 = EXE_MEM.readData1;
				  MEM_WB.readData2 = EXE_MEM.readData2;
				  MEM_WB.immediate = EXE_MEM.immediate;

				  MEM_WB.branchPendingFlag = 0; //0 or 1?
				  MEM_WB.writable = 0;
				  EXE_MEM.writable = 1;
				  break;
			}
	case(SUB) :
			  {
				  MEM_WB.opcode = EXE_MEM.opcode;
				  MEM_WB.regDest = EXE_MEM.regDest;
				  MEM_WB.readData1 = 0;
				  MEM_WB.readData2 = 0;
				  MEM_WB.immediate = 0;
				  MEM_WB.ALUresult = EXE_MEM.ALUresult;

				  EXE_MEM.writable = 1;
				  MEM_WB.writable = 0;

				  break;
			}
	case(HALT) :
			   {
				   assert(IF_ID.halt == 1);
				   assert(ID_EXE.halt == 1);
				   EXE_MEM.halt = 1;

				   MEM_WB.opcode = EXE_MEM.opcode;
				   MEM_WB.regDest = 0;
				   MEM_WB.readData1 = 0;
				   MEM_WB.readData2 = 0;
				   MEM_WB.immediate = 0;
				   MEM_WB.ALUresult = EXE_MEM.ALUresult;

				   EXE_MEM.writable = 0;
				   MEM_WB.writable = 0;

				   break;

						  }
	default:
		{printf("Unknown case in mem condition test."); break; }
	}
}

void testMEM(){
	int i;
	for (i = 0; i < 25; i++)
		regfile[i][0] = 2 * i;

	EXE_MEM.branchPendingFlag = 0;
	EXE_MEM.opcode = LW;
	EXE_MEM.readData1 = 15;
	EXE_MEM.readData2 = 0;
	EXE_MEM.regDest = 3;
	EXE_MEM.immediate = 0;
	EXE_MEM.PC = 0;
	EXE_MEM.halt = 0;
	EXE_MEM.ALUresult = 0;
	EXE_MEM.processed = 0;

	EXE_MEM.writable = 0;
	MEM_WB.writable = 1;

	regfile[EXE_MEM.regDest][1] = 1;
	memory[EXE_MEM.readData1] = 31;

	printAllLatch();

	printf("RegFile[%d][1]={%d,%d}\n", EXE_MEM.regDest,
		regfile[EXE_MEM.regDest][0], regfile[EXE_MEM.regDest][1]);
	printf("Memory[%d]=%d]\n", EXE_MEM.readData1, memory[EXE_MEM.readData1]);

	printLatch(&MEM_WB, 1);
	MEM();
	printLatch(&MEM_WB, 0);
	MEM();
	printLatch(&MEM_WB, 0);
	MEM();
	printLatch(&MEM_WB, 0);

	printAllLatch();


	printf("RegFile[%d][1]={%d,%d}\n", EXE_MEM.regDest,
		regfile[EXE_MEM.regDest][0], regfile[EXE_MEM.regDest][1]);
	printf("Memory[%d]=%d]\n", EXE_MEM.readData1, memory[EXE_MEM.readData1]);

}

void WB(){

	if (MEM_WB.writable == 1) return;
	else{
	
		MEM_WB.writable = 1;
		switch (MEM_WB.opcode)
		{

		case (LW) :
				  {
					  regfile[MEM_WB.regDest][0] = MEM_WB.readData1;
					  regfile[MEM_WB.regDest][1] = 0;
					  WB_tally++;
					  break;
	  }
		case (ADD) :
		case (SUB) :
		case (ADDI) :
		case (MULT) :
				   {
					   regfile[MEM_WB.regDest][0] = MEM_WB.ALUresult;
					   regfile[MEM_WB.regDest][1] = 0;
					   WB_tally++;
					   break;
	   }

		case (BEQ) : 
		case (SW) : { break; }

		case (HALT) : {
			assert(IF_ID.halt == 1);
			assert(ID_EXE.halt == 1);
			assert(EXE_MEM.halt == 1);
			MEM_WB.halt = 1;
			eoSimulation = 1;
			break;
		   }
		}


	}
}