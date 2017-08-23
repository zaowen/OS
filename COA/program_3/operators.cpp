#include "operators.h"
#include "globals.h"

#define _READMEM 1
#define _WRITEMEM 2

#define _AM_DIR 1
#define _AM_IMM 2
#define _AM_IND 4
#define _AM_IDR 8
#define _AM_IDI 16
#define _AM_ALL 31
#define _NOP_ 0
#define _ADD_ 1
#define _SUB_ 2
#define _COM_ 3
#define _CLR_ 4
#define _AND_ 5
#define _OR_  6
#define _XOR_ 7
#define _ADDX_ 8
#define _SUBX_ 9
#define _CLRX_ 10

/************************************************************************
Function: RunMEM
Author: Zacharious Owen
Description: Performs a memory operation based on the WRITE or READ input
Parameters:
		MemAcc- Tell the function to do a read or write 
			Based on #def's at the top.
 ************************************************************************/
void RunMEM( char MemAcc )
{

	if( MemAcc == 0 )
	{
		return;
	}

	MDR = DBUS;
	MAR = ABUS;

	if( MemAcc == _READMEM )// read
	{
		MDR = RAM[MAR];	
		DBUS = MDR;
	}
	if ( MemAcc == _WRITEMEM )//write
	{
		RAM[MAR] = MDR;
	}


	return;

}

/************************************************************************
Function: RunALU
Author: Zacharious Owen
Description: Performs an Arithmetic Logical Unit operation on AC and DBUS 
		as operands. THE OpX operations take their second operand
		from the ABUS
		NOTE: Compliment functions performs a 1's Compliment
Parameters: operand:
		Determines what operation is to be done;
 ************************************************************************/
void RunALU( char operand )
{
	switch( operand ){
		case _ADD_:
			ALU = DBUS + AC;
			break;

		case _SUB_:
			ALU = AC - DBUS;
			break;	

		case _COM_:
			ALU = ~AC;
			break;	

		case _CLR_:
			AC = 0;
			break;	

		case _AND_:
			ALU = AC & DBUS;
			break;	

		case _OR_:
			ALU = AC | DBUS;
			break;	

		case _XOR_:
			ALU = AC ^ DBUS;
			break;		

		case _ADDX_:
			ALU = XR[ABUS] + (DBUS );
			break;		

		case _SUBX_:
			ALU = XR[ABUS] + (( ~DBUS) + 1);
			break;		

		case _CLRX_:
			XR[ABUS] = 0;
			break;		

		default:
			break;	
	}
}

/************************************************************************
Function: getEA (get Effective Address)
Author: Zacharious Owen
Description: This function takes an struct instruction along wiht a character 
		that details which addressing modes are allowed. 
		The function returns the EA for functions that require an 
		address in RAM rather than the value. This function is 
		seperate from getVal() for sake of modularity in Immediate
		Addressing mode.
		Modes are determined by their bit:
			1: Direct Access
			2: Immediate
			3: Indexed
			4: Indirect
			5: Indexed Indirect
			6-8: Ignored
		This function also checks and sets the PSR Global bits 
		if the function attempts to 
Parameters: instruc:
		Parsed struct containing the parts of the opcode currently
		being executed
	allowed:
		Char containing the correct adressing mode bits from the 
		operator calling getEA()
************************************************************************/
int32_t getEA( instruction &instruc, char allowed )
{
	bool good = false;
	//check that the given mode is allowed by instruction
	for( int i = 0; i < 4 ; i ++)//iterate over _AM_MODE[]
	{
		if( ( (allowed >> i) & 0x1) == 1 && instruc.mode == _AM_MODE[i])
		{ 	//make sure addressing mode is allowed and existant
			good = true;
		}
	}

	if( good )
	{
		switch( instruc.mode )
		{
			case 0://Direct
				return EA = instruc.addr;
				break;

			case 1://Immediate (cause error
				PSR = 0;
				UAR = 0;
				break;

			case 2://Idexed
				EA = instruc.addr + XR[instruc.regn];
				return EA;
				break;

			case 4:// Indirect
				ABUS = instruc.addr;
				RunMEM( _READMEM );
				EA = (DBUS >> 12);
				return EA;
				break;
			case 6://Indirect Indexed
				ABUS = instruc.addr + XR[instruc.regn];
				RunMEM( _READMEM );
				EA = ( DBUS >> 12 );
				return EA;
				break;

			default: //this shouldn't happen
				UAR = 0;	
				break;	
		}
	}
	else//Improper accessing mode
	{
		PSR = 0;
		UAR = 0;
		return 0; 
	}
	return 0;	
}

