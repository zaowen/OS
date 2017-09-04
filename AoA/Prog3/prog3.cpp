/*///////////////////////////////////////////////////////////////////
//           FFT based on code from CLRS Algorithms Text          //
Author: Zachary Owen
Program: O( n ) Fourier Transform
Class:  CSC 372
Instructor: Dr A. Logar

 Due: 1st December

Description: Using special maths, is able to create several FFT's usings
a single FFT as the base. 


///////////////////////////////////////////////////////////////////*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <iomanip>
#include <cstring>
using namespace std;

const int MAX = 8096;
const double EPSILON = 1.0e-12;
const complex<double> I (0, 1);

//Function Defs

double compabs( complex <double>a);
void outmake( char * in, char * derp);
void fft(complex <double> a[], int n, complex <double> y[] );
void twiddle ( complex <double> a[] , complex <double> y[], int n, int k, double f[] );
void twin_peaks( double q[] , int n , ostream & fout);

////////////////////////////////////////////////////////////////////
//                        FFT based on CLRS page 911              //
////////////////////////////////////////////////////////////////////


int main( int argc, char* argv[] )
{
  double   a_real[MAX];
  complex  <double> a[14096];
  complex  <double> q[MAX] = {0};
  int      i;
  ifstream inf;
  ofstream fout;
  int      n;
  int  k;
  complex  <double> y[MAX] = {0};
  double absarr[MAX];

  //Opens inputfile
  inf.open( argv[1] );

  //Creates and opens outputfile
  char out[50];
  outmake( argv[1] , out );
  fout.open( out );
  
  //Check for failures
  if (inf.fail())
    {
      cout << "Unable to open" << argv[1]  << endl;
      exit(1);
    }

  if ( fout.fail() )
    {
     cout << "Unable to open fout "  << endl;
      exit(1);
    }

  //get N and K
  inf >> n;
  inf >> k;

  //read coeff values
  for (i = 0; i < n+k; i++)
    {
      inf >> a_real[i];
      a[i] = a_real[i];
    }  
  // Do forward FFT
 
  fft( a, n, y);

  //Slide and calculate additional FFT's
  twiddle( a, y, n, k, absarr );

  //Average FFT's togethers
  for( int i = 0; i < n/2; i++)
    {
      absarr[i] = absarr[i] / k;
    }

  //Find Peaks
  twin_peaks( absarr , n , fout);

  //Close my files
  inf.close();
  fout.close();

  return 0;
}
 
/*///////////////////////////////////////////////////////////////////
FFT() : Preforms an fft on a[]. and places the returns in y[].
        Only outputs n/2 because it is all that is nessisary for our purposes.

Inputs:
       a[] : Array of reals, 
       n   : Ammount of reals

Psudo Returns
       y[] : Useds to return the values of the polynomial at the roots of unity
             contains frequency and amplidude data

Credit: Shamelessly stolen from 
http://www.mcs.sdsmt.edu/ecorwin/cs372/programs/
Altered slightly

///////////////////////////////////////////////////////////////////*/

void fft(complex <double> a[], int n, complex <double> y[] )
{
  complex  <double> even[MAX];
  complex  <double> even_fft[MAX];
  int      i;
  int      j;
  int      n2;
  complex  <double> odd[MAX];
  complex  <double> odd_fft[MAX];
  complex  <double> omega;
  complex  <double> omega_power;

  //Bassiscase
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
    
  fft(even, n2, even_fft);
  fft(odd, n2, odd_fft);

  omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);
  omega_power = 1;
  
  for (i = 0; i < n2; i ++)
    {
      y[i] = even_fft[i] + omega_power * odd_fft[i];
      y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
      omega_power = omega * omega_power;
    }
}

