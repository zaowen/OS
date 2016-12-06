/************************************************************************
Program: Daring Internet Protocal Chat
Author: Zachary Owen	
Class: Operating Systems
Instructor: Dr.McGough
Date: 4/6/2016
Description: Simple Internet Relay mailbox
Input: This program needs to know the propertis of the mailboxes including
number of boxes to create, their sizes, the port to bind on and 
the size of packets to accept
Output: None
Compilation instructions: cc -pthread dipc.c -o dipc
Usage:  dipc <number of mailboxes>  <size of mailbox in kbytes>  <port>  <size of packet in kbytes>
Known bugs/missing features: More things should have been Global 
Would have been nice to make Semaphore createion dynamic
Same with port selection
Shutdown could be handled more gracefully
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>  //atof
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> 

#include<sys/types.h> //Shared memory
#include<sys/shm.h>
#include<sys/ipc.h>

#define SEMKEY 1337 //Uses a block of Shared memory segements starting at 1337

int setargs( int * boxnum, int * boxsize, int * port, int * pacsize, char ** argv );
int AcceptClient( int * read_size,  int socket_desc , pthread_mutex_t * W_Mutex, pthread_mutex_t * R_Mutex, int boxnum);
int SetupBind(int port );
void * CommThread( void * arg );
int * CreateStable( int boxnum, int boxsize, char * marebox );
int Parse( char * in, int * read_size, int boxnum, int client_sock , pthread_mutex_t * WM , pthread_mutex_t * RM );
int Tokenize( char * line , char * tokens[30], int len);
pthread_mutex_t * CreateM( int boxnum );
int ClientWrite( char * mytoken , int token , int boxnum, pthread_mutex_t * WM, pthread_mutex_t * RM, int sock );
int ClientRead( char * mytoken , int token , int boxnum, pthread_mutex_t * WM, pthread_mutex_t * RM, int sock );

/*Global Statics*/
int boxsize;
int pacsize;
int * rcounts; 

/************************************************************************
Function: main()
Author: Zacharious Owen
Description: Sets up basic parameters and begins the accept loop
Parameters: 
argc:
count of arguments passed to it
argv:
array of array of inputs
 ************************************************************************/
int main(int argc , char *argv[])
{
	//daemon( 1 , 1 );
	int boxnum, port, flag, socket_desc;
	FILE * fp;
	int * boxnums;	
	char * Share = NULL;
	pthread_mutex_t * W_Mutex;
	pthread_mutex_t * R_Mutex;

	//Make sure proper number of arguments
	if( argc < 5 )
	{
		printf( "Improper Usage: Too few arguments, %d given, 4 expected\ndipc <number of mailboxes>  <size of mailbox in kbytes>  <port>  <size of packet in kbytes>\nExiting\n" , argc - 1  );
		return -1;
	}

	//get args in useable fasion 
	flag = setargs( &boxnum , &boxsize, &port, &pacsize, argv );

	//check for valid args
	if( flag )
	{
		printf("Argument error on argument %d\n", flag * -1 );
		return -1;
	}	

	flag = 0;

	//setup server socket
	socket_desc =  SetupBind( port );

	//make sure Bind was successfull
	if( socket_desc == -1 )
	{
		return -1;
	}

	//create shared memeory
	boxnums = CreateStable( boxnum, boxsize, Share);	

	//create Mutexes
	W_Mutex = CreateM( boxnum );
	R_Mutex = CreateM( boxnum );


	if( W_Mutex == NULL || R_Mutex == NULL )
	{
		printf("Could not Create Mutexes\nExiting");
		return -1;
	}

	if( boxnums == NULL )
	{
		printf("Could not Create Mailboxes \nExiting");
		return -1;
	}

	//fork and deamon the process
	daemon( 1, 1 );	

	//create exit method
	fp = fopen( ".serverpid" , "w" );

	if( fp == NULL )
	{
		printf("Could not setup exit \nExiting");
		return -1;
	}
	else
	{
		printf( "Setting up dipcrm\n" );
		fprintf( fp, "%d\n" , getpid() );
		fclose( fp );
	}

	//Look for Clients
	while( !flag )
	{
		flag = AcceptClient( &pacsize, socket_desc, W_Mutex, R_Mutex, boxnum  );
	}

}


/************************************************************************
Function: count ops()
Author: Zacharious Owen
Description: The Count functions are a series of helper functions that allow the program to keep 
track of the quantity of Readers in any given mailbox so that The 
writers know they have to wait
Parameters:  	m:
Read mutex for the box dex refers to
dex:
Index in the array of shared memory to check
 ************************************************************************/