/************************************************************************
Function: getVal
Author: Zacharious Owen
Description: This function takes an struct instruction along wiht a character 
		that details which addressing modes are allowed. 
		The function returns the Value for functions that require an 
		value dereferenced from an adressing mode. this function is
		seperate from getEA() for sake of modularity in Immediate
		Addressing mode.
		Modes are determined by their bit:
			1: Direct Access
			2: Immediate
			3: Indexed
			4: Indirect
			5: Indexed Indirect
			6-8: Ignored
		This function also checks and sets the PSR Global bits 
		if the function attempts to 
Parameters: instruc:
		Parsed struct containing the parts of the opcode currently
		being executed
	allowed:
		Char containing the correct adressing mode bits from the 
		operator calling getEA()
************************************************************************/
int32_t getVal( instruction &instruc, char allowed )
{
	bool good = false;
	//check that the given mode is allowed by instruction
	for( int i = 0; i < 4 ; i ++)//iterate over _AM_MODE[]
	{
		if( ( (allowed >> i) & 0x1) == 1 && instruc.mode == _AM_MODE[i])
		{ 	//make sure addressing mode is allowed and existant
			good = true;
		}
	}

	if( good )
	{
		switch( instruc.mode )
		{
			case 0: // Direct
				ABUS = instruc.addr;
				EA = ABUS;
				RunMEM( _READMEM );
				return DBUS;
				break;

			case 1: // IMEDIATE
				return instruc.addr;
				break;

			case 2:// INDEXED 
				ABUS = instruc.addr + XR[instruc.regn];
				EA = ABUS;
				RunMEM( _READMEM);
				return DBUS;
				break;

			case 4://Indirect
				ABUS = instruc.addr;
				RunMEM( _READMEM);
				ABUS = DBUS;
				EA = ABUS;
				RunMEM( _READMEM);
				return DBUS;
				break;

			case 6://INDIRECT INEXED
				ABUS = instruc.addr + XR[instruc.regn];
				RunMEM( _READMEM);
				ABUS = DBUS;
				EA = ABUS;
				RunMEM( _READMEM);
				return DBUS;
				break;

			default: //this shouldn't happen
				PSR = 0;
				UAR = 0;	
				break;	
		}
	}
	else
	{
		PSR = 0;
		UAR = 0;
		return 0; 
	}
	return 0;	
}


/************************************************************************
OPERAND (op) FUNCTIONS
Description: Each one is an implemtation of a hardware operation.
Parameters: 
	instruction * instruct:
		Holds information needed to perform operation
		most frequently used is the adressing mode and address 
		contence.
************************************************************************/

/************************************************************************
Function:opHALT
Author: Zacharious Owen
Description: Sets halt bits and performs a opNOP
 ************************************************************************/
void opHALT( instruction * instruct)
{
	PSR = 0;
	HR = 0;
	opNOP( instruct );
}


/************************************************************************
Function: opMISS
Author: Zacharious Owen
Description: Used if an reserved opcode is used to break main loop 
 ************************************************************************/
void opMISS( instruction * instruct )
{
	PSR = 0;
	OPR = 0;

}


/************************************************************************
Function: opNOP
Author: Zacharious Owen
Description: Most important function in entire program trust me
 ************************************************************************/
void opNOP( instruction * instruct )
{
	RunALU( _NOP_ );
}


/************************************************************************
Function: opLD
Author: Zacharious Owen
Description: Loads value from Memory to AC
 ************************************************************************/
void opLD( instruction * instruct )
{
	AC = getVal( *instruct, _AM_ALL );	
}


