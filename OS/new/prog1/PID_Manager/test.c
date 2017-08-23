/*
 * test.c
 *
 * Test the implementation of the PID manager.
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pid.h"

#define NUM_THREADS 100
#define ITERATIONS   10
#define SLEEP         5

/**
 * mutex lock used when accessing data structure
 * to ensure there are no duplicate pid's in use.
 */


void *allocator(void *param)
{
	int i, pid;


	//printf("inalloca\n");

	for (i = 0; i < ITERATIONS; i++) {
		/* sleep for a random period of time */
		sleep((int)(random() % SLEEP));

		/* allocate a pid */
		//single locked to shared memory
		pid = allocate_pid();

		//printf("got id\n");

		if (pid == -1)
			printf("No pid available\n");
		else {
			/* indicate in the in_use map the pid is in use */
			printf( "Allocated %d\n" , pid );

			/* sleep for a random period of time */
			sleep((int)(random() % SLEEP));

			release_pid( pid );

		}
	}
	return (void*)0;
}


int main(void){

	int i;
	pthread_t tids[NUM_THREADS];
	
	allocate_map();

	pthread_mutex_init(&test_mutex , NULL);

	srandom((unsigned)time(NULL));

	/* create the threads */
	for (i = 0; i < NUM_THREADS; i++) {

		pthread_create( &tids[i], NULL, &allocator, NULL );
	}

	/* join the threads */
	for (i = 0; i <= NUM_THREADS; i++) {
		pthread_join(tids[0], NULL);
	}

	/* test is finished */
	pthread_mutex_destroy(&test_mutex);

	return 0;
}
