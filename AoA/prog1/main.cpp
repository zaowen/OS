/*
  Zachary Owen 
  Analasis of Algorithms - Program 1
  Due: 2015 - 10 - 4

  Description: Dynamically looks for sequences of repeated pairs of any length. 

  Input

  The input consists of several test cases. Each test case is on a separate line, and it consists of a single string of from 1 to 5,000 characters. Repeat until end of file. There will be no spaces in the input file. You may assume every line is a string of lower-case letters followed by an end of line character.

  Output

  For each test case, display a line consisting of the case number (starting at 1), a space and either "yes" or "no" depending on whether or not that string is a sequence of pairs.

  Bugs: None that can be deliberately reproduced although on occasion User time on linux systems can spike as high as 4 seconds but will return to under 1 second if run again.
*/
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

const int MAX = 5000;

static signed char table[MAX][MAX];

/*This seemed to be slightly faster than strncmp in testing
  Input: 2 strings to compare and the length to check
  Output: True if equal
  False as soon as a discrepancy is reached
*/
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

/*Main Recursive Function
  Takes in string to check and our start and end indicies
  Output: True if string is composed of repeated doubles false if not

*/
bool dubscheckem( char* str , int start, int end)
{
  //base cases if in table 
  if( table[start][end] != -1)
    return table[start][end];

  int len = (end - start)/2;
  //always return your vhs 
  int vhs = 0;
  
  //checks for odd case
  if( (end-start) & 0x1 == true  )
    {
      vhs = 0;
    }
  else
    { //checks if strings match cut in half
      if( checkstr( &str[start], &str[start+len], len) ) 
	{
	  vhs = 1;
	}
      else
	{
	  //begin recursivly checking stuff
	  for( int i = 2; start+i < end; i += 2)
	    {
	      //calls on left and right substrings
	      if( dubscheckem( str, start, start+i) && dubscheckem( str, start+i, end) )
		{
		  vhs = 1;
		  break;

		}
	    }
	}      
    }
  
  //oh yeah put in in that table
  table[start][end] = vhs;
  return vhs;
}

/*
  Resets the table so it can be used again for the next case
*/
void table_clr( int n)
{
  //just in case
  if (n > MAX) 
    {
      n = MAX;
    }
  
  for(int i = 0; i < n; i++)
    for(int j = i; j < n; j++)
      {
	table[i][j] = -1;
      }
}

/*
  Helper function to get the next case from the file
  str will be replaced with a pointer to the string
  Returns the length of the string
*/
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
  //important
  char t[] = {0x64,0x61,0x6e,0x6b,0x6d,0x65,0x6d,0x65,0x73,0x0d,0x00};
 

  fin.open("pairs.in");

  fout.open("pairs.out");

  //just in case
  if(fin)
    {
      //This is only here so we can use fin for the while loop condition
      len = get_case(fin, str);
      while(fin)
	{
	  //clear before next iteration
	  table_clr(len+1);
	  //just in case
	  if (fout)
	    {
	      //runs our Memoized funtion and outputs
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
	  //get next case
	  len = get_case(fin, str);
	  //increase case counter for output
	  testcase++;
	}
    }
  else
    {
      cout << "Input file error\n";
      exit(1);
    }
  //close files
  fin.close();
  fout.close();
  
  printf("%s",t);
}
