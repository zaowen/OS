/* File:       
 *    mpi_hello.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 mpicc -g -Wall -lm -o life life.c
 * Usage:        
 *    mpiexec -np <number of processes> ./mpi_hello
 *    mpiexec -np <number of processes> -hostfile <list of hosts> ./mpi_hello
 *
 * Input:      
 *    None
 * Output:     
 *    A greeting from each process
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 * IPP:  Section 3.1 (pp. 84 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

//Defines
#define cell uint8_t
#define alive 0x80

//Globals
static int gx, gy, myx, myy;

//void life_print();
void life_print( cell ** p);
cell ** life_init( int live );
void run_sim( cell ** arr );
void dist_print( cell ** a , int nprocs , int rank);	
void life_tally( cell ** arr );
cell ** life_dist( cell ** arr , int procs, int rank);
void get_top( cell ** arr , int procs, int rank);
void get_bot( cell ** arr , int procs, int rank);
void life_check( cell ** arr );
void inc_set(int y , int x , cell ** arr);

int main(int argc, char *argv[]) {

	int nprocs, rank;
	int live, iter, age, x , y;
	cell ** a;
	cell ** init;


	MPI_Init(&argc, &argv);

	//Check for # of args
	if( argc <= 5)
	{

		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		if( rank == 0){
			printf("Useage  mpiexec -np <number of processes> %s ", argv[0]);
			printf("<live cells to start with> <iterations> <print");
			printf(" every> <x size> <y size>\n");
		}
		MPI_Finalize();
		exit(0);
	}

	//init MPI stuff
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 	//get command line args
	live = strtol( argv[1], NULL, 10 );
	iter = strtol( argv[2], NULL, 10 );
	age = strtol( argv[3], NULL, 10 );
	x = strtol( argv[4], NULL, 10 );
	y = strtol( argv[5], NULL, 10 );

	//make sure row > col
	if( y < x ){
		x = x ^ y;
		y = y ^ x;	
		x = x ^ y;
	}

	//ensure y is proper
	y = y + (y % nprocs);

	printf( "%d %d %d %d %d\n", live, iter, age, x, y);
	//set static params
	gx = myx = x;
	gy = y;
	myy = y/nprocs;


	//printf( "%d %d %d %d %d\n", live, iter, age, x, y);

	//set up basic parameters
	if( rank == 0){
		//init array with random values
		printf("Field Init started\n");
		init = life_init( live );
		printf("Field Init done\n");
	
	}

	MPI_Barrier( MPI_COMM_WORLD );

	//distribute random life feild
	printf("Field distribution started\n");
	fflush(stdout);
	a = life_dist( init ,nprocs, rank);
	
	
	MPI_Barrier( MPI_COMM_WORLD );
	//print distributed state
	printf("Begining Print\n");
	dist_print( a , nprocs , rank);	
	//life_print( x , y , nprocs);
	//life_print( init );

	int i;
	for( i = 0; i < iter; i++){
		printf("\n");
		MPI_Barrier( MPI_COMM_WORLD );
		run_sim( a );

		if( i % age == 0)
			dist_print( a , nprocs, rank );
	}

	MPI_Finalize();

	return 0;
}

void run_sim( cell ** arr ){

	//Runs on the whole field.
	life_tally( arr );

	//Runs on part of the field.
	life_check( arr );
	return;
}

void dist_print( cell ** a , int nprocs, int rank ){
	
	int flag;
	
	printf("Starting Distributed Print Node#%d\n", rank);
	if( rank == 0 ){

		life_print( a + 1);

		MPI_Send( &flag, 1, MPI_INT, 1 , 0, MPI_COMM_WORLD);

		MPI_Recv( &flag, 1, MPI_INT, nprocs-1 , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	else {
		
		MPI_Recv( &flag, 1, MPI_INT, rank-1 , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		life_print( a + 1);

		MPI_Send( &flag, 1, MPI_INT, (rank + 1) % nprocs , 0, MPI_COMM_WORLD);
	}
}

cell ** life_init( int live ){

	cell ** init;	
	int i;
	int r,c;
	
	//allocate pointers for rows
	init = malloc( gy * sizeof(*init));
	
	//allocate space for cols
	for( i = 0; i < gy; i++){

		init[i] = malloc ( gx * sizeof( cell) );
		//set them to zero
		memset( init[i] , 0 , gx * sizeof( cell) );
	}
	
	//seed some random time
	srand(  time(NULL) );

	//set some random cells.
	for( i = 0; i > live; i++){
		
		r = rand() % gx;
		c = rand() % gy;
		init[c][r] = alive;	
	}

	init[2][2] = alive;
	init[3][3] = alive;
	init[4][1] = alive;
	init[4][2] = alive;
	init[4][3] = alive;

	return init;
}


void life_tally( cell ** arr ){

	int i , j;

	for ( i = 0; i < gy; i++){

		for( j = 0; j < gx; j++)
		{
			if( arr[i][j] >= alive ){
				inc_set(i , j , arr);
			}
		}
	}
}

void inc_set(int y , int x , cell ** arr){
	
	printf( "INCSETTING\n");
	int i, j;
	//left and right;
	for( i = -1; i < 2; i++)
		for( j = -1; j < 2; j++)
			if( y + i >= 0)
				arr[ (y+i) % myy ][ (x+j) % myx]++;
	
	arr[y][x]--;
	
}

void get_top( cell ** arr , int procs, int rank){	
	
	int flag;
	
	printf("Starting Upper Trade Node#%d\n", rank);
	if( rank == 0 ){

		life_print( a + 1);

		MPI_Send( &flag, 1, MPI_INT, 1 , 0, MPI_COMM_WORLD);

		MPI_Recv( &flag, 1, MPI_INT, nprocs-1 , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	else {
		
		MPI_Recv( &flag, 1, MPI_INT, rank-1 , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		life_print( a + 1);

		MPI_Send( &flag, 1, MPI_INT, (rank + 1) % nprocs , 0, MPI_COMM_WORLD);
	}
}

void get_bot( cell ** arr , int procs, int rank){

	int flag;
	
	printf("Starting Distributed Print Node#%d\n", rank);
	if( rank == 0 ){

		life_print( a + 1);

		MPI_Send( &flag, 1, MPI_INT, 1 , 0, MPI_COMM_WORLD);

		MPI_Recv( &flag, 1, MPI_INT, nprocs-1 , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	else {
		
		MPI_Recv( &flag, 1, MPI_INT, rank-1 , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		life_print( a + 1);

		MPI_Send( &flag, 1, MPI_INT, (rank + 1) % nprocs , 0, MPI_COMM_WORLD);
	}
}

cell life_set( cell a){
	//masked cell
	cell temp;

	temp = (a & 0x0F);
	// if cell is alive
	if( a > alive){
		//Check if its not crowded or alone	
		if( temp == 2 || temp == 3)
		{
			return alive;
		}
	}//if its dead check for 3 neiborps
	else if ( temp == 3){
		return alive;
	}

	//all other cases the cell is dead.
	return 0;
}


void life_check( cell ** arr ){

	int i , j;

	for ( i = 0; i < gy; i++){

		for( j = 0; j < gx; j++)
		{
			printf( "%d,", arr[i][j] & 0xf);
			arr[i][j] = life_set( arr[i][j] );
		}
		printf( "\n");
	}
}

void life_print( cell ** p){
	int j,i;

	printf("\n");
	for ( i = 1; i < myy; i++){

		for( j = 0; j < gx; j++)
		{
			printf("%c ", p[i][j] == 0 ? '.' : '#' );
		}
		printf("\n");
	}	
	fflush(stdout);
}

cell ** life_dist( cell ** arr , int procs, int rank ){

	int i,j;
	cell ** p;
	int rows = gy/procs;

	//allocate pointers for rows in non-root
	p = malloc( rows * sizeof(*p) + 2);

	//allocate space for cols
	for( i = 0; i < rows; i++){

		p[i+1] = malloc ( gx * sizeof( cell) );
	}

	printf( "Shit be malloc'ed yo from node %d\n", rank);

	// have Root distribute rows	
	if( rank == 0){
		//for each process
		printf( "Root be doing sendin shit yo\n");
		for( i = 1; i < procs; i++){
			//give it $row rows
			for( j = 0; j < rows; j++){
				printf( "Sending row %d\n", j);
				MPI_Send( arr[ i * rows + j], gx, MPI_CHAR, i , 0, MPI_COMM_WORLD );
			}
		}

		for( i = 0; i < rows; i++){
			memcpy( p[i+1] , arr[i], gx* sizeof(cell) );
		}
	}
	//non-root accepts them
	else {
		printf( "Recievining rows in node %d\n", rank);
		for( i = 0; i < rows; i++){
			printf( "reciving row %d\n", i);
			fflush(stdout);
			MPI_Recv( p[i+1], gx, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			printf( "recived row %d\n", i);
		}
	}		

	return p;
}
