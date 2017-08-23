#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>



/* Were going to simulate physical registers by putting them in the heap as globals */
/* Were also going to use 32 bit ints, and just ignore the top 8 bits, as you know, because they don't exist on the B17 */
extern uint32_t ABUS; //Default state on startup is 0
extern  int32_t   AC;
extern uint32_t  ALU;
extern uint32_t DBUS;
extern uint32_t   IC;
extern uint32_t   IR;
extern uint32_t  MAR;
extern uint32_t  MDR;
extern uint32_t XR[4];
extern uint8_t 	 PSR; //Program Status Register
extern uint8_t    HR;  //Halt flag
extern uint8_t	 OPR; //Bad OP flag
extern uint8_t   UAR; //Bad Address flag
extern uint8_t   SEG; //Bad Address
extern uint32_t  EA;

/* MEMORY ARRAY OF 32-BIT INTEGERS - IGNORE TOP 8 BITS OF EACH INT */
extern uint32_t RAM[4095];

//Declaration of Instruction struct
struct instruction;

//Holds binary representation of addressing modes
extern char _AM_MODE[5];

// These two arrays make a lookup table.  This would all be done in hardware on the machine
const uint8_t INSTRUCTION_NUMBER_ARRAY[22] = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 14, 15, 18, 22, 26, 33, 34, 37, 38, 41, 42 };
const std::string INSTRUCTION_CODE_ARRAY[23] = { "HALT",
	"LD  ",
	"ADD ",
	"J   ",
	"NOP ",
	"ST  ",
	"SUB ",
	"JZ  ",
	"EM  ",
	"CLR ",
	"JN  ",
	"COM ",
	"JP  ",
	"AND ",
	"OR  ",
	"XOR ",
	"LDX ",
	"ADDX",
	"STX ",
	"SUBX",
	"EMX ",
	"CLRX",
	"????" };
#endif
