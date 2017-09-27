/************************************************************************
	Program: Sieve of Eratosthenes
	Author: Zach Owen
	Class: CSC 410
	Instructor: Dr Karlson
	Date: October 16, 2016
 	Description: Calculates and prints prime numbers using the Sieve of Eratosthenes in Parallel
	Input: Takes a single argument and is the upper limit of the primes to print
	Output: Primes, delimited in multiples of ten
	Compilation instructions: gcc -fopenmp -o sieve sieve.c
	Usage: ./sieve <Primes>
	Known bugs/missing features:
		In theory the number of primes to calculate is limited on 3 fronts, allowed memory by OS, the size of the 64-bit integer, and the users's patience. In practice the highest searchable number is 18,446,744,073,709,551,615 (uint_64) no additional RAM or Permisison will fix that. and there are at least 98 million primes below 2 Billion so we could find quite a few.
	
	NOTE: If this program segmentation faults, the signal it recieves is a SIGSIEV of Eratosthenes.
************************************************************************/

#include <stdio.h> /* I/O*/
#include <string.h> /* memset */
#include <limits.h> /* ERANGE LONG_MAX LONG_MIN */
#include <stdlib.h> /* strtol exit malloc */
#include <stdint.h> /* uint64_t */
#include <inttypes.h> /* PRIu64 */
#include <omp.h> /* OMP PRAGMAS*/
#include <errno.h> /* errno */


/************************************************************************
Function: Print Primes
Author: Zacharious Owen
Description: linearly traverses an array printing the indexes of values that are not -1
in format <multiple of ten>: <p0> <p1> <p2> <p3> <p4> <p5> <p6> <p7> <p8> <p9>
Parameters:
	arr: array to traverse
	max: value to traverse to (Prevents SIGSIEV)
************************************************************************/
void print_primes( char * arr, int max )
{
	uint64_t i, j, k;

	k = 0;
	j = 0;

	printf( "\n%6"PRIu64": ", k);
	for( i = 0; i < max; i++){
		if( arr[i] == 0){
			printf( "%6"PRIu64" " , i+1);
			j++;
		}	
		if( j == 10){
			k += 10;
			j = 0;
			printf( "\n%6"PRIu64": ", k);
		}
	}
	printf( "\n" );
	return;
}


/************************************************************************
Function: Changes multiples of an index in an array to -1
Author: Zacharious Owen
Description: Takes in s number and multiplies it by i until s > max
Parameters: 
	arr: Array to step through
	s: Base index to clear
	max: upper limit to traverse arr (to prevent SIGSIEV)
************************************************************************/
void sieve( char * arr, uint64_t s, uint64_t max)
{
	uint64_t i;

	i = 2;

	while( i * s <= max ){
		arr[ (i++ * s) -1 ] = -1;
	}
}

int main( int argc , char * argv[])
{
	char *endptr, *str;
	uint64_t val, i, threads;
	char * arr;
	double start, stop;

	if( argc < 3){
		fprintf( stderr, "Usage %s limit threads\n" , argv[0] );
		exit( EXIT_FAILURE );	
	}		

	str = argv[1];
	errno = 0;

	val = strtol( str, &endptr , 10);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	str = argv[2];
	errno = 0;

	threads = strtol( str , &endptr, 10);	

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		exit(EXIT_FAILURE);
	}

	arr = malloc( sizeof( char ) * val );

	memset( arr , 0 , val );

	arr[0] = -1;

	start = omp_get_wtime();

	sieve( arr, 2, val);
#pragma omp parallel for num_threads( threads ) schedule(static)
	for( i = 3;  i < val; i+=2 )
	{
		if( arr[i] == 0)
			sieve( arr, i, val);
	}

	stop = omp_get_wtime();

//	print_primes( arr , val );
	printf( "Estimated time = %f ms\n", (stop - start)*1000 );
	
	free(arr);
	return 0;
}
