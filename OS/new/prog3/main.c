/************************************************************************
Program: 
Author: Zachary Owen
Class:
Instructor:
Date:
Description: 
Input:
Output:
Compilation instructions:
Usage:
Known bugs/missing features:
Modifications:
 ************************************************************************/

#include <stdio.h> 
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "request.h"

/*********************** function prototypes ***************************/

void FCFS( int address ,int * req);
void SSTF(int address ,int * req);
void SCAN(int address ,int * req);
void CSCAN( int address ,int * req);
void LOOK( int address ,int * req);
void CLOOK( int address ,int * req);
void bucket_sort( int * arr );
void find_median( int * arr, int med, int * low, int * high );

#ifdef DEBUG
int _OTOE = 1;
#endif

/************************************************************************
Function:
Author:
Description:
Parameters:
 ************************************************************************/
int main( int argc, char * argv[] )
{
   char * str, * endptr;
   int address;
   int req[REQUESTS];

   if( argc < 2 )
   {
      printf( "Usage: %s <start position>\n\tExiting\n", argv[0]  );
      exit(EXIT_FAILURE);
   }

   str = argv[1];

   address = strtol( argv[1] , &endptr, 10 );

   if(( errno == ERANGE && ( address == INT_MAX || address == INT_MIN))
         || (errno !=0 && address ==0)) {
      perror("strtol");
      exit(EXIT_FAILURE);

   }

   if( endptr == str ) {
      printf( "No Digits found\n" );
      printf( "Usage %s <start position>\n\tExiting.\n", argv[0] );
      exit(EXIT_FAILURE);
   }

   if( address < 0 || address > DISK_MAX ){
      printf( "Start Position must be between 0 and %d\n", DISK_MAX);
      exit( EXIT_FAILURE);
   }

   make_reqs( req );

   FCFS(address , req);
   SSTF(address , req);
   SCAN(address , req);
   CSCAN(address , req);
   LOOK(address , req);
   CLOOK(address , req);


   return -1;
}


void FCFS( int address ,int * req){
   int i, sum;

#ifdef DEBUG
   printf( "\nFirst Gome, First Serve:\n " );
#endif
   sum = 0;
   for( i = 0; i < REQUESTS; i++ ){
#ifdef DEBUG
      printf( "Moving head from %d to %d", address, req[i] );
#endif
      sum += abs( address - req[i] );
      address = req[i];
#ifdef DEBUG
      printf( " current seek time: %d\n", sum );
#endif
   }
   printf( "First Come, First Serve took %d time units\n", sum);
}

void SSTF(int address ,int * req){
   int i , j, sum;
   int s, sdex;
   int arr[ REQUESTS];

#ifdef DEBUG
   printf( "\nShortest Seek Time First:\n copy request array\n" );
#endif
   memcpy( arr, req, REQUESTS * sizeof(int ) );

   sum = 0;
   for( i = 0; i < REQUESTS; i++ ){
      s = DISK_MAX;
      for( j = 0; j < REQUESTS; j++ ){
         if( arr[j] != -1  && abs( address - arr[j] ) < s ){
#ifdef DEBUG
            printf( "Found new smallest seek index %d, distance %d\n", j, abs( address - arr[j])  );
#endif
            s = abs( address - arr[j] ) ;
            sdex = j;
         }
      }
      sum += abs( address - arr[sdex] );
#ifdef DEBUG
      printf( "Seeking to index %d, which is %d time units away. \n",sdex,  abs( address - arr[sdex] ) );
      printf( "Total seek time so far %d.\n" , sum );
#endif
      address = arr[sdex];
      arr[sdex] = -1;
   }


   printf( "Shortest Seek Time First, took %d time units\n", sum);

   return;
}

void SCAN(int address ,int * req){
   int i, low, high;
   int arr[ REQUESTS];

#ifdef DEBUG
   printf( "\nSCAN:\n copy request array\n" );
#endif
   memcpy( arr, req, REQUESTS * sizeof(int ) );

   bucket_sort( arr );

   find_median( arr, address, &low, &high);

   if( low == -1 ){
      printf( "SCANing, right took %d time units\n", arr[ REQUESTS - 1 ] - address );
      printf( "SCANing, left  took %d time units\n", arr[ REQUESTS - 1 ] + address);
   }
   else if( high == -1 ){
      printf( "LOOKing, right took %d time units\n", DISK_MAX * 2 - address -arr[0]  );
      printf( "SCANing, left  took %d time units\n",  address - arr[0]);
   }
   else{
      printf( "SCANing, right took %d time units\n", DISK_MAX - address + DISK_MAX - arr[0]);
      printf( "SCANing, left  took %d time units\n", address + arr[ REQUESTS - 1]);
   }

   return;
}

