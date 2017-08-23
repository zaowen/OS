/************************************************************************
Program: B17 Simulator
Author: Zachary Owen and the Pussycats
Class: Computer Organization and Architecture CSC317
Instructor: Dr. Karlsson
Date: April 29 2016
Description: Simulate B17 computer
Input: Hexedecimal program file
Compilation instructions: g++ -std=c++11 b17.cpp globals.cpp operators.cpp
Usage: ./b17 <programfile>
 ************************************************************************/
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <cmath>
#include <sstream>
#include "operators.h"


// Some bitmasks for clearing 12-bit and 24-bit registers
#define _REG_12_MAX 4095
#define _REG_24_MAX 16777215

//Used to tell the Memory write function which operaton to do
//RunMEM is called once in the stupid file so they need to be here.
#define _READMEM 1
#define _WRITEMEM 2

/**********************************************
  EXTERNS
 **********************************************/
/* Were going to simulate physical registers by putting them in the heap as globals */
/* Were also going to use 32 bit ints, and just ignore the top 8 bits, as you know, because they don't exist on the B17 */
uint32_t ABUS = 0; //Default state on startup is 0
int32_t   AC = 0;
uint32_t  ALU = 0;
uint32_t DBUS = 0;
uint32_t   IC = 0;
uint32_t   IR = 0;
uint32_t  MAR = 0;
uint32_t  MDR = 0;
uint8_t	  PSR = 1; //Program Status Register
uint8_t    HR = 1; //Halt flag
uint8_t	  OPR = 1; //Bad OP flag
uint8_t   UAR = 1; //Bad Address flag
uint8_t   SEG = 1; //Bad Address
uint32_t   EA = 0; //Added holder for Effective address
uint32_t XR[4]={0,0,0,0};

uint32_t RAM[4095];
char _AM_MODE[5] = {0,1,2,4,6};

void (*opArr[16][4])( instruction * ) = { 
	{	opHALT,	opLD,	opADD,	opJ 	},
	{ 	opNOP,	opST,	opSUB,	opJZ 	},
	{	opMISS,	opEM,	opCLR,	opJN 	},
	{	opMISS,	opMISS,	opCOM,	opJP	},
	{	opMISS,	opMISS,	opAND,	opMISS	},
	{	opMISS,	opMISS,	opOR,	opMISS	},
	{	opMISS,	opMISS,	opXOR,	opMISS	},
	{	opMISS,	opMISS,	opMISS,	opMISS	},
	{	opMISS,	opLDX,	opADDX,	opMISS	},
	{	opMISS,	opSTX,	opSUBX,	opMISS	},
	{	opMISS,	opEMX,	opCLRX,	opMISS	},
	{	opMISS,	opMISS,	opMISS,	opMISS	},
	{	opMISS,	opMISS,	opMISS,	opMISS	},
	{	opMISS,	opMISS,	opMISS,	opMISS	},
	{	opMISS,	opMISS,	opMISS,	opMISS	},
	{	opMISS,	opMISS,	opMISS,	opMISS	}
};

using namespace std;


/* FUNCTION PROTOTYPES */
uint32_t countNumberOfLinesInFile(char* filename);
void decodeToInstruction(instruction &instruct);
void runInstruction(instruction $instruct );
int32_t loadProgram(char* filename, uint32_t numLines);
void startProgram(uint32_t addresssOfFirstInstruction );
void printTrace(instruction instruct, uint32_t address, uint32_t instruction, ostream &out);
void printAddressMode( instruction instruct );
void haltingOutput( );


/****************************************************************************
 * Author: Elliott Rarden + Allison Bodvig + Anthony Morast
 *
 * This is the main() for the program.  It will read in the command line
 * arguments and then start the eumlator.
 *
 * Parameters:
 *	argc - The number of arguments passed from the command line
 *	argv - Char[]'s for each argument
 ****************************************************************************/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "The B17 simulation requires only an object file as a parameter." << endl;
		return 0;
	}

	uint32_t numlines = countNumberOfLinesInFile(argv[1]);
	int32_t start_addr = loadProgram(argv[1], numlines);

	if( start_addr != -1)
	{
		startProgram(start_addr );
		return 0;
	}

	return 1;
}

