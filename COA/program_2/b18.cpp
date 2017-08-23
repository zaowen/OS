/************************************************************************
Program: the NAND ( Nand Array Never Dumb)
Author: Zachary Owen
Class: Computer Organization and Archetecture
Instructor: Dr. Karlsson
Date: March 23 2016
Description: Simulates wired array of NAND gates and produces a full truth Table of results
Input: A file containing 
Output:
Compilation instructions:
Usage:
Known bugs/missing features:
Modifications:
Date                Comment            
----    ------------------------------------------------
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INNUM 8
#define OUTNUM 4
#define WIDTHNUM 10
#define HEIGHTNUM 10

struct NAND_GATE{
	int input1;
	int input2;
	int output;
	int used;
};



void make_result( char * result ) ;
int get_bit( int mask , void * from);
void write_header( int * in , int * out);
void write_output( struct NAND_GATE * GateArray , int * in , int * out , int * wiring );
int * make_wire( FILE * fp );
int check_wire( int * w );
int * get_in_used( int * wires );
int * get_out_used( int * wires );
void set_gates( struct NAND_GATE * a, int * wires );
void run_sim( struct NAND_GATE * GateArray ) ;
void gggg( struct NAND_GATE * GateArray, struct  NAND_GATE * curr);
void set_inputs( struct NAND_GATE * GateArray , int * in , int i );


/************************************************************************
Function: Main()
Author: Zacharious Owen
Description: Sets up the simulation and directs the flow of the program
Parameters:	int argc: count of arguments in argv
char ** argv: array of arrays containing function arguments
 ************************************************************************/
int main( int argc , char ** argv)
{	
	//file that holds input wiring
	FILE * fp;
	//holds the array of nands we are working on.
	struct NAND_GATE GateArray[ INNUM + HEIGHTNUM * WIDTHNUM ];
	//holds where the inputs come from 
	int * wires;
	int i;
	int lim;
	int * in;
	int * out;

	//Check compile time params
	if( INNUM && OUTNUM && WIDTHNUM && HEIGHTNUM )
	{
		//check for correct argumetns
		if( argc > 1 )
		{	
			//open input file
			fp = fopen( argv[1] , "r" );

			//did input file open?
			if( fp != NULL )
			{
				//read wireing by output number create array
				wires = make_wire( fp );

				//set the gate inputs
				set_gates ( GateArray , wires );

				//find used input wires
				in = get_in_used( wires );

				//Check that there are input wires
				if ( in[0] != 0 )	
				{
					//check for output wires
					out = get_out_used( wires );

					//Check that there are output wires	
					if( out[0] != 0 )
					{			
						//Write header of output hoping the rest of the program works
						write_header( in , out );

						//Calculates number of outputs to generate
						lim = (int)pow( 2 , in[0] );

						//Run 2^n Simulations
						for( i = 0; i <  lim ; i = i + 1)
						{
							//set default bools
							set_inputs( GateArray , in , i );
							//do simulations
							run_sim( GateArray );
							//Write
							write_output( GateArray, in , out , wires);
						}		
						//Always free what you malloc
						free( in );
						free( out );
						free( wires );
					} // ERROR HANDLEING TERRACE
					else
					{
						printf( "Zero outputs detected, please check wiring Diagram.\nExiting\n" );
						return -1;
					}
				}
				else
				{
					printf( "Zero inputs detected, please check wiring Diagram.\nExiting\n" );
					return -1;
				}
			}
			else
			{
				printf( "Could not open input file.\nExiting\n");
				return -1;
			}
		}
		else
		{
			printf( "Usage \n\t./b18 <input-file>\n" );
			return -1;
		}
	}
	else
	{
		printf( "Compile time paramaters (J K L M) are invalid,\nPlease Check them\n Exiting\n");
		return -1;
	}
	return 0;
}



/************************************************************************
Function: check_wire
Author: Zachary Owen
Description: does a simple check to see if the wiring diagram created in make_wire() is valid bychecking to see if next inputs wire into previous gates
Parameters: int * w: wiring diagram array where the index in input and value is output
 ************************************************************************/
int check_wire( int * w )
{
	int i;

	for( i = 0; i < HEIGHTNUM * WIDTHNUM * 2; i = i + 1 ) 
	{
		if( w[i] != -1 )
		{
			if( ((w[i] - INNUM )/ HEIGHTNUM)  >= (i/(HEIGHTNUM*2) ) )
			{
				return 0;
			}

		}
	} 
	return 1;
}



/************************************************************************
Function: gggg ( go go gadget gate )
Author: Zacharious Owen
Description: Performs a Single nand on a gate
Parameters: 	GATEARRAY
Pointer to the full array of gates so we can find out inputs
currptr 
Pointer to the nand gate we are currently performing on
 ************************************************************************/
