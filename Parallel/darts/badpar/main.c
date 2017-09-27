/************************************************************************
Program:
Author:
Class:
Instructor:
Date:
Description:
Input:
Output:
Compilation instructions: gcc -g -Wall -fopenmp mainp.c 
Usage:
Known bugs/missing features:
Modifications:
Date                Comment            
----    ------------------------------------------------
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


/************************************************************************
Function:
Author: Zacharious Owen
Description:
Parameters:
 ************************************************************************/
double dist( int x , int y )
{
	double dx , dy;
	dx = (double)x / RAND_MAX;	
	dy = (double)y / RAND_MAX;	

	return dy*dy + dx*dx;
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
	int hits = 0;
#pragma omp parallel for num_threads(thread_count) \
	reduction(+: hits)
	for(i = 0; i < darts; i++)
	{
		if( dist( rand() , rand()) < 1 )
		{
			hits += 1;
		}
	}
	return hits;
}

/************************************************************************
Function:
Author: Zacharious Owen
Description:
Parameters:
 ************************************************************************/
int main( int argc, char ** argv)
{	
	unsigned long darts,hits;
	int thread_count;

	thread_count = omp_get_num_procs();
	printf("Enter number of points to sample and threads\n");
	//scanf( "%lu %d" , &darts, &thread_count );
	darts = 10000000;
	thread_count = strtol(argv[1], NULL, 10);
	srand(0);

	printf("Throwing\n");
	hits = make_point( darts, thread_count );

	printf( "Your approximation is %e." , (double)(hits << 2 ) / darts );
}