void inc_count( pthread_mutex_t m, int dex)
{
	pthread_mutex_lock( &m );
	rcounts[dex] = rcounts[dex] + 1;
	pthread_mutex_unlock( &m );
}

void dec_count( pthread_mutex_t m, int dex)
{
	pthread_mutex_lock( &m );
	rcounts[dex] = rcounts[dex] - 1;
	if( rcounts[dex] < 0 )
		rcounts[dex] = 0;
	pthread_mutex_unlock( &m );
}

int get_count( pthread_mutex_t m, int dex)
{
	int c;
	pthread_mutex_lock( &m );
	c = rcounts[dex];
	pthread_mutex_unlock( &m );

	return c;
}


/************************************************************************
Function: AcceptClient()
Author: Zacharious Owen
Description: Sets up basic parameters of a Client thred and Blocks until one 
connects. Hands off important Data to thread
Parameters: 	read_size:
size in bytes to read from stream. Not terribly used
socket_desc:
file descriptor of an open server socket
W_Mutex:
pointer to array of boxnum mutexes. Used to track Writers
R_Mutex:
pointer to array of boxnum mutexes. Used to track Readers
boxnum:
Number of Shared Memory mailboxes to track in thread.

 ************************************************************************/
int AcceptClient( int * read_size,  int socket_desc , pthread_mutex_t * W_Mutex, pthread_mutex_t * R_Mutex, int boxnum)
{
	pthread_t myt;
	int64_t * args;
	int  client_sock;
	int c;
	struct sockaddr_in client;

	//Create argument void pointer
	args = (int64_t*)malloc( (size_t)( ( sizeof(int64_t) ) * 5 ) );

	//Listen
	listen(socket_desc , 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}

	//Put things the thread needs in a void pointer
	args[0] = (int64_t)client_sock;
	args[1] = *read_size;
	args[2] = (int64_t)W_Mutex;
	args[3] = (int64_t)R_Mutex;
	args[4] = (int64_t)boxnum;

	puts("Connection accepted");
	//Spawn thread
	pthread_create( &myt, NULL, CommThread, (void*)args );	

	return 0;
}


/************************************************************************
Function: Comm Thread()
Author: Zacharious Owen
Description: Client thread that talks to the client. Reads data from socket
and Passes data to parcer. Also Handles exits
Parameters:
arg:
Void pointer Objects held are 
sock:
client socket file descriptor
read_size:
how many bytes to read from the stream
 *W_Mutex:
 pointer to Writer Mutex array
 *R_Mutex:
 pointer to Reader Mutex array
boxnum:
Number of mailboxes
 ************************************************************************/
void * CommThread( void * arg )
{
	int64_t * myargs = arg;
	int client_sock, read_size;
	int pack_size;
	char * client_message;
	int boxnum;
	pthread_mutex_t * WM;
	pthread_mutex_t * RM;	


	client_sock = (int)myargs[0];
	pack_size = (int)myargs[1];
	WM = (pthread_mutex_t*)myargs[2];
	RM = (pthread_mutex_t*)myargs[3];
	boxnum = (int)myargs[4];

	client_message = (char*) malloc( (size_t)pack_size ) ;

	if( client_message == NULL )
	{
		printf("Memory allocation Error\nServer Exiting");
		//create function to deal with exit
		return NULL;
	}

	//Receive a message from client
	while( (read_size = recv(client_sock , client_message , pack_size, 0)) > 0  )
	{
		//Determine what to do with message.
		client_message[read_size] = '\0';

		if (  Parse( client_message, &read_size, boxnum, client_sock, WM , RM ) == -1 )
		{
			break;
		}
	} 


	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	write(client_sock , "Goodbye\n"  , 8) ;
	close( client_sock );

	free( arg );
	free( client_message);
	return NULL;
}

/************************************************************************
Function: clientread()
Author: Zacharious Owen
Description: Checks for valid mailbox data then acesses the shared memory.
Assures protecton using a doubly locked Critical section 
and a Counter Mutex
Parameters:	mytoken:
String containing the user input. Must contain atleast 2 strings
token:
Number of strings in mytoken
boxnum: 
I'm just realizing this could have been a static
WM:
pointer to Writer Mutex array
RM:
pointer to Reader Mutex array
sock:
client socket file descriptor; Used to display data
 ************************************************************************/
