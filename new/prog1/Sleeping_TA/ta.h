/**
 * Header file for sleeping TA
 */
#ifndef TA_H
#define TA_H

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* the maximum time (in seconds) to sleep */
#define MAX_SLEEP_TIME         5

/* number of maximum waiting students */
#define MAX_WAITING_STUDENTS   3

/* number of potential students */
#define NUM_OF_STUDENTS        5

/* number of available seats */
#define NUM_OF_SEATS           3

/* semaphores and mutex lock */
pthread_mutex_t chairs;
pthread_mutex_t door;

/* semaphore declarations */
sem_t butts;
sem_t seat[3];
sem_t TAhelp;

/* student function prototype */
void *student_loop(void*);
void hang_out(int lnumber, int sleep_time);

/* ta function prototype */
void *ta_loop(void*);
void help_student(int sleep_time, int buuuuuts);

#endif
