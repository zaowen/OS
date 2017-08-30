#include "request.h"

void make_reqs( int * req ){
   int i;
   srand( time( NULL ) );

   for( i = 0; i < REQUESTS; i++ ){
      req[i] = rand() % DISK_MAX;
   }

#ifdef DEBUG
   printf( "Created request list:\n{ " );
   for( i = 0; i < REQUESTS-1; i++)
      printf( "%d, ", req[i] );

   printf( "%d}\n", req[i] );

#endif
   return;
}