void gggg( struct NAND_GATE * GateArray, struct  NAND_GATE * currptr )
{
	int in1 = 0;
	int in2 = 0;

	struct NAND_GATE curr = * currptr;

	// if in inputs are -1 the gate is dissconnected
	if (curr.input1 != -1)
	{	
		in1 = GateArray[ curr.input1 ].output;
	}

	if (curr.input2 != -1)
	{	
		in2 = GateArray[ curr.input2 ].output;
	}

	curr.output = ( ( ~( in1 & in2) ) & 1 ) ;

	*currptr = curr;

	return;

}

/************************************************************************
Function: get_in_used
Author: Zacharious Owen
Description: Given an array of wires, checks the whole array for values less than J. J being INNUM set as a #def 
Parameters: 	wires:
array of the inputs to the nand array, index being which input and value being where the connection is from
returns
a pointer to an array of size j + 1, the zeroth element is the quanity of input wires that connect to the array
and the following that many values are the indexes of them in the overall gate array.
 ************************************************************************/
int * get_in_used( int * wires )
{
	int j;
	int i;
	int * in;
	int * out;
	//make an array of size number of input wires plus one
	in = malloc( sizeof(int) * ( INNUM + 1) );

	if( in == NULL )
	{
		printf( "Memory allocation Error\nExiting");
		exit(-1);	
	}

	//set all the values to -1 as init
	for ( i = 0; i <= INNUM; i = i+1 )
	{
		in[i] = -1;
	}

	//execept the first, its the counter
	in[0] = 0;

	for( i = 0; i < HEIGHTNUM * WIDTHNUM * 2 ; i = i + 1 )
	{
		// if we find an input connection
		if( wires[ i ]  <  INNUM && wires[i] != -1) // check if we already found one of that type
		{
			if( in[ wires[ i ] + 1 ] == -1 )
			{
				// if we have set it to used
				in[ wires[ i ] + 1] = 1;
				// and incriment number found
				in[0] ++;	
			}
		}
	}	

	//sorts and puts them in the right order
	out = malloc( sizeof( int ) * (in[0] +1)) ;

	j = 0;

	for( int i = 0; i < INNUM + 1; i = i + 1)
	{
		if ( in[i] == 1 )
		{
			out[j+1] = i - 1;
			j++;
		} 
	}

	out[0] = in[0];

	return out;
}



/************************************************************************
Function: get_out_used
Author: Zacharious Owen
Description: Finds the quanity and indexes of the output wires used in the wiring diagram in wires. (See make_wires for more info)
Parameters:	wires: 
pointer representing the wiring diagram(See make_wires for info)
return:	
a pointer to an array of size j + 1, the zeroth element is the quanity of output wires that connect to the array
and the following that many values are the indexes of them in the overall gate array.
 ************************************************************************/
int * get_out_used( int * wires )
{
	int i;
	int * in;
	//make an array of size number of input wires plus one
	in = malloc( sizeof(int) * ( OUTNUM + INNUM + WIDTHNUM+HEIGHTNUM + 1) );


	if( in == NULL )
	{
		printf( "Memory allocation Error\nExiting");
		exit(-1);	
	}

	//execept the first, its the counter
	in[0] = 0;

	for( i =  HEIGHTNUM * WIDTHNUM * 2 ; i < ( HEIGHTNUM * WIDTHNUM * 2 ) + OUTNUM  ; i = i + 1 )
	{
		// if we find an input connection
		if( wires[ i ] != -1 )
		{
			// if we have set it to used
			in[ in[0] + 1] = i;
			// and incriment number found
			in[0] ++;	
			//printf( "Found use of input wire#%d, found %d total\n" , i , in[0] );	
		}
	}	

	return in;
}



/************************************************************************
Function: make_wire
Author: Zacharious Owen
Description: creates a wiring diagram pointer that can be used by the rest of the program
the value returned will be of length enough to hold the full output. It is equal to the number of output wires (k) plus 2 inputs for each nand gate in the array (m*n) plus one for good measure. The indexes of this array represend the number of the output starting at zero for the first input on the first nand and ending with the k output wires.
Will not accept a single wire. I think you can figure out what the output should be at that point. if you cannot even I cannot save you
Parameters: 	fp:	
Filepointer holding the open file containing the desired wiring array.
return:	
pointer to array of "wires"
 ************************************************************************/
