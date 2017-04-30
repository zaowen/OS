/**
 * General structure of the teaching assistant.
 *
 */

#include "ta.h"

void *ta_loop(void *param)
{
	int temp;
	/* seed random generator */
	srandom((unsigned)time(NULL));


	pthread_mutex_lock(&door);
	while (1) {


		/* wait for a student to show up */
		sem_wait(&TAhelp);
		//printf("TA awake\n" );

		//make sure no more students show up and change the semaphore
		pthread_mutex_lock(&chairs);
		//printf("locked chairs\n" );

		//unlock seat 2
		sem_post( &seat[2]);
		//printf("unlocked seat 3\n" );

		//decrement butts
		sem_wait( &butts);
		//printf("1 less butt\n" );

		//get number of students
		sem_getvalue( &butts, &temp );
		//printf("gett butt\n" );

		//allow other students top do stuff
		pthread_mutex_unlock( &chairs );
		//printf("unlock chairs\n" );

		//print help student message
		help_student(1+ random() % MAX_SLEEP_TIME, temp ); 
		//sem_post(&TAhelp);
		//printf("let student out\n" );
		pthread_mutex_unlock(&door);
		

		/* indicate the TA is ready to help a student */


	}
}

