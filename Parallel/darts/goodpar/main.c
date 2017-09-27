/************************************************************************
Program: Pi from Darts
Author: Zachary Owen
Class:
Instructor:
Date:
Description:
Input:
Output:
Compilation instructions: gcc -g -Wall -fopenmp main.c 
Usage: /a.out <number of processors to run program on.
Known bugs/missing features:
Modifications:
Date                Comment            
----    ------------------------------------------------
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

//RAN1 #DEFINES
#define M1 259200
#define IA1 7141
#define IC1 54773 
#define RM1 (1.0/M1)
#define M2 134456
#define IA2 8121
#define IC2 28411
#define RM2 (1.0/M2)
#define M3 243000
#define IA3 4561
#define IC3 51349

//RAN0 #DEFINES
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define MASK 123459876


/************************************************************************
Function:
Author: Numerical Recipies in C
Description:
Parameters:
************************************************************************/
float ran1( int *idum )
{
	static long ix1, ix2, ix3;
	static float r[98];
	float temp;
	static int iff=0;
	int j;

	if ( *idum < 0 || iff == 0) {
		iff = 1;
		ix1 = (IC1-(*idum)) % M1;
		ix1 = (IA1*ix1+IC1) % M1;
		ix2 = ix1 % M2;
		ix1 = (IA1*ix1+IC1) % M1;
		ix3 = ix1 % M3;
		for( j=1;j<=97;j++){
			ix1=(IA1*ix1+IC1) % M1;
			ix2=(IA2*ix2+IC2) % M2;
			r[j]=(ix1+ix2*RM2)*RM1;
		}
		*idum=1;
	}
	ix1=(IA1*ix1+IC1) % M1;
	ix2=(IA2*ix2+IC2) % M2;
	ix3=(IA3*ix3+IC3) % M3;
	j=1 + ((97*ix3)/M3);
	if(j>97 || j < 1) printf("RAN1: This Cannot Happen.");
	temp=r[j];
	r[j]=(ix1+ix2*RM2)*RM1;
	return temp;
}


/************************************************************************
Function:
Author: Numerical Recipies in C
Description:
Parameters:
************************************************************************/
float ran0 ( int *idum ){

	long k;
	float ans;

	*idum ^= MASK;
	k =(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) 
		*idum += IM;
	ans=AM*(*idum);
	*idum ^= MASK;
	return ans;
}

/************************************************************************
Function:
Author: Zacharious Owen
Description:
Parameters:
 ************************************************************************/
int dist( double x , double y )
{
	if( y*y + x*x <= 1.0 )
	{
		return 1;
	}
	return 0;
}

/************************************************************************
Function:
Author: Zacharious Owen
Description:
Parameters:
 ************************************************************************/
unsigned long make_point( unsigned long darts , int thread_count )
{
	int i;
	double x,y;
	int hits = 0;

	int idum = 0; 
	
#pragma omp parallel for num_threads(thread_count) \
	private( idum ) reduction(+: hits)
	for(i = 0; i <= darts; i++)
	{
		if( idum == 0)
			idum = omp_get_num_procs();
		x = ran0( &idum );
		y = ran0( &idum );   
		hits += dist( x , y);
	}
	return hits;
}

unsigned long make_serial( unsigned long darts , int thread_count )
{
	int i;
	double x,y;
	int hits = 0;

	int idum = 0; 
	
	for(i = 0; i <= darts; i++)
	{
		x = ran0( &idum );
		y = ran0( &idum );   
		hits += dist( x , y);
	}
	return hits;
}
/************************************************************************
Function:
Author: Zacharious Owen
Description:
Parameters:
************************************************************************/
unsigned long simsquare( int thread_count )
{
	int i , j = 0;
	double x,y;
	double temp;
	int hits = 0;

	int *idum;
	idum = malloc( sizeof(int) * thread_count);
	for( i = 0; i < thread_count; i++) idum[i] = i;
	
	for(i = 0; ; j++, i++)
	{
		x = ran0( &idum[i] );
		y = ran0( &idum[i] );   
		hits += dist( x , y);
	
		temp = (((double)(hits << 2 )) /j ) - 3.1415926; 
		if( temp < 0) temp *= -1;

		if( i == thread_count && temp < .000000001 ) break;
		if( i >= thread_count) i = 0;	
	}

	printf("Found %d Hits.", hits );
	printf("Running with value %lf.\n", (((double)(hits << 2 )) /j ) );

	return j;
}

/************************************************************************
Function: main
Author: Zacharious Owen
Description: Calls 
Parameters: argc , argv
 ************************************************************************/
int main( int argc, char ** argv)
{	
	unsigned long darts,hits;
	int thread_count;

	printf("Enter number of points to sample and threads\n");

	thread_count = strtol(argv[1], NULL, 10);

	printf("Calculating Throws\n");
	darts =  simsquare( thread_count );

	printf("Throwing %lu darts\n", darts);
	if(thread_count > 1 ) {
		hits = make_point( darts, thread_count );
	}
	else {
	hits = make_point( darts, thread_count );
	}

	printf( "Your approximation is %lf." , (double)(hits << 2 ) / darts );
}

