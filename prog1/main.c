// Author: Zachary Owen
// Instructor: Dr. McGough
// Class: Operating Systems
//  
// Program Name: Diagnostic Shell (dsh) 2.0
//
// compilation instr:
//	cc -o dsh dsh.c
//
// Program error: in short, minor corruption issues some crashes. Expounded in prog1.pdf
//
// Program timeline:
//  Jan 29th begun program creation v1.0
//  Feb 10th All code lost in Major crash, not yet known
//  Feb 12th Computer access regained, begun production of v2.0
//  Feb 13th Rewrote all calling routines and program loop
//  Feb 14th begun work using spawn() and replacing use of system()
//  Feb 15th All functionality implemented, some testing, Documentation
//  
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

#define MYPROMPT "\ndsh> "

void prompt();
int parse( char * line , char * tokens[100]);
int checkdef( char * token);
int findspace( char * line, int start);
int execute( char * line);
int definedrun( char * line[100] , int tokennum, int id);
int systemrun (char * line[100] , int tokennum );
int spawn (char* program, char** arg_list);

/* INTRISIC FUNCITONS*/
int  cmdnm( char * line[100] , int tokens);
int mysignal( char * line[100] , int tokens);
int systat( char * line[100] , int tokens);
int     cd( char * line[100] , int tokens);
int    pwd( char * line[100] , int tokens);

char * instr[] = { "cmdnm","signal","systat","exit","cd", "pwd"};
int  (*instrfunc[])( char ** , int ) = { cmdnm, mysignal, systat, exit, cd, pwd }; 


int main( int argc, char * argv[] ) {
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int run = 1;

	while( run )
	{
		prompt();
		getline( &line, &len, stdin  );
		execute( line );
		//sleep ( 1 );
	}

}

/*
   void prompt : prints the prompt
 */
void prompt() {
	printf(MYPROMPT);
}

/*
   Int findspace

   Returns the index of the first whitespace character 
   found or -1 if none were found

 */
int findspace( char * line, int start ) { 
	int i;
	for( i = start; line[i] == '\0'; i++)
	{
		if ( line[i] == '\n' | line[i] == ' ' )
		{
			return i;
		}
	}

	return -1;
}

/*
int execute
	
   Does all parsing and passing to proper functions to execute user input

   returns based on user input function termination
*/
int execute( char * line )
{
	char * tokens[100];
	int tokennum = 0;	
	int id;	
	int i;

	tokennum = parse ( line , tokens);

	if( tokennum > 0 )
	{
		id = checkdef( tokens[0] );

		if ( id == -1 )
		{
			systemrun( tokens, tokennum );
		}
		else
		{
			definedrun(  tokens ,tokennum , id );
		}
	}

	for( i = 0; i < tokennum; i++)

	{
	free(tokens[i]);
	}
	
return 1;

}

/*
Parse:  Seperates and tokenizes and "returns" the results given console input

INPUT:
line:  string to tokenize and count
tokens: Psudoreturn where we store the tokens, uses dynamic memory should be freed elsewhere
OUTPUT: 
int: number of tokens in tokens;

Notes: Only seperates based on ' ' whitespace.

 */
int parse( char * line , char * tokens[100] ) {

	int i;
	char temp[100];
	int tokennum =0;
	int next, prev = 0;


	while( line[prev] == ' ') { prev++;}

	if( line[0] == '\n')
	{
	return 0;
	}

	for( i = prev; line[i] != '\0'; i++)
	{
		if( line[i] == ' ' )
		{
			tokens[ tokennum ] = (char*) malloc( i - prev);
			strncpy( tokens [ tokennum ],  &line[prev] , i - prev);
			prev = i+1;
			tokennum = tokennum + 1;

			while( line[prev] == ' ') { prev++;}
			i = prev;
		}
	}

	tokens[ tokennum ] = (char*) malloc( i - prev);
	strncpy( tokens[tokennum] ,  &line[prev] , i - prev-1 );
	prev = 0;
	tokennum = tokennum + 1;

	//printf ( "returns %d tokens\n", tokennum);
	return tokennum;
}

/*
   Checkdef


   for loop, goes through array of intrinsic funtions and returns index of found function or -1 if not in array
 */
int checkdef( char * token  ) {
	int i;
	int found = 0;  


	for( i = 0; i < 6 ; i++)
	{
		found = strcmp( token , instr[i] ) ;
		if(!found)
		{
			return i;
		}	
	}

	return -1;
}

/*
   int isitdigit

   checks every character in array until \0 returns 1 if each is digit and 0 if one is found
*/
int isitdigit( char * pn )
{
	int i = 0;
	while ( pn[i] != '\0')
	{
		if( ! isdigit( pn[i] ) )
		{
			return 0;
		}
		i++;
	}
	return 1;
}

/*
   int dirchk

   Foundation of this found on stackexchange. Atomized enough to make rewriting pointless.
   Checks given directory for name given in pid. Originally used to find pid directories in /proc. Proved usefull elsewhere
*/
int dirchk( char * pid , char * dir)
{
	DIR *pDir;
	struct dirent *pDirent;

	pDir = opendir( dir );

	if( pDir == NULL )
	{
		return 0;
	}

	while ((pDirent = readdir(pDir)) != NULL) {
		if( !strcmp( pDirent->d_name, pid ) )
		{
			return 1;
		}
	}
	closedir (pDir);
	return 0;
}

