/**
 * General structure of a student.
 *
 */


#include "ta.h"



void *student_loop(void *param)
{
	/* varaibles */
	int64_t studentID = (int64_t)param;
	int quant;

	srandom((unsigned)time(NULL));

	int times_through_loop = 0;

	hang_out( studentID, 1+ random() % MAX_SLEEP_TIME);

	while (times_through_loop++ < 5) {

		/* acquire the mutex lock */
		pthread_mutex_lock( &chairs );

		sem_getvalue( &butts,&quant );

		/* is there a seat available */
		if ( quant < MAX_WAITING_STUDENTS  ) {

			//increment people counter
			sem_post( &butts);

			printf("\t\tStudent %d takes a seat waiting = %d\n",studentID ,quant+1 );
			pthread_mutex_unlock( &chairs );

			//get in first seat
			sem_wait( &seat[0]);
			//printf("first lock\n" );

			//get in second seat and release first
			sem_wait( &seat[1]);
			sem_post( &seat[0]);
			//printf("second lock\n" );

			//get in third seat and release second
			sem_wait( &seat[2]);
			sem_post( &seat[1]);
			//printf("Third lock\n" );

			//indicate to TA that there is someone to help	
			sem_post( &TAhelp );
			printf("Student %d receiving help\n",studentID );
			pthread_mutex_lock( &door );


		} 
		else {
			printf("\t\t\tStudent %d will try later\n", studentID);
			pthread_mutex_unlock(&chairs);
		}

		hang_out( studentID, 1+ random() % MAX_SLEEP_TIME);
	}
}

