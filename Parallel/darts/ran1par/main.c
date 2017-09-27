/************************************************************************
Program:
Author:
Class:
Instructor:
Date:
Description:
Input:
Output:
Compilation instructions: gcc -g -Wall -fopenmp main.c 
Usage:
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

#define MYRAND_MAX 100255

float myrand (seed )
int *seed
{
 	return ((((seed * seed) + 1)*3583) % 100255);
}

/************************************************************************
Function:
Author: Zacharious Owen
Description:
Parameters:
 ************************************************************************/
int dist( int x , int y )
{
	double dx , dy;
	dx = (double)x / MYRAND_MAX;
	dy = (double)y / MYRAND_MAX;	

	if( dy*dy + dx*dx <= 1.0 )
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
	int i,x,y;
	int hits = 0;
	
	x = getpid();
#pragma omp parallel for num_threads(thread_count) \
	reduction(+: hits)
	for(i = 0; i <= darts; i++)
	{
		x = myrand( x * omp_get_thread_num() );
		y = myrand( y * omp_get_thread_num() );
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
int main( int argc, char ** argv)
{	
	unsigned long darts,hits;
	int thread_count;

	thread_count = omp_get_num_procs();
	printf("Enter number of points to sample and threads\n");
	//scanf( "%lu %d" , &darts, &thread_count );
	darts = 10000000;
	thread_count = strtol(argv[1], NULL, 10);
	
	thread_count = getpid();
	printf( "%d\n" , thread_count );
	for( int i = 0; i < 100; i++)
	{
		thread_count = myrand ( thread_count);
		printf( "%d\n" , thread_count );
	}

	//printf("Throwing\n");
	//hits = make_point( darts, thread_count );

	//printf( "Your approximation is %e." , (double)(hits << 2 ) / darts );
}
