#ifndef OPERATORS_H
#define OPERATORS_H

#include "globals.h"

/* It's 32 bits, because the standard type int is a lot easier than using 3 chars and it's nice to have word aligned memory on the host.  Plus, we have plenty of memory to spare, so I don't mind wasting 8 bits */
struct instruction
{
	// 12 bit address (3 hex numbers)
	// 2 bits specify the column
	// 4 bits specify the row
	// 6 remaining bits specify the addressing mode
	uint32_t herp : 8;
	uint32_t addr : 12;
	uint32_t icol : 2;
	uint32_t irow : 4;
	uint32_t mode : 4;
	uint32_t regn : 2;
};

//Meta operations on ALU and Memory functions
void RunMEM( char MemAcc );
void RunALU( char operand);

//Funtions for grabing pointers and values
int32_t getEA( instruction &instruc, char allowed );
int32_t getVal( instruction &instruc, char allowed );

//Massive 
void opMISS	( instruction * instruct );
void opHALT	( instruction * instruct );
void opNOP	( instruction * instruct );
void opLD 	( instruction * instruct );
void opADD 	( instruction * instruct );
void opSUB 	( instruction * instruct );
void opJ 	( instruction * instruct );
void opJN	( instruction * instruct );
void opJZ	( instruction * instruct );
void opJP	( instruction * instruct );
void opST	( instruction * instruct );
void opCLR	( instruction * instruct );
void opEM	( instruction * instruct );
void opCOM	( instruction * instruct );
void opAND	( instruction * instruct );
void opOR	( instruction * instruct );
void opXOR	( instruction * instruct );
void opLDX	( instruction * instruct );
void opADDX	( instruction * instruct );
void opSTX	( instruction * instruct );
void opSUBX	( instruction * instruct );
void opEMX	( instruction * instruct );
void opCLRX	( instruction * instruct );

#endif
