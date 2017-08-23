#include <pthread.h>

#define PID_MIN  	300
#define PID_MAX 	500

/* mutex lock for accessing pid_map */
pthread_mutex_t test_mutex;

int pid_map[PID_MAX+1];

int allocate_pid(void);

int get_id( void );

int relase_pid(void);


int last;	// last pid in use