/************************************************************************
Function: opADD
Author: Zacharious Owen
Description: Add memory to accumulator 
Addressing mode: ALL
 ************************************************************************/
void opADD 	( instruction * instruct  )
{
	DBUS = getVal( *instruct, _AM_ALL );
	RunALU( _ADD_ );
	AC = ALU;
}


/************************************************************************
Function: opSUB
Author: Zacharious Owen
Description: Subtract memory from the accumulator
Addressing mode: ALL
 ************************************************************************/
void opSUB	(instruction * instruct )
{
	DBUS = getVal( *instruct, _AM_ALL);

	RunALU( _SUB_ );

	AC = ALU;
}


/************************************************************************
Function: opJ
Author: Zacharious Owen
Description: Jump to specified memory address
Addressing Mode: All except Immediate
 ************************************************************************/
void opJ	(instruction * instruct )
{
	IC = getEA( *instruct, _AM_DIR | _AM_IND | _AM_IDR | _AM_IDI ) - 1;
}


/************************************************************************
Function: opJN
Author: Zacharious Owen
Description: Jump to memory address if the accumulator contains zero
Addressing Mode: All except Immediate
 ************************************************************************/
void opJN	(instruction * instruct )
{
	DBUS = getEA( *instruct, _AM_DIR | _AM_IND | _AM_IDR | _AM_IDI ) - 1;

	if( (( AC >> 23) & 0x1 ) == 0x1 )
		IC = DBUS; 
}	

/************************************************************************
Function:opJP
Author: Zacharious Owen
Description: Jump to the memory address if the accumulator contains a postitive 
		number.
Addressing Mode: All except Immediate
 ************************************************************************/
void opJP	(instruction * instruct )
{
	DBUS = getEA( *instruct, _AM_DIR | _AM_IND | _AM_IDR | _AM_IDI ) - 1;

	if( AC >  0 )
		IC = DBUS; 
}	

/************************************************************************
Function: opJZ
Author: Zacharious Owen
Description: Jump to the memory address if the accumulator contains a negative
		number
Addressing Mode: All except Immediate
 ************************************************************************/
void opJZ	(instruction * instruct )
{
	DBUS = getEA( *instruct, _AM_DIR | _AM_IND | _AM_IDR | _AM_IDI ) - 1;

	if( AC ==  0 )
		IC = DBUS; 
}	

/************************************************************************
Function: opST
Author: Zacharious Owen
Description:Store Accumulator into memroy
Addressing Mode: All except Immediate
 ************************************************************************/
void opST	(instruction * instruct )
{
	DBUS = AC;
	ABUS = getEA( *instruct, _AM_DIR | _AM_IND | _AM_IDI | _AM_IDR);

	RunMEM( _WRITEMEM );
}


/************************************************************************
Function: opCLR
Author: Zacharious Owen
Description: Clear the Accumulator
Addressing Mode: Ignored
 ************************************************************************/
void opCLR	(instruction * instruct )
{
	RunALU( _CLR_ );	
}

/************************************************************************
Function: opEM
Author: Zacharious Owen
Description: Exchange the accumulator with memory
Addressing Mode: All except Immediate
 ************************************************************************/
void opEM	( instruction * instruct )
{
	//STORE AC IN ALU
	DBUS = 0;
	RunALU( _ADD_ );	

	//GET FROM MEMORY
	AC = getVal( *instruct,  _AM_DIR | _AM_IND | _AM_IDI | _AM_IDR);

	//STORE PREV-AC TO MEMORY
	DBUS = ALU;
	ABUS = getEA( *instruct,  _AM_DIR | _AM_IND | _AM_IDI | _AM_IDR);
	RunMEM( _WRITEMEM );
}


/************************************************************************
Function: opCOM
Author: Zacharious Owen
Description: Compliment the Accumulator ( 1's compliment)
Addressing Mode: Ignored
 ************************************************************************/
void opCOM	( instruction * instruct )
{
	RunALU( _COM_ );
	AC = ALU;
}

/************************************************************************
Function: opAND
Author: Zacharious Owen
Description: AND memory to the A-Q-Mew-Later
Addressing Mode: All 
 ************************************************************************/