int ClientRead( char * mytoken , int token , int boxnum, pthread_mutex_t * WM, pthread_mutex_t * RM , int sock)
{
	int wb = -1;
	int key = SEMKEY;
	int shmid;
	char * box;

	if( token < 2 )
	{
		write( sock, "Improper usage\n" , 15 ); 
	}
	else
	{
		wb = (int)atof( mytoken );

		if( wb > 0 && wb < boxnum ) 
		{
			key = key + wb;

			if ((shmid = shmget( key ,boxsize , 0666)) < 0) 
			{
				perror("shmget");
				exit(1);
			}

			if ((box = shmat(shmid, NULL, 0)) == (char *) -1) 
			{
				perror("shmat");
				exit(1);
			}

			//Write hold check
			//spin until write is done
			while( 	pthread_mutex_trylock( &WM[wb] ) != 0 ){}
			pthread_mutex_unlock( &WM[wb] );

			inc_count( RM[wb], wb );

			write( sock, box, strlen(box) ) ;
			write( sock, "\n" , 2 );
			dec_count( RM[wb], wb );

			shmdt ( box );
		}
		else
		{
			write( sock, "Improper usage\n" , 15 ); 
		}	
	}		
	return 0;
}

/************************************************************************
Function: clientwrite()
Author: Zacharious Owen
Description: Checks that the box number in the second entry of mytoken is valid
then prompts the user for text to write to that box. Uses a doubly
locked shared memory section to prevent Reads while writing.
Parameters:	mytoken:
String containing the user input. Must contain atleast 2 strings
token:
Number of strings in mytoken
boxnum: 
I'm just realizing this could have been a static
WM:
pointer to Writer Mutex array
RM:
pointer to Reader Mutex array
sock:
client socket file descriptor; Used to display data
 ************************************************************************/
int ClientWrite( char * mytoken , int token , int boxnum, pthread_mutex_t * WM, pthread_mutex_t * RM, int sock )
{
	int wb = -1;
	int key = SEMKEY;
	int shmid;
	char * box;
	char message[100];
	int read_size;

	if( token < 2 )
	{
		write( sock, "Improper usage\n" , 15 ); 
	}
	else
	{
		wb = (int)atof( mytoken );

		if( wb > 0 && wb < boxnum ) 
		{
			key = key + wb;

			read_size = recv(sock , message , pacsize, 0);
			message[read_size-1] = '\0';

			if ((shmid = shmget( key ,boxsize , 0666)) < 0) 
			{
				perror("shmget");
				exit(1);
			}

			if ((box = shmat(shmid, NULL, 0)) == (char *) -1) 
			{
				perror("shmat");
				exit(1);
			}

			//Write hold check
			//spin until write is done
			while( 	pthread_mutex_trylock( &WM[wb] ) != 0 ){}

			while( get_count( RM[wb] , wb ) != 0 ){}
			//pthread_mutex_lock( &RM[wb] );	

			if( message != NULL )	
			{
				strncpy( box , message , read_size );			
			}

			pthread_mutex_unlock( &WM[wb] );
			shmdt ( box );

			write( sock, "Write successful\n" , 17 ); 
		}
		else
		{
			write( sock, "Improper usage\n" , 15 ); 
		}	
	}		
	return 1;
}

/************************************************************************
Function: CreateM()
Author: Zacharious Owen
Description: Helper function to create an array of mutexes
Parameters: 	boxnum:
number of boxes in main program and number of mutexes to create
 ************************************************************************/
pthread_mutex_t * CreateM( int boxnum )
{
	int i;
	pthread_mutex_t * M;
	pthread_mutexattr_t Attr;

	pthread_mutexattr_settype(&Attr , PTHREAD_MUTEX_RECURSIVE);

	M = malloc( sizeof( pthread_mutex_t) * boxnum );

	if( M == NULL )
	{
		printf("Read Mutex Creation Failed\nExiting");
	}
	else
	{
		for( i = 0; i < boxnum; i = i +1 )
		{
			pthread_mutex_init( &M[i] , &Attr );
		}
	}
	return M;
}

/************************************************************************
Function: CreateStable()
Author: Zacharious Owen
Description: Sets up and creates the Mare Boxes. Get it?
Parameters: 	boxnum:		
number of boxes to create
boxsize:
size (in Kilobytes) of each box
marebox:
Semi-Useless pointer to char array 
 ************************************************************************/
