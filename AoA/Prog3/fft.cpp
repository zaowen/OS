////////////////////////////////////////////////////////////////////
//           FFT based on code from CLRS Algorithms Text          //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "complex.h"  // complex code from Dr. Weiss with additions


using namespace std;

const int MAX = 4096;

////////////////////////////////////////////////////////////////////
//                     print a complex polynomial                 //
////////////////////////////////////////////////////////////////////

void print_polynomial(complex a[], int n)
{
  int i;
  int dex;
  double temp, max = 0;
  for (i = 2; i < n/2 - 2; i++)
    {
      temp = a[i].abs();

      cout << "Checking value #" << i+1 << " : " << temp << endl;
	
      if ( temp > max ) 
	{
	  dex = i;
	  max = temp;
	}
    }
  cout << "Highest found is "  << max << " in spot " << dex+1; 
  cout << endl;
}

////////////////////////////////////////////////////////////////////
//                forward FFT based on CLRS page 835              //
////////////////////////////////////////////////////////////////////

void forward_fft(complex a[], int n, complex y[])
{
  complex  even[MAX];
  complex  even_fft[MAX];
  int      i;
  int      j;
  int      n2;
  complex  odd[MAX];
  complex  odd_fft[MAX];
  complex  omega;
  complex  omega_power;

  if (n == 1)
    {
      y[0] = a[0];
      return;
    }
  
  n2 = n / 2;
  j = 0;
  for (i = 0; i < n; i += 2)
    {
      even[j] = a[i];
      odd[j] = a[i + 1];
      j ++;
    }
    
  forward_fft(even, n2, even_fft);
  forward_fft(odd, n2, odd_fft);

  omega = complex(cos(2.0 * M_PI / n), sin(2.0 * M_PI / n));
  omega_power = 1;
  for (i = 0; i < n2; i ++)
    {
      omega_power = omega * omega_power;
      y[i] = even_fft[i] + omega_power * odd_fft[i];
      y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
    }
}

////////////////////////////////////////////////////////////////////

void twiddle ( complex a[] , complex y[], int n, int k,  complex f[])
{

  complex q[MAX] = {0};
  complex omega = complex(cos(2.0 * M_PI / n), sin(2.0 * M_PI / n));  
  complex omegarr[n];
  
  omegarr[0] = 1;
  
  for( int i = 1; i < n/2 ; i ++)
    {
      omegarr[i] = omega * omegarr[i-1];
      cout << omegarr[i] << endl;
    }
    
  for( int m = 0; m < k; m++)
    {
      complex azero = a[0];
      complex newy = a[n+m];

      cout << " A sub 0 in this iteration is :" << azero << endl;
      cout << " Y_new in this iteration is :" << newy << endl;

  
      for( int i = 0; i < n; i++)
	{
	  q[i] = (y[i] - azero + newy )/omegarr[i];

	}

      a = a + 1;
      
      for ( int i = 0; i < n; i++ )
	{
	  f[i] = f[i] + q[i];
	}
    }
  
  return;
}

////////////////////////////////////////////////////////////////////

void find_peaks( double a[], int n, ostream& fout, int num)
{
  double max = 1;
  for( int j = 0; j < n; j++)
    {
      if( max > a[j] )
	{}
    }
  
  for( int i = 0; i < num; i++)
    {
      for( int j = 0; j < n; j++)
	{
	  

	}
    }
  
}


////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
  complex  a[MAX];
  ifstream inf;
  int      n;
  int      k;
  complex  y[MAX] = {0};
  complex  q[MAX] = {0};
  double   temp;
  double   absarr[MAX];
  
  inf.open( argv[1]);
  
  if (inf.fail())
    {
      cout << "Unable to open"<< argv[1] << endl;
      exit(1);
    }

  inf >> n;
  inf >> k;
  cout << "n = " << n << endl;
  cout << "k = " << k << endl;


  for( int i = 0; i < n+k+100; i++)
    {
      inf >> temp;
      a[i] = complex( temp, 0);
    }
  
  cout << "READING FINISHED" << endl;
  
  // Do forward FFT

  forward_fft( a, n, y);

  cout << "FFT FINISHED" << endl;

  cout << "Final Y(): " << a[n] << endl; 
   
  twiddle( a, y, n, k-1, q);

  //print_polynomial( q , n ); 
  
  for( int i = 0; i < n/2; i++)
    {
      absarr[i] = q[i].abs() / k;
      cout << "Checking value #" << i+1 << " : " << absarr[i] << endl;
    }
  
  //find_peaks( q , n , cout , 5 );


  inf.close();
  
}
l
////////////////////////////////////////////////////////////////////
