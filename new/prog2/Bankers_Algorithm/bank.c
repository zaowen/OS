#ifndef BANK_H
#define BANK_H

#include <pthread.h>
/* these may be any values >= 0 */ 
#define NUMBER_OF_CUSTOMERS 5 
#define NUMBER_OF_RESOURCES 3

#define CUSTOMER_GOES 10
#define HEAD_SLEEP 2


/* the available amount of each resource */ 
int available[NUMBER_OF_RESOURCES]; 

/*the maximum demand of each customer */ 
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 

/* the amount currently allocated to each customer */ 
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 

/* the remaining need of each customer */ 
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 

/* the available amount of each resource */ 
int MAX_RESOURCE[NUMBER_OF_RESOURCES]; 

//pthread
pthread_mutex_t READ;


#endif 
