/************************************************************************
Program: 
Author: ZArthur 
Class:
Instructor:
Date:
Description: 
Input:
Output:
Compilation instructions:
Usage:
Known bugs/missing features:
Modifications:
Date                Comment            
----    ------------------------------------------------
 ************************************************************************/

#include <stdio.h> 
#include <time.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "bank.c"
#include "customer.h"



/*********************** global symbolic constants *********************/

/*********************** global type definitions ***********************/

/*********************** function prototypes ***************************/

void init_all_the_things();


/************************************************************************
Function:
Author:
Description:
Parameters:
 ************************************************************************/
int main( int argc, char * argv[] )
{

		int i;
		pthread_t customerthread[NUMBER_OF_CUSTOMERS];

		//check for proper number of resources
		if( argc < NUMBER_OF_RESOURCES ){
				fprintf( stderr, "Usage %s ", argv[0] ); 
				for( i = 0; i < NUMBER_OF_RESOURCES; i++)
						fprintf( stderr, "<resource %d> " , i );

				fprintf( stderr, "\n\tExiting.\n");
				exit(EXIT_FAILURE);
		}


	
		//initializing stuff
		init_all_the_things( argv );
		print_state();

		for( i =  0; i < NUMBER_OF_CUSTOMERS; i++ ){

			pthread_create( &customerthread[ i ], NULL, &customer, (void*)i );

		}

		for (i = 0; i < NUMBER_OF_CUSTOMERS; i++)
				pthread_join(customerthread[i], NULL);


		return 0;
}

void init_all_the_things( char * argv[] ){
		int i,j, res;
		char *endptr, *str;

		//read in resources
		for( i = 0; i < NUMBER_OF_RESOURCES; i ++ ){	

				str = argv[i+1];

				res = strtol( str , &endptr, 10 );

				if(( errno == ERANGE && ( res == INT_MAX || res == INT_MIN))
								|| (errno !=0 && res ==0)) {
						perror("strtol");
						exit(EXIT_FAILURE);

				}

				if( endptr == str ) {
						fprintf( stderr, "Improper digit found in argument %d\n", i );
						fprintf( stderr, "Usage %s ", argv[0] ); 
						for( i = 0; i < NUMBER_OF_RESOURCES; i++)
								fprintf( stderr, "<resource %d> " , i );

						fprintf( stderr, "\n\tExiting.\n");
						exit(EXIT_FAILURE);
				}

				MAX_RESOURCE[ i ] = res;
				available[ i ] = res;
		}
		
		srand( time(NULL) );

		for ( i = 0; i < NUMBER_OF_CUSTOMERS; i++){

				for ( j = 0; j < NUMBER_OF_RESOURCES; j++){

						maximum[i][j] = MAX_RESOURCE[j] - isqrt( rand() % (MAX_RESOURCE[j] * MAX_RESOURCE[j]) );

						allocation[i][j] = 0;

						need[i][j] = maximum[i][j];
				}
		}

		pthread_mutex_init( &READ, NULL );
}


