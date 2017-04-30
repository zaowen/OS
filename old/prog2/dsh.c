// Author: Zachary Owen
// Instructor: Dr. McGough
// Class: Operating Systems
//  
// Program Name: Diagnostic Shell (dsh) 2.0
//
// Due date: 03 14 16
//
// compilation instr:
//	cc -pthreads -o dsh dsh.c
//
// Program errors: Some threaded functions can produce unexpected consequenses. 
//
// Program timeline:
//  Jan 29th begun program creation v1.0
//  Feb 10th All code lost in Major crash, not yet known
//  Feb 12th Computer access regained, begun production of v2.0
//  Feb 13th Rewrote all calling routines and program loop
//  Feb 14th begun work using spawn() and replacing use of system()
//  Feb 15th All functionality implemented, some testing, Documentation
//  Mar  6th Began work on dsh 2 functionality
//  Mar  9th Pthreads fully implemented
//  Mar 11th file redirects finished
//  Mar 12th Pipes finished
//  Mar 14th Remote pipes finished
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
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MYPROMPT "\ndsh> "

void prompt();
int parse( char * line , char * tokens[100], int len);
void printtime();
int checkdef( char * token);
int findspace( char * line, int start);
int execute( char * line, int read);
int definedrun( char * line[100] , int tokennum, int id);
int systemrun (char * line[100] , int tokennum );
int spawn (char* program, char** arg_list);
int isitdigit( char * pn );
int outspawn( char* program, char ** arg_list, char* filename  );
int inspawn( char* program, char ** arg_list, char* filename  );
int pipespawn ( char * fromprog, char** fparg_list, char* toprog, char** tparg_list );

int clientspawn( char* program, char ** arg_list, char* targeaddr , char* port  );
int serverspawn( char* program, char ** arg_list, char* port  );

/* THREADED FUNCTIONS */
void* hbthread( void* arg);
void * cmdnmthread( void * arg );
void * systatthread( void * arg );

/* INTRISIC FUNCITONS*/
int  cmdnm( char * line[100] , int tokens);
int mysignal( char * line[100] , int tokens);
int systat( char * line[100] , int tokens);
int     cd( char * line[100] , int tokens);
int    pwd( char * line[100] , int tokens);
int     hb( char * line[100] , int tokens);

char * instr[] = { "cmdnm","signal","systat","exit","cd", "pwd", "hb"};
int  (*instrfunc[])( char ** , int ) = { cmdnm, mysignal, systat, exit, cd, pwd, hb }; 