int * CreateStable( int boxnum, int boxsize , char * marebox )
{

	size_t size;
	int i;

	char standard[] = "You've Got Mail 0\n";
	int mylen = strlen(standard) -1 ;
	int * boxid;

	boxid = malloc( boxnum * sizeof(int) );

	rcounts = malloc( boxnum * sizeof(int) );	
	size = boxsize * 1024;


	for( i = 0; i < boxnum; i= i+1 )
	{
		//create shared memory
		rcounts[i] = 0;
		if(  ( boxid[i] = shmget( SEMKEY+i, size, IPC_CREAT | 0666 ) ) < 0 ) 
		{
			//Kindergardener's dont know how to share
			printf( "ShareBlock Unavailable\nExiting" );
			return NULL;
		}

		if( ( marebox = shmat( boxid[i] , NULL, 0 ) ) == (char*) -1 )
		{
			//This joke is a bit dark...
			printf( "Could not Create Shared Memory\nExiting" );
			return NULL;
		}
		strcpy( marebox , standard ) ;
		standard[mylen-1] ++;
	}

	return boxid;
}


/************************************************************************
Function: make_token()
Author: Zacharious Owen
Description: copies part of a character array and returns it. Indexed based
on i and prev	
Parameters:
src:
String to take from	
int i: 	
ending index
prev: 
starting index
 ************************************************************************/
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

/************************************************************************
Function: Parse()
Author: Zacharious Owen
Description: Given input, will determine if it is of the form
w <boxnum> , r <boxnum>, q, or other 
Parameters:
in:
input string to read from
read_size:
it seems really inefficent to just throw this arround
client_sock:
file descriptor for this threads socket
 *WM:
 pointer to Writer mutex array
 *RM:
 pointer to REader mutex array
 ************************************************************************/
int Parse( char * in, int * read_size, int boxnum, int client_sock , pthread_mutex_t * WM , pthread_mutex_t * RM )
{
	int token;
	char * mytokens[30];
	char sorry[] = "Unknown command, Usage:\n\tw <mailboxnum>\n\t<Message>\nor\n\tr <mailboxnum>\nor\n\tq\n";

	token = Tokenize( in , mytokens, *read_size );

	if( token > 0 )
	{	

		switch( mytokens[0][0]) {	
			case 'w':
				ClientWrite( mytokens[1] , token , boxnum, WM, RM, client_sock);
				return 0;	
				break;
			case 'r':
				ClientRead( mytokens[1] , token,  boxnum, WM, RM, client_sock);
				return 0;
				break;	
			case 'q':
				return -1;
				break;
			default:
				write( client_sock, sorry, sizeof(sorry) );
				return 0;
				break;
		}
	}	
	return 0;
}

/************************************************************************
Function: setargs() 
Author: Zacharious Owen
Description: Helper function to translate the strings from argc to ints 
Also does error checking
Parameters:
boxnum:
number of boxes
boxsize:
size of boxes in Kilobytes
Port:
port to communicate on
pacsize:
Size in bytes to read from string
argv:
array of array from main(), should contain atleaset 4
things or this function will segfault
 ************************************************************************/
int setargs( int * boxnum, int * boxsize, int * port, int * pacsize, char ** argv )
{
	int ret = 0;

	*boxnum  = atof( argv[1] );
	*boxsize = atof( argv[2] );
	*port    = atof( argv[3] );
	*pacsize = atof( argv[4] );

	if( *boxnum <= 0 )
	{
		printf("Invalid Number of mailboxes value\n");
		ret = -1;
	}
	if( *boxsize <= 0 )
	{
		printf("Invalid mailbox size value\n");
		ret = -2;
	}

	if( *port <= 1024 && *port > 65536 )
	{
		printf("Invalid port number value\n");
		ret = -3;
	}

	if( *pacsize <= 0 )
	{
		printf("Invalid package size value\n");
		ret = -4;
	}

	return ret;
}


/************************************************************************
Function: SetupBind()
Author: Zacharious Owen
Description: Helper function to bind the program to a port
Parameters:
Port:
port to bind on
 ************************************************************************/
int SetupBind(int port )
{
	int socket_desc;
	struct sockaddr_in server;

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket\n");
		return -1;
	}
	puts("Socket created");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );

	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return -1;
	}
	puts("bind done");

	return socket_desc;

}

/************************************************************************
Function: Tokenize()
Author: Zacharious Owen
Description: Given a string of input, Tokenizes and returns based on 
seperating whitespace. Returns quanity of tokens

Parameters:
line:	
string to parse
`		tokens: Psudeo return
Contains parsed tokens
len:
length of line

 ************************************************************************/
int Tokenize( char * line , char * tokens[30], int len) 
{

	int i;
	int tokennum =0;
	int prev = 0;


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

	return tokennum;
}