/***************************************************************************
 * Author: Elliott Rarden + Allison Bodvig + Anthony Morast(Zach is great)
 *
 * startProgram will take in the first instruction, and run the program starting
 * from that point.  There is a debug mode, you can enable by setting the second
 * argument to be true in main(), and recompiling.
 *
 * Parameters:
 *	addressOfFirstInstruction - The address of the first instruction
 *	debugMode - A boolean value to enable/disable debug mode.
 ******************************************************************************/
void startProgram(uint32_t addresssOfFirstInstruction )
{
	IC = addresssOfFirstInstruction;
	uint32_t prev = 0;
	instruction instruc;

	// run the program until an error occurs or HALT is executed
	while ( PSR )
	{
		prev = IC;
		decodeToInstruction(instruc); //parse instruction
		runInstruction(instruc); // run instruction
		IC = IC +1;
		printTrace(instruc, prev, RAM[prev], cout); // print the trace
		if( IC > _REG_12_MAX )
		{
			PSR = 0;
			SEG = 0;
		}
	}
	haltingOutput( );

}


/******************************************************************************
 * Author: Elliott Rarden + Allison Bodvig + Anthony Morast But mostly Zach
 *
 * decodeToInstruction reads the given 32-bit integer, parses it, and fills out
 * the appropriate fields in &instruct.
 *
 * Parameters:
 *	instruct - The structure, which is assumed to be empty, then filled out
 *	and returned code - A 32-bit integer to be examined.
 ******************************************************************************/
void decodeToInstruction(instruction &instruc)
{
	uint32_t mask[5];
	mask[0] = 0x3C; 	// mode
	mask[1] = 0x3C0; 	// irow
	mask[2] = 0xC00; 	// icol
	mask[3] = 0xFFF000;   	// addr
	mask[4] = 0x3; 		//IndexRegister
	IR = RAM[IC];	

	instruc.addr = ( IR & mask[3] ) >> 12;
	instruc.icol = ( IR & mask[2] ) >> 10;
	instruc.irow = ( IR & mask[1] ) >>  6;
	instruc.mode = ( IR & mask[0] ) >>  2;
	instruc.regn = ( IR & mask[4] ) >>  0;

}

/******************************************************************************
 * Author: Zacharious Owen
 *
 * printTrace will print some information about the instruction executed and
 * some register values AFTER the execution of the instruction. This is printed
 * after each instruction. There is some special casing here due to illegal
 * addressing modes for particular instructions.
 *
 * Parameters:
 *	instruction - holds information about the instruction such as the address,
 *		       the addressing mode, and the instruction itself
 * 	address - the address of the current instruction in RAM
 * 	instruction - the instruction its self as a hex string
 *	ostream - output stream
 ******************************************************************************/
void printTrace(instruction instruct, uint32_t address, uint32_t instruction, ostream &out)
{
	int i = 0;
	uint32_t op;
	op = instruct.icol;
	op = op +  (instruct.irow << 2) ;

	cout << std::setw(3) << setfill('0') << std::hex << address << ":  " ;
	cout << std::setw(6) << setfill('0') << std::hex << instruction << " ";

	while( INSTRUCTION_NUMBER_ARRAY[i] != op && i != 22 )
	{
		i++;

	}	

	cout << std::setw(6) << setfill(' ') << INSTRUCTION_CODE_ARRAY[ i ]; 

	if( i == 4  || i == 0 )
	{
		cout << "     ";
	}
	else
	{
		printAddressMode( instruct );
	}

	cout << "AC[" << setw(6) << setfill('0') << (AC & 0xffffff) << "] ";
	cout << "X0[" << setw(3) << setfill('0') << XR[0] << "] ";
	cout << "X1[" << setw(3) << setfill('0') << XR[1] << "] ";
	cout << "X2[" << setw(3) << setfill('0') << XR[2] << "] ";
	cout << "X3[" << setw(3) << setfill('0') << XR[3] << "] ";

	cout << endl;
}

