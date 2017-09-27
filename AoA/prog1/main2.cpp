#include <iostream>
#include <fstream>
#include <string.h>
#include <unordered_map>

using namespace std;

const int MAX = 6000;

static int table[MAX][MAX];
std::unordered_map< int,bool > hashtable;

bool checkstr( char* str1, char * str2, int len)
{
  for( int i = 0; i < len; i++)
    {
      if(str1[i] != str2[i])
	{
	  return false;
	}
    }
  return true;
}


bool dubscheckem( char* str , int start, int end)
{
  //base cases if in table 
  // if( table[start][end] != -1)
  //   return table[start][end];

  hashtable.find( end | (start << 16) );
 
  int len = (end - start)/2;
  int vcr = 0;
  
  if( (end-start) & 0x1 == true  )
    {
      vcr = 0;
    }
  else 
    if( strncmpr( &str[start], &str[start+len], len) == 0 )  //checkstr( &str[start], &str[start+len], len) ) 
      {
	vcr = 1;
      }
  else
    {
      //begin recursivly checking stuff
      for( int i = 2; start+i < end; i++)
	{
	  if( dubscheckem( str, start, start+i) && dubscheckem( str, start+i, end) )
	    {
	      vcr = 1;
	      break;
	      }
	}
    }
  
 
  table[start][end] = vcr;
  return vcr;
}

void hash_clr()
{
  hashtable.clear();
}

void table_clr( int n)
{
  if (n > MAX) 
    {
    n = MAX;
    }
  
  for(int i = 0; i < n; i++)
    for(int j = i; j < n; j++)
      {
	if( (j ^ i ) & 0x1 )
	  {
	    table [i][j] = 0; 
	  }
	  else
	    table[i][j] = -1;
      }
}

int get_case( fstream & fin, char* str)
{
  string temp;
  fin >> temp;
  strcpy( str, temp.c_str());
  return temp.length();

}

int main( int argc, char* argv[] )
{

  fstream fin; 
  ofstream fout;
  int len, testcase = 1;
  bool win;
  char str[MAX];
  fin.open("pairs.in");

  fout.open("pairs.out");
  
  if(fin)
    {
      len = get_case(fin, str);
      while(fin)
	{
	  //table_clr(len+1);
	  hash_clr();

	  if (fout)
	    {
	      if(  dubscheckem( str, 0, len) )
		{
		  fout << testcase << " yes\n";
		}
	      else
		{
		  fout << testcase << " no\n";
		}
	    }
	  else
	    {
	      cout << "File Output Error\n";
	      exit(1);
	    }
	  
	  len = get_case(fin, str);
	  testcase++;
	}
    }
  else
    {
      cout << "Input file error\n";
	exit(1);
    }
  fin.close();
  fout.close();

}