void opAND	( instruction * instruct )
{
	DBUS = getVal( *instruct, _AM_ALL );
	RunALU( _AND_ );
	AC = ALU;
}

/************************************************************************
Function: opOR
Author: Zacharious Owen
Description: OR Memory to the akumulator
Addressing Mode: All 
 ************************************************************************/
void opOR	( instruction * instruct )
{
	DBUS = getVal( *instruct, _AM_ALL );
	RunALU( _OR_ );
	AC = ALU;
}

/************************************************************************
Function: opXOR
Author: Zacharious Owen
Description: XOR Memory to the Accumulatormagator
Addressing Mode: All 
 ************************************************************************/
void opXOR	( instruction * instruct )
{
	DBUS = getVal( *instruct, _AM_ALL );
	RunALU( _XOR_ );
	AC = ALU;
}

/************************************************************************
Function: opLDX
Author: Zacharious Owen
Description: Load the specified index register from teh upper half of a memory
		Word
Addressing Mode: Direct or Immediate
 ************************************************************************/
void opLDX	( instruction * instruct )
{
	XR[ instruct->regn ] = (getVal( *instruct, _AM_DIR | _AM_IMM ) >> 12);
	XR[ instruct->regn ] = XR[ instruct->regn ] & 0xfff;
}

/************************************************************************
Function: opADDX
Author: Zacharious Owen
Description: Add Memory to the specified index register. Uses lower twelve
		bits as second operand due to unclear program specifications
		and sloth
Addressing Mode: Direct
 ************************************************************************/
void opADDX	( instruction * instruct )
{
	DBUS = getVal( *instruct, _AM_DIR | _AM_IMM );
	ABUS = instruct->regn;
	RunALU( _ADDX_ );
	XR[ instruct->regn ] = ALU;
	XR[ instruct->regn ] = XR[ instruct->regn ] & 0xfff;
}

/************************************************************************
Function: opSTX
Author: Zacharious Owen
Description: Store the specefied index register into the upper half of a 
		memory word
Addressing Mode: Direct
 ************************************************************************/
void opSTX	( instruction * instruct )
{
	
	DBUS = (XR[ instruct->regn] << 12 );
	ABUS = getEA( *instruct, _AM_DIR | _AM_IND | _AM_IDI | _AM_IDR);

	RunMEM( _WRITEMEM );
	XR[ instruct->regn ] = XR[ instruct->regn ] & 0xfff;
	
}

/************************************************************************
Function: opSUBX
Author: Zacharious Owen
Description: Subtract memory from the specified index register. Since the 
		Documentation was unclear the lower 12 bits are used for the
		Second operand
Addressing Mode: Direct or Immediate
 ************************************************************************/
void opSUBX	( instruction * instruct )
{
	DBUS = getVal( *instruct, _AM_DIR | _AM_IMM );
	ABUS = instruct->regn;
	RunALU( _SUBX_ );
	XR[ instruct->regn ] = ALU;
	XR[ instruct->regn ] = XR[ instruct->regn ] & 0xfff;

}

/************************************************************************
Function: opEMX
Author: Zacharious Owen
Description: Exchange the specified index register wiht the upper half of 
		a memory word
Addressing Mode: Direct 
 ************************************************************************/
void opEMX	( instruction * instruct )
{
	//store old value
	ALU = XR[ instruct->regn ];
	
	//Get from memory
	XR[ instruct ->regn] = (getVal( *instruct , _AM_DIR ) >> 12 );
	
	int temp = ( getVal( *instruct, _AM_DIR) & 0xfff);
	DBUS = (ALU << 12);
	DBUS = DBUS + temp;
	ABUS = getEA( *instruct, _AM_DIR );
	RunMEM( _WRITEMEM);
	XR[ instruct->regn ] = XR[ instruct->regn ] & 0xfff;
	
}

/************************************************************************
Function: opCLRX
Author: Zacharious Owen
Description: Clear the specified index register
Addressing Mode: IGNORED
 ************************************************************************/
void opCLRX	( instruction * instruct )
{
	ABUS = instruct->regn;
	RunALU( _CLRX_);
}


