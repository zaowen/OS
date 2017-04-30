#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>


int main( int argc, char * argv[]){

	char *endptr, *str;
	int i;
	uint32_t address;

	if( argc < 2){
		printf( "Usage %s <address>\n\tExiting.\n", argv[0] );
		exit(EXIT_FAILURE);
	}

	str = argv[1];

	address = strtol( argv[1] , &endptr, 10 );

	if(( errno == ERANGE && ( address == LONG_MAX || address == LONG_MIN))
			|| (errno !=0 && address ==0)) {
		perror("strtol");
		exit(EXIT_FAILURE);

	}

	if( endptr == str ) {
		printf( "No Digits found\n" );
		printf( "Usage %s <address>\n\tExiting.\n", argv[0] );
		exit(EXIT_FAILURE);
	}
	
	printf( "The address %d contains:\n", address);

	printf( "page number = %d\n", address >> 12 );

	printf( "offset = %d\n", address & 0xFFF);

}
