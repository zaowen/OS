#include "customer.h"
#include "bank.c"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *customer ( void * in ) {

		int id = (int) in ;
		int i;
		int test[ NUMBER_OF_RESOURCES] = {0};


		for( i = 0;  i < CUSTOMER_GOES; i ++ ){

				//generate need
				make_need( id,  (int*)test ); 

				//request some stuff
				if( request_resources( id, test ) ){

						sleep( HEAD_SLEEP );

						release_resources( id, test );
						//if not wait and request again.
				}
				else{

						i--;
						sleep( HEAD_SLEEP );
				}
		}
}

int isqrt( int n ){
        int smallCandidate;
        int largeCandidate;

    if( n < 2){
        return n;
	}
    else{
        smallCandidate = isqrt(n >> 2) << 1;
        largeCandidate = smallCandidate + 1;

        if(largeCandidate*largeCandidate > n){
            return smallCandidate;
		}
        else
		{
            return largeCandidate;
		}
	}
}	
				
void make_need( int n ,int * akljsnjskskjkjfalkskjkjejn  ) {
		int i;

		for( i = 0; i < NUMBER_OF_RESOURCES; i++){
				if( need[n][i] != 0 )
						akljsnjskskjkjfalkskjkjejn[i] = need[n][i] - isqrt(rand() % (need[n][i] * need[n][i]) );
		}

}

void print_req( int c, int r[] ){
		int i;

		printf( "Request P%d <%d", c, r[0] );

		for( i = 1; i < NUMBER_OF_RESOURCES; i++){
				printf( ", %d", r[i] ); 
		}
		printf( ">\n" );
}

int request_resources ( int customer_num, int request[]){

		int i;
		int ret;

		//LOCK 
		pthread_mutex_lock( &READ );

		print_req( customer_num, request);
		
		alloc_req( customer_num, request);

		ret = safety_test();

		if( ret ){
				printf( "Safe, request granted.\n" );
				print_state();
		}	
		else{
				printf( "Unsafe, request denied.\n");
				free_req( customer_num, request);
#ifdef DEBUG
				print_state();
#endif
		}

		//UNLOCK
		pthread_mutex_unlock( &READ );

		return ret;
}

void release_resources ( int customer_num, int release[]){


		pthread_mutex_lock( &READ );

		printf( "P%d: releaseing resources\n",customer_num );
		free_req( customer_num, release);
		sleep( 1 );
		printf( "done!\n");


		pthread_mutex_unlock( &READ );

}

void print_state(){
		int i,j;

		printf( "\t\tAllocation\t\tNeed    \t\tAvailable\n" );
		printf( "\t\t" );

		for( i = 0; i < 3; i++){

				for( j = 0; j < NUMBER_OF_RESOURCES; j++){
						printf( "%c " , 'A'+j );
				}

				printf( "\t\t\t" );

		}


		i = 0;

		printf( "\n" );

		printf( "P0\t\t" , i);

		for( j = 0; j < NUMBER_OF_RESOURCES; j++){
				printf( "%d " ,allocation[i][j] );
		}

		printf( "\t\t\t" );

		for( j = 0; j < NUMBER_OF_RESOURCES; j++){
				printf( "%d ", need[i][j]  );
		}

		printf( "\t\t\t" );

		for( j = 0; j < NUMBER_OF_RESOURCES; j++){
				printf( "%d ", available[j]  );
		}
		printf( "\n" );

		for( i = 1; i < NUMBER_OF_CUSTOMERS; i++){
				printf( "P%d\t\t" , i);

				for( j = 0; j < NUMBER_OF_RESOURCES; j++){
						printf( "%d " ,allocation[i][j] );
				}

				printf( "\t\t\t" );

				for( j = 0; j < NUMBER_OF_RESOURCES; j++){
						printf( "%d ", need[i][j]  );
				}
				printf( "\n" );

		}		

		printf( "\n" );

}

void alloc_req( int n , int * arr ){
	int i;

	for ( i = 0; i < NUMBER_OF_CUSTOMERS; i++){

			available[i] -= arr[i];
			need[n][i] -= arr[i];
			allocation[n][i] += arr[i];
	}
		
}

void free_req( int n , int * arr ){

	int i;

	for ( i = 0; i < NUMBER_OF_CUSTOMERS; i++){

			available[i] += arr[i];
			need[n][i] += arr[i];
			allocation[n][i] -= arr[i];
	}

}	

int safety_test( ){
	
		int i;
		int finished[ NUMBER_OF_CUSTOMERS] = {0};
	
		int resources[ NUMBER_OF_RESOURCES] = {0};

		add_arr(  available, resources );

		for( i = 0; i < NUMBER_OF_CUSTOMERS; i++){
				if( !finished[i] && can_finish( resources ,  need[i] ) ){

						finished[i] = 1;
						add_arr( available, resources  );	
						i = 0;
				}

		}

		for( i = 0; i < NUMBER_OF_CUSTOMERS; i++){
			if( !finished[i] ){
					return 0;
			}
		}
		
		return 1;
}

void add_arr( int *a, int *b ){
		int i;
		
		for( i = 0; i < NUMBER_OF_RESOURCES; i++){
			b[i] += a[i];
		}

		return;
}

int can_finish( int *r, int *n){

		int i;

		for( i = 0; i < NUMBER_OF_RESOURCES; i++){
			if( r[i] < n[i] ){
					return 0;
				
			}
		}
		return 1;
}
