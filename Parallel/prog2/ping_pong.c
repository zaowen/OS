/* Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
mpicc -g -Wall -lm -o ping ping_pong.c
// Ping pong example with MPI_Send and MPI_Recv. Two processes ping pong a
// number back and forth, incrementing it until it reaches a given value.
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int nb(int size, int num, int rank);
int sr(int size, int num, int rank);
int bl(int size, int num, int rank);


int main(int argc, char** argv) {
	int size, num;

	// Initialize the MPI environment
	MPI_Init( &argc, &argv);

	size = strtol(argv[1] , NULL , 10);
	num = strtol( argv[2] , NULL , 10);
	// Find out rank, size
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// We are assuming at least 2 processes for this task
	if (world_size != 2) {
		fprintf(stderr, "World size must be two for %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	bl( size , num , world_rank );
	sr( size , num , world_rank );
	nb( size , num , world_rank );

	MPI_Finalize();
	return 0;
}

/*
 * Nonblocking Version
 */
int nb( int size, int num, int rank){
	int i = 0;
	double t1, t2, tt;
	int * msg;
	MPI_Request rs, rr;
	MPI_Status  ss, sr;

	msg = malloc( sizeof( int ) * size );

	tt = 0;
	while (i < num ) {
		
		MPI_Barrier( MPI_COMM_WORLD );
		if ( rank == 0) {
			// Increment the ping pong count before you send it
			i++;
			t1 = MPI_Wtime();

			MPI_Isend(msg, size, MPI_INT, 1 , 0, MPI_COMM_WORLD, &rs);
			MPI_Irecv(msg, size, MPI_INT, 1 , 0, MPI_COMM_WORLD, &rr);

			MPI_Wait( &rr , &sr);
			t2 = MPI_Wtime();
			tt += t2 - t1;

			//printf( "Ping #%d:%f\n", i , t2 - t1 );
			MPI_Wait( &rs , &ss);

		} else {
			i++;
			MPI_Isend(msg, size, MPI_INT, 0 , 0, MPI_COMM_WORLD, &rs);
			MPI_Irecv(msg, size, MPI_INT, 0 , 0, MPI_COMM_WORLD, &rr);

			MPI_Wait( &rs , &ss);
			MPI_Wait( &rr , &sr);
		}
	}

	if ( rank == 0) {
		printf( "Nonblocking:\n\t%f\n", tt/num );
	}	
	free( msg );

	return 0;
}

/*
 * Send Revive
 */
int sr( int size, int num, int rank){
	int i = 0;
	double t1, t2, tt;
	int * msg;
	
	msg = malloc( sizeof( int ) * size );


	tt = 0;
	while (i < num ) {
		i++;

		MPI_Barrier( MPI_COMM_WORLD );

		t1 = MPI_Wtime();
		MPI_Sendrecv( 	msg, size, MPI_INT, ((rank + 1) % 2), 0,
				msg, size, MPI_INT, ((rank + 1) % 2), 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		t2 = MPI_Wtime();
		tt += t2 - t1;
		//	if ( rank == 0) {
		//		// Increment the ping pong count before you send it
		//		i++;
		//		t1 = MPI_Wtime();

		//		MPI_Sendrecv( 	msg, size, MPI_INT, 1, 0,
		//				msg, size, MPI_INT, 1, 0,
		//				MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		//		t2 = MPI_Wtime();
		//		tt += t2 - t1;
		//		//printf( "Ping #%d:%f\n", i , t2 - t1 );

		//	} else {
		//		i++;
		//		MPI_Sendrecv( 	msg, size, MPI_INT, 0, 0,
		//				msg, size, MPI_INT, 0, 0,
		//				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//		//MPI_Recv(msg, size, MPI_INT, 0 , 0, MPI_COMM_WORLD,
		//		//		MPI_STATUS_IGNORE);
		//		//MPI_Send(msg, size, MPI_INT, 0 , 0, MPI_COMM_WORLD);
		//	}
	}

	if ( rank == 0) {
		printf( "SendRecive:\n\t%f\n", tt/num );
	}	
	free( msg );
	return 0;
}

/*
 * Blocking
 */
int bl(int size, int num , int rank ){

	int i = 0;
	double t1, t2, tt;
	int * msg;

	msg = malloc( sizeof( int ) * size );

	tt = 0;

	while (i < num ) {
		MPI_Barrier( MPI_COMM_WORLD );
		if ( rank == 0) {
			// Increment the ping pong count before you send it
			i++;
			t1 = MPI_Wtime();
			MPI_Send(msg, size, MPI_INT, 1 , 0, MPI_COMM_WORLD);
			MPI_Recv(msg, size, MPI_INT, 1 , 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
			t2 = MPI_Wtime();
			tt += t2 - t1;
			//printf( "Ping #%d:%f\n", i , t2 - t1 );
		} else {
			i++;
			MPI_Recv(msg, size, MPI_INT, 0 , 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
			MPI_Send(msg, size, MPI_INT, 0 , 0, MPI_COMM_WORLD);
		}
	}

	if ( rank == 0) {
		printf( "Blocking:\n\t%f\n", tt/num );
	}	
	free( msg );
	return 0;
}
