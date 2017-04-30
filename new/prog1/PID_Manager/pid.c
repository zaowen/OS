/*
 * A solution to the pid manager problem. 
 *
 */

#include "pid.h"
#include <pthread.h>
#include <stdio.h>

// Allocates the pid map.
int allocate_map(void) 
{
	int i;
	for (i = 0; i <= PID_MAX; i++) {
		pid_map[i] = 0;
	}
	
   return PID_MIN;
}

// Allocates a pid
int allocate_pid(void)
{
   /* local variables */
	int pid;

   /* acquire the mutex lock and warn if unable */
	if(pthread_mutex_lock ( &test_mutex)  )
		printf("Mutex Lock failed.\n" );

   /* find the next available pid */
	pid = get_id();

	pid_map[pid] = 1;

   /* release and warn if the mutex was not released */
	if(pthread_mutex_unlock ( &test_mutex) )
		printf("Mutex Unlock failed in pid %d.\n", pid);

   return pid;
}

// Releases a pid
void release_pid(int pid)
{
	/* acquire the mutex lock and warn if unable */
	if(pthread_mutex_lock ( &test_mutex)  )
		printf("Mutex Lock failed in pid %d.\n", pid);

	/* release the pid */
	//single locked shared memroy
	pid_map[pid] = 0;

   /* release and warn if the mutex was not released  */
	if(pthread_mutex_unlock ( &test_mutex)  )
		printf("Mutex Unlock failed in pid %d.\n", pid);
		

	printf( "Released %d\n" , pid );

}

int get_id( void ){
	int i;
		//printf("in ID \n");

	for( i = PID_MIN; i < PID_MAX; i++){
		//printf("chekin if %d  is %d \n", i,  pid_map[i]);
		if( !pid_map[i] ){
			return i;
		}
	}
		return -1;
}
