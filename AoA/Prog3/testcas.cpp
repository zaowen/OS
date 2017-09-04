#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

int main()
{
  ifstream fin;
  ofstream fout;

  //open in for testfile
  fin.open("owen.in");
  fout.open("owen.out");
  
  if( fin && fout)
    {
      int wavnum;
      int n;
      int k;

      fin >> wavnum;
      fin >> n;
      fin >> k;
      
      double * a = new double[wavnum];
      double * b = new double[wavnum];;
      double * c = new double[wavnum];;

      for(int i = 0; i < wavnum; i++)
	{
	  fin >> a[i];
	  fin >> b[i];
	  fin >> c[i];
	}

      fout << n;
      fout << k;
      
      double o;
      for( int i = 0; i < n+k+1; i++)
	{
	  o = 0;
	  for(int j = 0; j < wavnum; j++)
	    {
	      o += a[j] * sin(( i * b[j] ) + c[j] ); 
	    }
	  fout << fixed << setprecision(5);
	  fout << o << endl;
	}
	
    }
  else
    {
      cout << "Files fuckd" << endl;
    }
  fin.close();
  fout.close();
  
}
