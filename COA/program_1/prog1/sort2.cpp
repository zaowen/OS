/************************************************************************
   Program: Tapes are Cool
   Author:  Zachary Owen
   Class: Computer Org and Arc
   Instructor: Dr/Karrlsson
   Date: Feb 19th 2016
   Description: Sorts a list of numbers using three psudo-tape drives
   Input: tape.bin name of a binary file of number to sort
   Output: Tape.bin will be sorted
   Compilation instructions: cc sort2.cpp
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


//Function Prototype
void sort(int& mask, fstream& src, fstream& high, fstream& low , char* srcname );


/*

*/
int main ( int argc, char *argv[] )
{

	fstream tape[3];
	char tp1[] = "tape2.bin";
	char tp2[] = "tape3.bin";

	//Just incase the other files dont exist
	ofstream touch;
	touch.open( tp1 ); // create tape2.bin
	touch.close();
	touch.open( tp2 ); // create tape3.bin
	touch.close();

	//open buffers to tape drives
	tape[0].open(argv[1],ios::in | ios::out | ios::binary);
	tape[1].open( tp1 ,ios::trunc | ios::in | ios::out | ios::binary);
	tape[2].open( tp2 ,ios::trunc | ios::in | ios::out | ios::binary);

	//make sure all of our tape drives are open
	if( tape[0] and tape[1] and tape[2] )
	{

		//initialize vars
		int mask = 0x01;
		int big =0;
		int temp;
		int iter;

		//read first
		temp = read(tape[0]);

		//find biggest and do lowest order radix sort
		while( !tape[0].eof() ) 
		{
			//put it in the little box
			if( (temp & mask)  == 0)
			{
				write(tape[1] , temp);
			}
			else//put it in the big box
			{
				write( tape[2] , temp);
			}
			//get biggest val
			if(temp > big)
			{
				big = temp;
			}
			temp = read( tape[0] );	
		}

		//add delimiter
		write( tape[2] , -1 );

		//rewind odd bin
		rewind( tape[2], tp2 );

		//write the high order onto the low order bin
		temp = read( tape[2] );
		while( temp != -1 )
		{
			write( tape[1] , temp);
			temp = read( tape[2] );
		}


		//initialize number of iterations
		iter = 0;
		//find bitwidth of largest value
		while( big != 0) 
		{
			//shift and count bits
			big = big >> 1;
			iter++;
		}
		iter--;

		//increase mask
		mask = mask << 1;		

		//reset buffers
		rewind( tape[0], argv[1] );
		rewind( tape[1], tp1 );
		rewind( tape[2], tp2 );

		//run sort using tape[2] as source
		//We run this sort so in the while loop we can run an even number of sorts and not check which bin the sorted list ends up in
		sort( mask, tape[1], tape[2], tape[0], tp2 );
		
		iter--;

		//keep running  sorts and rewinds until we are done. 
		while( iter > 0 )
		{	
		//reset buffers
		rewind( tape[0], argv[1] );
		rewind( tape[1], tp1 );
		rewind( tape[2], tp2 );
		
		//run sort into bin 1
		sort( mask, tape[0], tape[2], tape[1], tp2 );
		iter--;	
		
		//reset buffers
		rewind( tape[0], argv[1] );
		rewind( tape[1], tp1 );
		rewind( tape[2], tp2 );
		
		//run sort dumping into bin 0
		sort( mask, tape[1], tape[2], tape[0], tp2 );
		iter--;
		}

	}
	else
	{	
		//error
		cout << "could not open files\n" ;
	}
}



/*
sort:

This function performs a single iteration in a binary Radix sort this fucntion
takes the values from src until eof and places the high and low order bits in
to their respective streams. It divides them using the mask varialbe which is 
ANDed with each value in the stream. Mask is shifted at the end of the 
function since the next time sort() is called we will want a shifted mask.

Highname is used to rewind the high stream since it's values are written to low at the end.
*/
void sort(int& mask, fstream& src, fstream& high, fstream& low , char* highname )
{
	//declare variable
	int temp = 0;

	//do our radix sort
	temp = read( src );
	while( !src.eof() ) 
	{
		if( (temp & mask)  == 0)
		{
			write( low , temp);
		}
		else
		{
			write( high , temp);
		}

		temp = read( src );
	}

	//rewind high drive and place a delimiter
	//the delimiter is needed since we cannot clear the high
	//drive and the quantity of values in high is variable
	write( high, -1 );
	rewind( high , highname );

	//place high values on top of the low ones
	temp = read( high );
	while( temp != -1 )
	{
		write( low , temp);
		temp = read( high );
	}
	mask = mask << 1;

}
