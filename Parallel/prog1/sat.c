/************************************************************************
	Program: Parallel Satafiability
	Author: Zachary Owen, Dr Karlson
	Class: CSC 410 
	Instructor: Dr Karlson
	Date: October 16, 2016
 	Description: Outputs the inputs to a circuit that return true
	Input: None
	Output: test number and its input that recived a True output
	Compilation instructions: gcc -fopenmp -o sat sat.c
	Usage: ./sat
************************************************************************/
#include <stdio.h> /* I/O */
#include <omp.h> /* omp Pragmas */

/* Return 1 if 'i'th bit of 'n' is 1; 0 otherwise */ 
#define EXTRACT_BIT(n,i) ((n&(1<<i))?1:0) 

/* Check if a given input produces TRUE (a one) */ 
int check_circuit (int id, int z)  
{ 
	int v[16];        /* Each element is a bit of z */ 
	int i; 
	for (i = 0; i < 16; i++) v[i] = EXTRACT_BIT(z,i); 
	if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) 
			&& (!v[3] || !v[4]) && (v[4] || !v[5]) 
			&& (v[5] || !v[6]) && (v[5] || v[6]) 
			&& (v[6] || !v[15]) && (v[7] || !v[8]) 
			&& (!v[7] || !v[13]) && (v[8] || v[9]) 
			&& (v[8] || !v[9]) && (!v[9] || !v[10]) 
			&& (v[9] || v[11]) && (v[10] || v[11]) 
			&& (v[12] || v[13]) && (v[13] || !v[14]) 
			&& (v[14] || v[15])) { 
		printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", id, 
				v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9], 
				v[10],v[11],v[12],v[13],v[14],v[15]); 
		fflush (stdout); 
		return 1; 
	} else return 0; 
}

int main( int argc, char * argv[])
{
	int i, thread_count;
	double start , stop;

	//Get maximum number of threads
	thread_count = omp_get_num_procs();
	//Start timer
	start = omp_get_wtime();

	//Static 1 schedualing seems to be slightly faster. Sometimes.
# pragma omp parallel for num_threads(thread_count) schedule(static,1)
	for( i = 0; i < 65536; i++)
	{
		check_circuit( omp_get_thread_num(), i);
	}
	//Stop timer
	stop = omp_get_wtime();
	
	printf( "Estimated time = %f ms\n", (stop - start)*1000 );
	return 0;
}
