/************************************************************************
Program: dipcrm
Author: Zachary Owen
Class: Operating Systems
Instructor: Dr. McGough
Date: April 6th 2016
Description: Reads the .serverpid file in the pwd() and then kills the process. Must be run in the same directory as dipc
Input: None
Output: None
Compilation instructions: cc dipcrm.c -o dipcrm
Usage: ./dpicrm
Known bugs/missing features: Has to read the .serverpid file in the current directory.
Modifications: None
Date                Comment            
----    ------------------------------------------------
4/4	Wrote program.
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<signal.h>

int main()
{
	FILE * fp;
	pid_t pid;

	fp = fopen( ".serverpid" , "r" );

	if( fp == NULL )
	{
		printf("Could not find .serverpid\nNot Exiting\n");
		return -1;
	}
	else
	{
		fscanf( fp , "%d" , &pid );

		if ( kill( pid, 9 )  == -1 )
		{
			printf( "Could not kill server.\n" );
			fclose( fp );
			return -1;
		}
		else
		{
			if( remove( ".serverpid" ) == -1 )
			{
				printf("Problem removing pid.\n");
				return -1;
			}
		}


	}

	return 0;
}
