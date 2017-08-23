/**
 * Simulate helping a student
 */

#include "ta.h"

void help_student(int sleep_time , int butttoxes )
{
   printf("Helping a student for %d seconds waiting students = %d\n",sleep_time, butttoxes);
   sleep( sleep_time );
}