int main( int argc, char * argv[] ) {
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int run = 1;

	while( run )
	{
		prompt();
		read = getline( &line, &len, stdin  );
		execute( line , read );
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
int execute( char * line, int read )
{
	char * tokens[100];
	int tokennum = 0;	
	int id;	


	int i;

	tokennum = parse ( line , tokens, read );

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

	for( i = 0; i < tokennum ; i++)
	{
		free(tokens[i]);
		tokens[i] == NULL;
	}

	return 1;

}

char* make_token( char src[], int i, int prev)
{
	char * token;

	token = (char*) malloc( i - prev + 1);
	if( token == NULL)
	{
		printf( "Memory Creation Failed\nExiting");
		exit( 1 );
	}

	strncpy( token ,  &src[prev] , i - prev);
	token[i-prev] = '\0';


	return token;
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
int parse( char * line , char * tokens[100], int len) {

	int i;
	char temp[100];
	int tokennum =0;
	int next, prev = 0;


	while( line[prev] == ' ') { prev++;}

	if( line[0] == '\n')
	{
		return 0;
	}

	for( i = prev; i < len; i++)
	{
		if( line[i] == ' ' || line[i] == '\n')
		{
			tokens[tokennum] = make_token( line, i , prev );
			prev = i + 1;
			tokennum = tokennum + 1;
			while( line[prev] == ' ' ||  line[prev] == '\n' ) { prev++;}
			i = prev;
		}
	}

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


	for( i = 0; i < 7 ; i++)
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
	int alloc;

	if( dir == NULL)
	{
		dir = malloc(PATH_MAX);
		getcwd( dir , PATH_MAX); 
	}

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

	if( alloc )
	{
		free(dir);
	}

	return 0;
}


/*
	cmdnm
	Function to create a Thread that contains a function that prints the command used to create a process
	Performs checks for input correctness before creating the thread.
*/
int  cmdnm( char * line[100] , int tokens)
{
	char foo[100] = "/proc/\0";
	char * bar = "/cmdline";
	pthread_t myt;

	if( tokens < 2) // make sure correct number of tokens
	{
		printf("Usage: cmdnm <pid>\n");
		return -1;
	}	
	if( !isitdigit( line[1] ) ) // Its really a pid number 
	{
		printf( "Entered <pid> is not a digit.\n");
		return -1;
	}	

	if( ! dirchk( line[1] , "/proc") ) // make sure something is there for us to open
	{
		printf( "<pid> is not in use");
		return -1;
	}	

	//concatenate the path to the file to read
	strcat( foo, line[1]);
	strcat( foo, bar);

	//spins up the thread
	pthread_create( &myt, NULL, cmdnmthread, (void*)foo );
	//wait for thread to finish
	pthread_join( myt, NULL );

	return 1;	
}

/*
	cmdnmthread
	The function called as a thread from cmdnm(), prints contence of a file refered to in the arg void pointer at the 0x0 offset	
*/
void * cmdnmthread( void * arg )
{

	FILE *fp;
	//get the first arg as file
	char * foo = (char*) arg;
	char buffer;	

	//open file to read
	fp = fopen( foo, "r");

	//read ad EOFinitem
	if( fp != NULL)
	{
		buffer = getc( fp);
		while( 	buffer != EOF )
		{
			putc( buffer, stdout );
			buffer = getc( fp );
		}
	}
	else
	{
		printf("Could not open cmdline for reading\nPlease check permissions");
	}

	fclose(fp);
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
	pthread_t myt;

	pthread_create( &myt, NULL, systatthread, NULL );
	pthread_join( myt , NULL );
}

/*
	Systatthread
	does all the thing systat does but with diffrent return values and inputs. Did I mention it's threaded!
	and it can all by yours for the low low price of $19.99. Throw away you're old thread and get some PTHREADS
*/
void * systatthread( void * arg )
{

	FILE * fp;
	char b[2048];

	fp = fopen("/proc/version", "r" );
	
	if( fp == NULL )
	{
	printf("Version could not be opened");
	return -1;
	}
	
	printf ("linux-version:\n");
	b[0] = fgetc(fp);

	while( b[0] != EOF )
	{
		printf( "%c" , b[0] );
		b[0] = fgetc(fp);
	}
	printf("\n");
	fclose(fp);

	printf( "Uptime: \n" );


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
   NEW: Also checks for a single instance of Pipe or Redirect and will execute accordingly. Pipes/Redirects must be seperated by whitespace and can use )) or (( to establish remote connections
 */
int systemrun (char * line[100] , int tokennum )
{
	int i;

	//Make sure the first program to run is in /bin
	if( dirchk( line[0] , "/bin" ) )
	{

		//if so iterate through tokens to find pipes or redirects
		for( i = 0 ; i < tokennum; i++ ) 
		{
			if(       line[i][0] == '|' ) // Pipe
			{
				//make sure the second program exists as well
				if( dirchk( line[i+1] , "/bin" ) ) 
				{
					//place NULL's for execs
					line[i] = NULL;
					line[tokennum] = NULL;
					
					pipespawn(line[0] , line, line[i+1], &line[i+1] );
					return 0;
				}
				else
				{
					printf("Second program file error\n");
					return -1;
				}
			}
			else if ( line[i][0] == '<' )// Redirect in
			{
				//place NuLL for exec
				line[i] = NULL;

				//check for atleast 1 extra argument
				if( tokennum > i )	
				{
					inspawn( line[0] , line, line[i+1] ) ;
					return 0;
				}	
				else // if we don't return error
				{
					printf("Input file not given" );
					return -1;
				}
			}	
			else if ( line[i][0] == '>' ) // redirect out
			{
				//Place NulL for exec
				line[i] = NULL;

				// check for proper number of args
				if( tokennum > i )	
				{
					outspawn( line[0] , line, line[i+1] ) ;
					return 0;
				}	
				else // if we dont return error
				{
					printf("Input file not given" );
					return -1;
				}//fi
			}//fi
			else if ( !strcmp( line[i] , "))" ) ) // remote shell out
			{
				line[i] = NULL;
				if( tokennum > i+1) // check for right number of args
				{
					serverspawn( line[0] , line, line[i+1] );
					return 0;
				}
				else // error if not
				{
				printf("Improper Remote shell call\n\tUsage:\n\tcmd )) <port> " );
					return -1;
				}
			}
			else if( !strcmp( line[i] , "((" ) ) //remote shell in
			{
				printf("Found get case\n" );
				line[i] = NULL;
				if( tokennum > i+2) // check for at least 2 args
				{
					clientspawn( line[0] , line, line[i+1] , line[i+2] );
					return 0;
				}
				else // error if not
				{
				printf("Improper Remote shell call\n\tUsage:\n\tcmd (( <server IP> <port> " );
					return -1;
				}
			}

		} // rof

		//normal spawn
		//place NULL for exec
		line[tokennum] = NULL;
		spawn( line[0] , line );
		return 0;

	}
	else
	{
		printf("Command not found\n");
		return -1;
	}//fi
}

/*
	clientspawn
	Contains socket code from http://www.linuxhowtos.org/C_C++/socket.htm
	Creates a client that can recive input from a stream that is piped to a unix program.
	The function then goes about its merry way wiht the standard fork()/exec() process while replacing STDIN with the socket. The parent then closes the socket because weird things kept happening when it was in the child.

*/
int clientspawn( char* program, char ** arg_list, char* targeaddr , char* port  )
{
	//START SOCKET CODE
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	//Socket CODE
	portno = atoi(port );
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		{ 
		error("ERROR opening socket");
		return -1;
		}
	server = gethostbyname( targeaddr );
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	//END SOCKET CODE

	pid_t child_pid;
	int temp;
	siginfo_t * inflop;
	idtype_t mytype = P_ALL;
	FILE *procstat;
	char statname[100] = "/proc/";
	char pidnum[20];



	/* Duplicate this process.  */
	child_pid = fork ();
	if (child_pid != 0){

		//make sure the socket is closed on our end
		close(sockfd);
		
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
		//replace STDIN wiht our socket		
		dup2( sockfd , STDIN_FILENO); 
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort();
	}

}

/*
	serverspawn
	Contains socket code from http://www.linuxhowtos.org/C_C++/socket.htm
	Creates a server that can send output from a stream that is piped from a unix program.
	The function then goes about its merry way wiht the standard fork()/exec() process while replacing STDOUT AND STDIN with the socket. The parent then closes the socket because weird things kept happening when it was in the child.
*/
int serverspawn( char* program, char ** arg_list, char* port  )
{
	//SOCKET CODE
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		{
		error("ERROR opening socket");
		return -1;
		}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi( port );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, 
			(struct sockaddr *) &cli_addr, 
			&clilen);
	if (newsockfd < 0)
		{ 
		error("ERROR on accept");
		return -1;
		}
	//SOCKET CODE

	pid_t child_pid;
	int temp;
	siginfo_t * inflop;
	idtype_t mytype = P_ALL;
	FILE *procstat;
	char statname[100] = "/proc/";
	char pidnum[20];



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

		//close(sockfd);
		close(newsockfd);
		wait(&temp);
		return 1;
	}
	else {
		//In somecases the program would complain about broken pipes. replacing both seemed to fix it.
		dup2( newsockfd , STDOUT_FILENO); 
		dup2( newsockfd , STDIN_FILENO); 
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort();
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
	PipeSpawn
	Spawns 2 processes one piped into the other. The process names are stored in Fromprog and ToProg. I forget which is piped to which.
	Uses pipe() to create an anonymous pipeline that fromprog can use to communicate with toprog. The communication is unidirectional.and as such the useless ends are closed before before the call to exec()

*/
int pipespawn ( char * fromprog, char** fparg_list, char* toprog, char** tparg_list )
{

	pid_t child_pid;
	int temp;
	siginfo_t * inflop;
	idtype_t mytype = P_ALL;
	FILE *procstat;
	char statname[100] = "/proc/";
	char pidnum[20];

	//pipe variable
	int mypipes[2];

	//creates pipes
	pipe( mypipes );	

	/* Duplicate this process.  */
	child_pid = fork ();
	if (child_pid != 0){
		/* This is the parent process.  */

		printf( "Child process spawned with pid:%d\n", child_pid );	

		wait(&temp);
		child_pid = fork();

		if( child_pid != 0)
		{ 

			printf( "Child process spawned with pid:%d\n", child_pid );	

			//close both pipes in parent
			close( mypipes[1] ) ;
			close( mypipes[0] );

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
		else // TO child fork
		{	
			//replace standard input with the pipe
			dup2( mypipes[0] , STDIN_FILENO );
			//close the output side
			close( mypipes[1] );

			/* Now execute PROGRAM, searching for it in the path.  */
			execvp ( toprog, tparg_list);

			/* The execvp function returns only if an error occurs.  */
			fprintf (stderr, "an error occurred in execvp\n");
			abort();
		}
	}
	else // From program fork 
	{
		//replace Standard out with the pipe
		dup2( mypipes[1] , STDOUT_FILENO );
		//Close the input side
		close( mypipes[0] );

		/* Now execute PROGRAM, searching for it in the path.  */
		execvp ( fromprog, fparg_list);

		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort();
	}

}

/*
	outspawn
	Spawns a proccess with its Standard out replaced with a file pointer to a file the process creates. Since there is no additional setup required File pointer is created in the child fork.
	 filename is the name of the file to create for output
*/
int outspawn( char* program, char ** arg_list, char* filename  )
{

	pid_t child_pid;
	int temp;
	siginfo_t * inflop;
	idtype_t mytype = P_ALL;
	FILE *procstat;
	char statname[100] = "/proc/";
	char pidnum[20];



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
		FILE * fp;
		fp = freopen ( filename , "w" , stdout ); 
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort();
	}

}

/*
	inspawn
	Spawns a proccess with its Standard in replaced with a file pointer to a file the process creates. Since there is no additional setup required File pointer is created in the child fork. Although we do check to see that the file exists before attmepting the fork.
	 filename is the name of the file to read from.
*/
int inspawn( char* program, char ** arg_list, char* filename  )
{
	pid_t child_pid;
	int temp;
	siginfo_t * inflop;
	idtype_t mytype = P_ALL;
	FILE *procstat;
	char statname[100] = "/proc/";
	char pidnum[20];


	if( !dirchk( filename , NULL ) )
	{
		printf("inputfile not found exiting" );
		return 0;
	}

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
		FILE * fp;
		fp = freopen ( filename , "r" , stdin ); 
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort();
	}
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

/*
	hb or HeartBleed
	This function parses the input given to it from the command line and prints a heart beat based on it. it accepts at least 3 inputs or more but ignores the rest. after the parce the information needed is packaged and threaded  and the main thread waits for it to finish. 
 */
int hb ( char* line[100] , int tokennum)
{
	pthread_t myt;

	int interval;
	int end;
	int arg[2];

	if( tokennum < 4)// if the usage is blatantly wrong print usage statement
	{
		printf("Usage:\n\thb tinc tend tval\n\n\ttinc: time increment\n\ttend: runtime\n\ttval specified units { s | ms }.\n");
		return -1;
	}

	if( !isitdigit(line[1]) ) // make sure tinc is digit
	{
		printf("Entered value for tinc is not a digit.");
		return -1;
	}

	if( !isitdigit(line[2]) ) // make sure tend is digit
	{
		printf("Entered value for tend is not a digit.");
		return -1;
	}

	//Since we're using microsleep set time incriments to millisecond by default
	interval = atof( line[1] ) * 1000;
	end      = atof( line[2] ) * 1000;


	if( !strcmp(line[3], "s")) // change to second if thats the case
	{
		//printf("seconds\n");
		interval = interval * 1000;
		end      = end      * 1000;
	}
	else if ( !strcmp( line[3] , "ms" ) ) //We dont actually need to do anything here we just need to escapse the else
	{
		//printf("miliseconds\n");
	}
	else // if it's not ms or s errorstate!
	{
		printf("Invalid tval value, seek help"); 
		return -1;
	}		
	//package our input vars
	arg[0] = interval;
	arg[1] = end;

	//Create threads and join
	pthread_create( &myt, NULL, hbthread, (void*)arg );
	pthread_join( myt, NULL );
	return 0;	
}

/*
	hbthread
	The function called by HB() as a thread. Starts by unpacking the void* arg into tinc and tend. it then prints the time while sleeping and decrementing tinc from tend until less than zero.
*/
void *  hbthread( void* arg)
{
	int * myargs = arg;

	int tinc = myargs[0];
	int tend = myargs[1];

	for( ; tend >= 0; tend -= tinc )
	{
		printtime();
		usleep( tinc );
	}
	return;
}

/*

	printtime
	Helperfunction to easily print the time in format HH:MM:SS
*/
void printtime()
{

	char buffer[100];
	struct tm * mytime;
	time_t rawtime;

	time( &rawtime);
	mytime = localtime( &rawtime);

	strftime( buffer, 100, "%T\n" , mytime );

	printf( "%s" , buffer );
}