void CSCAN( int address ,int * req){

   int i, low, high;
   int arr[ REQUESTS];

#ifdef DEBUG
   printf( "\nC-SCAN:\n copy request array\n" );
#endif
   memcpy( arr, req, REQUESTS * sizeof(int ) );

   bucket_sort( arr );

   find_median( arr, address, &low, &high);

   if( low == -1 ){
      printf( "C-SCANing, right took %d time units\n", arr[ REQUESTS - 1 ] - address );
      printf( "C-SCANing, left  took %d time units\n", arr[ REQUESTS - 1 ]  - arr[ 0 ] + address);
   }
   else if( high == -1 ){
      printf( "C-SCANing, right took %d time units\n", DISK_MAX - address + arr[ REQUESTS - 1]);
      printf( "C-SCANing, left  took %d time units\n", arr[0] - address );
   }
   else {
      printf( "C-SCANing, right took %d time units\n", DISK_MAX - address + arr[low]);
      printf( "C-SCANing, left  took %d time units\n", address + ( DISK_MAX - arr[ high ]));
   }
   return;
}

void LOOK( int address ,int * req){

   int i, low, high;
   int arr[ REQUESTS];

#ifdef DEBUG
   printf( "\nLOOK\n copy request array\n" );
#endif
   memcpy( arr, req, REQUESTS * sizeof(int ) );

   bucket_sort( arr );

   find_median( arr, address, &low, &high);

   if( low == -1 ){
      printf( "LOOKing, right took %d time units\n", arr[ REQUESTS -1 ] - address );
      printf( "LOOKing, left  took %d time units\n", arr[ REQUESTS -1 ] - address );
   }
   else if( high == -1 ){
      printf( "LOOKing, right took %d time units\n", address - arr[0] );
      printf( "LOOKing, left  took %d time units\n", address - arr[0] );
   }
   else {
      printf( "LOOKing, right took %d time units\n", arr[REQUESTS] + DISK_MAX   - address - arr[0]  );
      printf( "LOOKing, left  took %d time units\n", address - arr[0] * 2 + arr[REQUESTS-1] );
   }

   return;
}

void CLOOK( int address ,int * req){

   int i, low, high;
   int arr[ REQUESTS];

#ifdef DEBUG
   printf( "\nC-LOOK\n copy request array\n" );
#endif
   memcpy( arr, req, REQUESTS * sizeof(int ) );

   bucket_sort( arr );

   find_median( arr, address, &low, &high);

   if( low == -1 ){
      printf( "C-LOOKing, right took %d time units\n", arr[ REQUESTS -1 ] - address );
      printf( "C-LOOKing, left  took %d time units\n", address + DISK_MAX - arr[0] );
   }
   else if( high == -1 ){
      printf( "C-LOOKing, right took %d time units\n", DISK_MAX - address + arr[ REQUESTS - 1 ] );
      printf( "C-LOOKing, left  took %d time units\n", address - arr[0] );
   }
   else {
      printf( "C-LOOKing, right took %d time units\n", DISK_MAX - arr[high] + arr[low] );
      printf( "C-LOOKing, left  took %d time units\n",  address + DISK_MAX - arr[high]);
   }

   return;
}

void bucket_sort( int * arr ){
   int i;
   int n;
   int bucket[DISK_MAX] = {0};

   for( i = 0; i < REQUESTS; i++){
      bucket[ arr[i] ]++;
   }

   n = 0;

   for( i = 0; i < DISK_MAX; i++){
      if( bucket[i]-- != 0){
         arr[n] = i;
         i--;
         n++;
      }
   }
#ifdef DEBUG
   printf( "Post bucket sort\n{ " );
   for( i = 0; i < REQUESTS-1; i++)
      printf( "%d, ", arr[i] );

   printf( "%d}\n", arr[i] );

#endif
   return;
}

void find_median( int * arr, int med, int * low, int * high ){

   int i;
#ifdef DEBUG
   if( _OTOE ){
      printf( "Start Median Find:\n" );
      printf( "Finding Address: %d\n" , med );
   }
#endif
   if( arr[0] >= med ){
      *low  =-1;
      *high = 0;
      return;
   }
   else if( arr[REQUESTS-1] <= med ){
      *low  = 0;
      *high =-1;
      return;
   }
   else{
      for( i = 0; i <= REQUESTS; i++ ){
#ifdef DEBUG
         if( _OTOE ){
            printf( "Looking Between %d and %d \n" , arr[i] , arr[i+1] );
         }
#endif
         if( med >= arr[i] && med <= arr[i+1] ){
#ifdef DEBUG 
            if( _OTOE ){
               printf( "Found starting head position: %d between sector request %d and %d\n" , med , arr[i], arr[i+1] );
               _OTOE--;
            }
#endif
            *low  = i;
            *high = i+1;
            break;
         }
      }
   }
   while( *low > 0 && arr[*low] == arr[*low-1] ){ *low--;}; 
   while( *high > DISK_MAX && arr[*high] == arr[*high+1] ){ *high++; };

   return;

}
