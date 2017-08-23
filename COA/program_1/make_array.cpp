/*-----------------------------------------------------------------
* File:    make_array.cpp
*
* Purpose: Create a random list of n numbers 
*
* Compile: g++ -g -std=c++11 -Wall -o make_array make_array.cpp 
*   
* Run:     ./make_array <n>
*             n:   number of elements in list
*
-----------------------------------------------------------------*/
#include <time.h>
#include <new>
#include <iostream>
#include <fstream>

using namespace std;

/* User defined Functions */
/*-----------------------------------------------------------------
 * Function:  Permute_array
 * Purpose:   Use random number generator to create a permutation of 
 *            the array with a uniform distribution.
 * In args:   n
 * Out args:  c
 */
void* Permute_array(int *c, int n)
{
   int i, j, k;
   
   for(i=0; i<(n-1); i++)
   {
      j = i+(int)((n-i)*(rand()/(RAND_MAX+1.0)));
      k = c[i];
      c[i] = c[j];
      c[j] = k;
   }

   return NULL;
}  /* Permute_array */

int main(int argc, char *argv[])
{
   //code to write characters in a file
   int *c, i, n;

   // Make sure we a commandline argument for file-name
   if ( argc != 2 )
   {   
      /* We print argv[0] assuming it is the program name */
      printf( "usage: %s <n>\n", argv[0] );
      printf( "    n:   number of elements in list\n");
      return(0);
   }

   // Seed the randomizer
   srand(time(NULL));
   // Get number of elements
   n = strtol(argv[1], NULL, 10);

   // Allocate size for c
   c = new (nothrow) int[n];
   // Any error?
   if (c == nullptr)
   {
      cout << "Error: memory could not be allocated";
      exit(1);
   }
   // Initialize c
   for(i=0; i<n; i++)
      c[i]=i+1; 
   // Permute the array
   Permute_array(c,n);

   //Open an output stream
   ofstream test ("tape.bin",ios::out | ios::trunc | ios::binary );
   // Write the output data   
   if(test.is_open())
   {
       cout << "Writing to file" << endl;
       for(int i=0; i<n; i++)
       {
           test.write(reinterpret_cast<char*>(&c[i]), sizeof(int));
#  ifdef DEBUG
	   cout << c[i] << " ";		// Debug output
#  endif          
       }
       cout << endl;
   }
   else
      cout << "Can't open file" << endl;

   test.close();
   delete[] c;

   return 1;
}
