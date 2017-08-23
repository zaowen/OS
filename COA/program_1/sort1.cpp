/************************************************************************
Program: Tapes are Dumb
Author:  Zachary Owen
Class: Computer Org and Arc
Instructor: Dr/Karrlsson
Date: Feb 19th 2016
Description: Sort's a list of numbers using a psudo tape drive
Input: tape.bin name of a binary file of number to sort
Output: Tape.bin will be sorted
Compilation instructions: cc sort1.cpp
Usage: a.out tape.bin
Known bugs/missing features: DOES NOT HANDLE NEGATIVE VALUES
 ************************************************************************/
#include<iostream>
#include<fstream>


using namespace std;

//Tape I/O functions written by Doc Karlsson
// User defined functions
// Function to read a single element from the tape
int read(fstream& tape)
{
	int data;
	tape.read(reinterpret_cast<char*>(&data), sizeof(int));
	return data;
}
// Function to rewaind the tape to the beginning
void rewind(fstream& tape, char *arg)
{
	tape.close();
	tape.open(arg,ios::in | ios::out | ios::binary);
}
// Function to write a single element to the tape
void write(fstream& tape, int data)
{
	tape.write(reinterpret_cast<char*>(&data), sizeof(int));
}

//Convinient memory structure that allows passing of our whole memory space to functions easily and without using globals
struct memory{
	int index; // current index
	int start; // first working index
	int end;   // last  working index
	int temp;  // buffer
	int big;   // largest val
	int bdex;  // index of largest val
	int small; // smallest val
	int sdex;  // index of smallest
	int first; // value at start
	int last;  // value at end
};

//ok heres the thing. i need the ability to rewind the tape inside of the inside of functions
// and i really dont feel like passing around a pointer to argv[1] so its a global.
char * tapename;

//Function Brototypes
void gotostart( memory& RAM, fstream& tape);
void check( memory& RAM);
void find ( memory& RAM, fstream& tape);
void  set ( memory& RAM, fstream& tape);

int main ( int argc, char *argv[] )
{
	//tape to read from
	fstream tape;
	//memory space
	memory RAM;	

	//check that we have an argument
	if ( argc != 2 )
	{
		printf ( "Usage: %s <filename>\n" , argv[0] );
		return(0);
	}

	//open the tape
	tape.open(argv[1],ios::in | ios::out | ios::binary);

	// First read of the tape
	if(tape.is_open()) // Were we able to open the tape?
	{	

		tapename = argv[1];


		//init some vals
		RAM.start = 0;
		RAM.index = RAM.start;

		//read first value set big and small
		RAM.temp = read(tape);
		RAM.first = RAM.temp;
		RAM.big = RAM.temp;
		RAM.small = RAM.temp;	
		RAM.bdex = RAM.start;
		RAM.sdex = RAM.start;

		//do inital read iteration, diffrent from subsequent find()'s because we need last value and index
		RAM.temp = read( tape );
		while( !tape.eof() )
		{
			//read from tape
			RAM.index = RAM.index + 1;
			check( RAM );
			RAM.last = RAM.temp;
			RAM.temp = read( tape );
		}
		//save end value;
		RAM.end = RAM.index;

		//place Biggest and smallest and replace displaced all over the place
		set( RAM, tape);

		//we only need to sort between the first and last value
		while( RAM.start < RAM.end )
		{	
			//find new biggest and smallest
			find( RAM, tape );
			//set biggest smallest in places narrow search feild
			set( RAM, tape);
		}
	}
}

/*
gotostart: 

Simple function to put the tape drive head at the first value we need to check 
 */
void gotostart( memory& RAM, fstream& tape)
{
	rewind( tape, tapename);
	for( int i = 0; i < RAM.start; i++)
		read(tape);	
	RAM.index = RAM.start;
}

/*
Find:

Function that reads through the tape stream and finds both the largest and smallest values on the tape 
between values in the memory structure RAM.start and RAM.end which is nice since theres another function
that places those values in the correct spots and adjusts start and end accordingly

Note: in theory this could be faster if i were to check if there were only 2 values i could either swap
or not. But thats less modular.
 */
void find ( memory& RAM, fstream& tape)
{
	//go to starting index
	gotostart( RAM, tape );

	//read first value from tape
	RAM.temp = read(tape);

	//initialize
	RAM.first = RAM.temp;
	RAM.big = RAM.temp;
	RAM.small = RAM.temp;	
	RAM.bdex = RAM.index;
	RAM.sdex = RAM.index;

	//Check temp buffer. see check()
	check( RAM );
	while( RAM.index < RAM.end )
	{
		RAM.temp = read( tape );
		//cout << "checking index: " << RAM.index << endl;
		RAM.index = RAM.index + 1;
		check( RAM );
		RAM.last = RAM.temp;
	}

}

/*
set

This fucntion is the oposite and brother function of Find(). It takes the values found in a find() call
and places them onto the tape. Only runs through the tape once. All it does is finds the indecies 
of the biggest and smallest values and writes the values on the edges into the middle spots except if
one of the edge values is either the max or min in which case it gets complicated but dealt with.
*/
void  set ( memory& RAM, fstream& tape)
{
	//go to start of working tape
	gotostart( RAM, tape);

	//write lowest value and inc index
	write( tape , RAM.small ); 
	RAM.index = RAM.start + 1;


	//replace values
	while( RAM.index < RAM.end )
	{
		//if this index is where the biggest number is found write smallest or first value
		if( RAM.index == RAM.bdex )
		{
			if( RAM.last != RAM.small )
				write(tape , RAM.last );
			else
				write(tape, RAM.first);
			RAM.index++;
		}
		else if( RAM.index == RAM.sdex ) // if its the small index write something else
		{
			if( RAM.first != RAM.big )
				write(tape , RAM.first );	
			else 
				write(tape, RAM.last);
			RAM.index++;
		}
		else
		{ // if its neither keep reading
			read(tape);
			RAM.index++;
		}

	}
	//write biggest value
	write( tape , RAM.big );
	
	//narrow our field of search
	RAM.start++;
	RAM.end--;
}

//Simple function to find if our current value is bigger or smaller than the biggest and smallest and save indexes
void check ( memory& RAM ){
	if( RAM.temp > RAM.big)
	{
		RAM.big = RAM.temp;
		RAM.bdex = RAM.index;
	}	
	else if ( RAM.temp < RAM.small )
	{
		RAM.small = RAM.temp;
		RAM.sdex = RAM.index;
	}

}

