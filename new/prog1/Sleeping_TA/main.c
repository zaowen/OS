#include "ta.h"

pthread_t ta;
pthread_t students[NUM_OF_STUDENTS];

/**
 * Initialize all relevant data structures and
 * synchronization objects.
 */
void init()
{
	int i;

	if ( pthread_mutex_init(&chairs, NULL) )
		printf("%s\n",strerror(errno));

	if ( pthread_mutex_init(&door, NULL) )
		printf("%s\n",strerror(errno));

	if (sem_init(&butts, 0, 0 ) )
		printf("error init students_sem\n"); 

	if ( sem_init(&TAhelp, 0, 0) )
		printf("error init ta\n");


	for (i = 0; i < MAX_WAITING_STUDENTS; i++)
		sem_init( &seat[i], 0, 1);


}

void dinit(){
	int i;

	pthread_mutex_destroy( &chairs);
	pthread_mutex_destroy( &door);
	
	sem_destroy( &butts);
	sem_destroy( &TAhelp);

	for (i = 0; i < MAX_WAITING_STUDENTS; i++)
		sem_destroy( &seat[i]);


}

void create_students()
{
	int64_t i;

	for (i = 0; i < NUM_OF_STUDENTS; i++) {
		pthread_create( &students[i], NULL, &student_loop, (void*)i+1 );
	}
}

void create_ta()
{
	pthread_create( &ta, NULL,&ta_loop, NULL );
}

int main(void)
{
	int i;

	init();
	create_ta();
	create_students();

	for (i = 0; i < NUM_OF_STUDENTS; i++)
		pthread_join(students[i], NULL);

	/* when all students have finished, we will cancel the TA thread */	
	if (pthread_cancel(ta) != 0)
		printf("%s\n",strerror(errno));

	dinit();
	return 0;
}