int  cmdnm( char * line[100] , int tokens)
{
	char foo[100] = "/proc/\0";
	char * bar = "/cmdline";

	if( tokens < 2)
	{
		printf("Usage: cmdnm <pid>\n");
		return -1;
	}	
	if( !isitdigit( line[1] ) )
	{
		printf( "Entered <pid> is not a digit.\n");
		return -1;
	}	

	//if( ! dirchk( line[1] , "/proc") )
	if( ! dirchk( line[1] , "/proc") )
	{
		printf( "<pid> is not in use");
		return -1;
	}	

	strcat( foo, line[1]);
	strcat( foo, bar);

	char* herp[] = { "cat", foo , NULL };

	spawn( "cat" , herp);


	return 1;	
}


/*
   int mysignal

   sends signal to given pid aslong as pid directory exists in /proc and is >20. There were scary problems  if you send messages in that range.
*/
int mysignal( char * line[100] , int tokens)
{
	pid_t mypid;
	int mysignal;

	// make sure we have enough tokens to continue
	if( tokens < 3)
	{
		printf("Usage: signal <signal_num> <pid>\n");
		return 0;
	}

	mypid = (pid_t)atoi(line[2] );
	mysignal = atoi (line[1] ) ;

	// check that the signal is a number
	if( mysignal < 1  )
	{
		printf( "Invalid signal.");
		return 0;
	}	
	if( mysignal > 20  )
	{
		printf( "Invalid signal.");
		return 0;
	}	

	// make sure process exists
	if( kill( mypid , 0  ) == -1 )
	{
		printf("Process does not exist." );
		return 0;
	}


	if ( kill( mypid , mysignal ) == -1 ) 
	{
		printf( "Signal failed for some reason." );
	}

	return 1;
}


/*
   int systat

   Outputs a host of system data including computer name, uptime and memory usage.
*/
int systat( char * line[100] , int tokens)
{

	char* uname[] = { "uname" , "-r" , NULL } ; 
	spawn( uname[0] , uname);

	char* uptime[] = { "uptime"  , NULL } ; 
	spawn( uptime[0] , uptime);

	//printf(" memory usage information:  memtotal and memfree.");
	char* memory[] = { "free"  , "-h" , NULL } ; 
	spawn( memory[0] , memory);

	printf(" cpu information:  vendor id through cache size.");

}

/*
   cd

   changes directory has minor error correcting checks
 */
int     cd( char * line[100] , int tokens)
{
	if( tokens == 1)
	{
		printf ( "Usage, cd <directory>\n" );
		return 0;
	}

	if ( chdir( line[1] ) )
	{
		printf( "Could not cd\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
   int pwd

   calls pwd function.
*/
int    pwd( char * line[100] , int tokens)
{
	char * args[] = { "pwd" , NULL };
	spawn( "pwd" , args );

	return 1;
}


/*
   int definedrun

   accesses the function pointer array on behalf of execute() and runs intrinsic functions
*/
int definedrun(  char * line[100] , int tokennum, int id)
{
	if( line[0][0] == ' ')
	{
		return 1;
	}
	else
	{
		instrfunc[id]( line, tokennum);
	}
}

/*
   int systemrun 

   makes sure if the user asked funtion isn't intrinisic exists in /bin. If not gives error message. If so adds a NULL to the end of the program arguments and passes them to spawn()
*/
int systemrun (char * line[100] , int tokennum )
{

	if( dirchk( line[0] , "/bin" ) )
	{
	line[tokennum] = NULL;
	spawn( line[0] , line );
	}
	else
	{
	printf("Command not found\n");
	}
}

/*
    void printstat

    Prints ciertan values in opened file pointer. Supposed to read the page faults, user and system times and swaps. Has a tendency to print zeros for reasons the programmer was unable to gleam from the man pages.
*/
void printstat( FILE * myfile )
{
	int utime;
	int stime;
	int page;
	int swap;
	int temp;
	int i;
	char * derp;


	for( i = 0; i < 10; i++)
	fscanf( myfile , "%*s" );

	fscanf( myfile, "%lu" , &page);
		
	
	for( i = 0; i < 3; i++)
	fscanf( myfile , "%*s" );

	fscanf( myfile, "%lu" , &utime);
	
	fscanf( myfile, "%lu" , &stime);

	for( i = 0; i < 15-36; i++)
	fscanf( myfile, "%*s");	

	fscanf( myfile , "%lu" , &page );


	printf( "User time: %lu, System time: %lu\nPage Faults: %lu, Swaps: %lu\n", utime, stime,page,swap);
	return;
}


/*
   int spawn

   Shamelessly stolen from https://d2l.sdbor.edu/content/enforced/2015FA/777862-CSC_456_M001_2016SP_538486/Code_Examples.html though heavily altered to fit program secifications.

   given program name and properly set up argument lists runs will fork and exec system programs and wait for them to return.  
*/
int spawn (char* program, char** arg_list)
{
	pid_t child_pid;
	int temp;
	siginfo_t *  inflop;
	idtype_t mytype = P_ALL;
	FILE *procstat;
	char statname[100] = "/proc/";	
	char  pidnum[20];


	/* Duplicate this process.  */
	child_pid = fork ();
	if (child_pid != 0){
		/* This is the parent process.  */

		printf( "Child process spawned with pid:%d\n", child_pid );	

		waitid( P_PID , child_pid , inflop , WNOWAIT|WEXITED ); 
	
		sprintf( pidnum ,"%d", child_pid);		

		strcat( statname, pidnum);
		strcat( statname, "/stat");

		if (( procstat = fopen( statname , "r")) == NULL)
		{
			fprintf(stderr, "Cannot open %s\n", statname );	
			return 0;
		}

		printstat( procstat );		
		

		wait(&temp);
		return 1;
	}
	else {
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort ();
	}
}