int * make_wire ( FILE * fp )
{
	int tfrom = -1;
	int  tto = -1;
	int inputsize =  OUTNUM + (HEIGHTNUM*WIDTHNUM * 2) + 1;
	int * wiring;
	int i;

	wiring = malloc( ( inputsize ) * sizeof(int)  );

	for( i = 0; i < inputsize ; i = i+1)
	{
		wiring[i] = -1;
	}	



	fscanf( fp , "%d" , &tfrom);
	fscanf( fp , "%d" , &tto);

	while( !feof(fp) )
	{
		if( tto > inputsize || tfrom > INNUM + (HEIGHTNUM *WIDTHNUM ) )
		{
			printf("ERROR IN WIRE DESIGN\n EXITING\n" );
			free( wiring);
			exit(-1);
		}

		if(  wiring[tto] != -1  || tto == -1 || tfrom == -1)
		{
			printf("ERROR IN WIRE DESIGN\n EXITING\n" );
			free( wiring);
			exit(-1);
		}
		else
		{

			wiring[tto] = tfrom;

			tfrom = -1;
			tto = -1;	
			fscanf( fp , "%d" , &tfrom);
			fscanf( fp , "%d" , &tto);
		}
	}

	if( check_wire( wiring ) ) 
	{
		printf("ERROR IN WIRE DESIGN\n EXITING\n" );
		free( wiring);
		exit(-1);
	}

	return wiring;
}


/************************************************************************
Function: run_sim
Author: Zacharious Owen
Description: Runs the full simulation on the array of nand gates. Itterates of the full array and calls the gggg() function on each ( See gggg() for more info )
Parameters:	GateArray:
pointer to a prepared and set array of NAND_GATE structs
 ************************************************************************/
void run_sim( struct NAND_GATE * GateArray )
{
	int i;

	for ( i = INNUM; i < INNUM + (HEIGHTNUM * WIDTHNUM);  i = i + 1)
	{
		gggg( GateArray , & GateArray[ i ] );		
	}
}


/************************************************************************
Function: set_gates
Author: Zacharious Owen
Description: Runs through the NAND gate array and sets the inputs to their proper values. There are no checks since at this point in the program the input is assumed to be valid.
Parameters:	a:	
pointer to a unprepared and unset array of NAND_GATE structs
wires:	
pointer representing the wiring diagram(See make_wires for info)
 ************************************************************************/
void set_gates( struct NAND_GATE * a, int * wires )
{
	int i;
	int k;

	for( i = 0; i < HEIGHTNUM * WIDTHNUM; i = i+1)
	{

		k = i * 2;	
		a[INNUM+i].input1 = wires[k];  
		a[INNUM+i].input2 = wires[k+1];

	}
	return;
}



/************************************************************************
Function: set_inputs
Author: Zacharious Owen
Description: Clever function, takes the input from the for() loop from which this is being run and masks and shifts it to obtain the next value for each nand gate. Then it sets it to the gates output.
Parameters:	GateArray:
pointer to a prepared but unset array of NAND_GATE structs
in:
Array containing quantity and indexes of input wires (See get_in_used() for more info)
test:
Test value containing the values to set the gates output.
 ************************************************************************/
void set_inputs( struct NAND_GATE * GateArray , int * in , int test )
{

	int i;

	for( i = 0; i < in[0]; i = i + 1)
	{
		GateArray[ in[in[0] - i] ].output = (test >> i ) & 0x1;
	}

}


/************************************************************************
Function: write_header
Author: Zacharious Owen
Description: Prints the names of all of the inputs and outputs to the gate array in a neat fasion. Labels inputs 'I' and outputs 'O'
Parameters: 	in:
Array containing quantity and indexes of input wires (See get_in_used() for more info)
out:
Array containing quantity and indexes of output wires (See get_out_used() for more info)
 ************************************************************************/
void write_header( int * in , int * out )
{
	int i;
	int print = in[0] + out[0];

	for( i = 0; i < print; i = i + 1)
	{
		printf( "------" );
	}

	printf( "\n" );

	for( i = 0; i < in[0]; i = i + 1)
	{
		printf("%3dI |", i);
	}


	for( i = 0 ; i < out[0]; i = i+1 )
	{
		printf("%3dO |", i);
	}

	printf( "\n" );

	for( i = 0; i < out[0] + in[0] ; i = i + 1)
	{
		printf( "------" );
	}

	printf( "\n" );
}



/************************************************************************
Function: write_output
Author: Zacharious Owen
Description: Uses the Array of gates along with the tracked input, outputs and their sources to print the output of a single simulation in a neat fasion.
Parameters:	GateArray:
pointer to a prepared and set array of NAND_GATE structs
in:
Array containing quantity and indexes of input wires (See get_in_used() for more info)
out:
Array containing quantity and indexes of output wires (See get_out_used() for more info)
wiring:
array of the inputs to the nand array, index being which input and value being where the connection is from
 ************************************************************************/
void write_output( struct NAND_GATE * GateArray , int * in , int * out, int * wiring )
{

	int i ;

	for( i = 0; i < in[0] ; i = i + 1 )
	{
		printf( "%3d  |", GateArray[ in[ i + 1 ] ].output );
	} 

	for( i = 0; i < out[0] ; i = i + 1 )
	{
		printf( "%3d  |", GateArray[ wiring [ out[ i + 1 ] ]  ].output );
	}
	printf( "\n" );

}