/*///////////////////////////////////////////////////////////////////
twiddle() : Using a 'sliding window' creates and combinds several FFT's using
            algebra to obtain Q(x) from P(x)
        

Inputs:
       a[] : An array of real coeffecents of an amalgamation of sine waves used as A sub 0
       y[] : An array of Fourier transformed data from A.
       n   : integer representing terms in a
       k   : integer represending the number of addional Transforms to add

Psudo Returns
       f[] : returns the 'f'inal values for our additional Transforms. MUST BE AVERAGED 
             Unlike y[] and a[] this contains the absolute distance not complex



Credit: Shamelessly stolen from 
http://www.mcs.sdsmt.edu/ecorwin/cs372/programs/
Altered slightly

///////////////////////////////////////////////////////////////////*/

void twiddle ( complex <double> a[] , complex <double> y[], int n, int k, double f[] )
{
  //storage array for totals since y[] changes 
  complex <double>q[MAX] = {0};
  //root of unity
  complex <double>omega = cos(2.0 * M_PI / n) + I * sin(2.0 * M_PI / n) ;
  //array to store precalculated roots
  complex <double>omegarr[n/2];

  //first root of unity
omegarr[0] = 1;

//calculate roots so we dont have to do it k-1 times
 for( int i = 1; i < n/2 ; i++)
   {
     omegarr[i] = omega * omegarr[i-1];
   }
 
 //storage for a_0 and new_y[i]
 complex <double> azero = a[0];
 complex <double> newy = a[n];
 
 for( int i = 0; i < n/2; i++)
   {
     q[i] = (y[i] - azero + newy )/omegarr[i];
   }
 //pointer hack to move the window
 a = a + 1;

 //add to our final array
 for ( int i = 0; i < n; i++ )
   {
     f[i] += compabs( q[i]) ;
   }  
 

 //we dont want to destroy y[] since we need it later and
 //copying is slow so we do a single calculation then
 //values can be derived from our q[]'s ( the windowed values)
 for( int m = 1; m < k; m++)
  {
    //cluttering my namespace and exploiting scoping rules
    complex <double> azero = a[0];
    complex <double> newy = a[n];
  
    for( int i = 0; i < n/2; i++)
      {
	q[i] = (q[i] - azero + newy )/omegarr[i];
      }

    a = a + 1;
      
    for ( int i = 0; i < n; i++ )
      {
	f[i] += compabs( q[i]) ;
      }  
  }
 
return;
}

/*
helper function to calculate absolute distance for complex values 
 */
double compabs( complex <double>a)
{
  return sqrt ( a.real() * a.real() + a.imag() * a.imag() ) ;
}

/*///////////////////////////////////////////////////////////////////
twin_peaks() : Finds highpoints in the array, saves and Nukes them so
               They dont come back round here again. and Dont effect 
	       future attempts to find peaks.

Inputs:
       q[] : Array of Fourrior Transformed data
       n   : integer representing terms in a

Psudo Returns
       fout: Stream used to out put data ( helpful in beta stages and you dont have to change anything)



///////////////////////////////////////////////////////////////////*/

void twin_peaks( double q[] , int n , ostream & fout)
{
  double max = 0.0;
  int dex = 0;
  
  for ( int i = 0; i < 5; i++)
    {
      max = 0.0;
      dex = 0;
      
      int j;
      for( j = 0; j < n; j++)
	{
	  if( q[j] > max)
	    {
	      dex = j;
	      max = q[j];
	    }
	}
      //add our hill together
      q[dex] += q[dex-2]+q[dex-1]+q[dex+1]+q[dex+2];
      
      fout << dex << " " << fixed << setprecision(2) << q[dex] << endl;
	
	for( int k = dex-2 ; k < dex+2; k++)
	  {
	    q[k] = 0.0;
	  }
    }  
}


/*
Helper function to make our .out from the .in because i hate <string> for no reason.
 */
void outmake( char * in, char * derp)
{
  int i = 0;
  strcpy( derp, in);
  
  while( in[i] != '.'){i++;};

  derp[i+1] = 'o';
  derp[i+2] = 'u';
  derp[i+3] = 't';
  derp[i+4] = '\0';  
}

////////////////////////////////////////////////////////////////////