/******************************************************************************
 * Author: Elliott Rarden + Allison Bodvig + Anthony Morast
 *
 * runInstruction takes the parsed hex values from the instruction read from the
 * object file and determines which instruction to execute depending on the
 * instruction and the addressing mode. This function will return the address of
 * the next instruction to make handling jumps easier.
 *
 * Parameters:
 * 	instruc - the instruction parsed into a struct
 *	current_address - the address in RAM of the instruction being executed
 ******************************************************************************/
void runInstruction(instruction instruc)
{
	// Lots of code //psh i dont think so more like one line.
	opArr[ instruc.irow ][ instruc.icol]( &instruc );
}

/******************************************************************************
 * Author: Zachary Owen
 *
 * Reads the object file and reads its contents into ram. Returns the first
 * instruction. 
 * FINAL NUMBER MUST PRECEED A NEWLINE I CANNOT STRESS THIS ENOUGH
 * 
 *
 * Parameters:
 *	filename - The name of the file
 * 	numLines - the number of lines in the file
 ******************************************************************************/
int32_t loadProgram(char* file, uint32_t numLines)
{
	ifstream fin;
	int memLoc;
	int perline;	
	int temp;
	string ss;

	fin.open( file );

	if( fin )
	{
		for( int i = 0 ; i < _REG_12_MAX; i++) // Clear RAM
		{
			RAM[i] = 0;
		}

		for( unsigned int i = 0; i < numLines - 1; i++ ) //Begin REading lines
		{

			fin >> ss;
			if( !fin) // check for values
			{
				cout << "ERROR FOUND IN INPUT FILE: IMPROPER FORMAT \nExiting.\n";
				fin.close();
				exit(1);
			}
			memLoc = stoul( ss , nullptr, 16 ); // convert from base 16
			if(memLoc > 0xfff)
			{
				cout << "ERROR FOUND IN INPUT FILE: INDEX OVERFLOW \nExiting.\n";
				fin.close();
				exit(1);
			}

			fin >> perline; // get num valuse in line

			if( !fin ) // if fin.bad in last statement ss holds 
			{
				break;	
			}

			for( int j = 0; j < perline and fin ; j++ ) //Read perline numbers
			{
				fin >> ss;
				temp = stoul(ss , nullptr, 16);
				RAM[memLoc] = temp;
				memLoc++;
			}
			if( !fin)
			{
				cout << "ERROR FOUND IN INPUT FILE: IMPROPER FORMAT \nExiting.\n";
				fin.close();
				exit(1);
			}

			perline = 0;
		}
		fin.close();
		return (uint32_t)memLoc; //returns start address
	}
	else
	{
		cout << "Could not open input file " << file << ".\nExiting";
		return (uint32_t)-1;
	}
	cout << "Misc Error\n";
	return -1;
}


/******************************************************************************
 * Author: Elliott Rarden + Allison Bodvig + Anthony Morast
 *
 * countNumberOfLinesInFile does exactly that
 *
 * Parameters:
 *	filename - The name of the file
 ******************************************************************************/
uint32_t countNumberOfLinesInFile(char* filename)
{
	ifstream f(filename);
	string line;
	int i;
	for (i = 0; getline(f, line); ++i);
	f.close();
	return i;
}


/************************************************************************
Function: haltingOutput
Author: Zacharious Owen
Description: prints an erro message based on set PSR flags
Parameters: None
 ************************************************************************/
void haltingOutput( )
{
	cout << "Machine Halted - ";

	if( !HR )
	{
		cout << "HALT instruction executed\n";
	}
	else if ( !OPR )
	{
		cout << "undefined opcode\n";
	}
	else if ( !UAR )
	{
		cout << "illegal addressing mode\n";
	}
	else if ( !SEG )
	{
		cout << "Segmentation Fault\n";
	}

}

/************************************************************************
Function: printAddressMode
Author: Zacharious Owen
Description: Prints The proper address mode for the Trace function. EA
Needs to be Set during excecution of OPCODE. Which it should
Hopfully
 ************************************************************************/
void printAddressMode( instruction instruct )
{
	int mode = ( instruct.mode);

	if( UAR == 0)
	{
		cout << " ??? ";
		return;
	}

	if( mode != 1)
	{
		cout << " " << std::hex << setw(3) << setfill('0') << EA << " " ;
	}
	else
	{
		cout << " IMM ";
	}
}

