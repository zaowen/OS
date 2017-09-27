#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string.h>
#include <list>

using namespace std;

struct points{
  long double x;
  long double y;
}; 

points* get_points(ifstream &fin, int count, int & min, int & max);
long double area(points* p, int n);
bool is_convex( points* lst, int cnt);
char* in_to_out( char* arg );
long double direction(points a, points b, points c);
long double cross(points a, points b);
long double bounded(points * p ,int cnt,  int n, long double b);
void midpoint( points & a, list<points>::iterator b, list<points>::iterator c, long double x);
long double bisect( points * lst, const int cnt, int ldex, int hdex, const long double want);
long double vecarea( list<points> mylist );
long double bounded(points * p , int cnt , int n ,long double m);


int main(int argc, char* argv[] )
{
  ifstream fin;
  ofstream fout;

  //check correct number of args
  if(argc == 2)
    { 
      //open our files
      fin.open(argv[1]);
      fout.open( in_to_out(argv[1]) );

      //Hey look a use for demorgan's law
      if( !(fout || fin) )
	{
	  cout << "Could not create in/out streams\nCheck permissions and input file names\n";
	  return 0;
	}
      else
	{
	  //declare some vars
	  //counter for cases
	  int field = 0;
	  //number of points
	  int count;
	  //index of min and max points
	  int  min, max;
	  //while we can get input
	  while(fin >> count) 
	    {
	      //inc counter
	      field += 1;
	      //get our points
	      points * lst = get_points(fin ,count , min, max);
	      //print case number
	      fout << "Case " << field << ": ";
	      //check convexity
	      if( !is_convex(lst,count))
		{
		  fout << "No solution\n";
		}
	      else
		{
		  //find bisection
		  long double answer = bisect(lst,count, min,max, area(lst,count));
		  //setting output feilds 
		  fout << std::fixed;
		  fout << std::setprecision(5);
		  fout << answer;
		}
	      //no memory leaks here, no sir.
	      delete lst;
	      fout << "\n";
	    }
	}
    }
  else
    {
      cout << "Improper usage:\nTry: ./prog input.in\n";
      return 0;
    }
  //close our buffers
  fout.close();
  fin.close();
  //cout << "Boom Boom Boom, that.\n";
  
  //#lol
  return 1337;
}

/*
points * get_points
ins:
     fin: stream we read our points from
     count:
     &min: used as a psudo return value, holds index of min x val
     &max: used as a psudo return value, holds index of max x val
outs
     Dynamic array containing points from file
 */
points* get_points(ifstream &fin, int count, int & min, int & max)
{ 
  //dynamic points list
  points * lista = new (nothrow) points[count];
  long double temp = 0;
  //make sure dynamic works
  if( lista == nullptr)
    {
      //probably should just get out
      exit(1);
      return nullptr;
    }
  //get first point
  fin >> lista[0].x; 
  fin >> lista[0].y;
  //set indexes to a base value
  max = 0;
  min = 0;

  for(int i = 1; i < count; i++)
    {
      //FEED ME VALUES SEYMOUR
      fin >> lista[i].x;
      fin >> lista[i].y;
      
      //make sure we have max and min vals
      if(lista[i].x > lista[max].x)
	{
	  max = i;
	}
      if(lista[i].x < lista[min].x)
	{
	  min = i;
	}
    }
  //return our list
  return lista;
}

/*
shamelessly stolen from Geometry.C 
except i added  the result /4 inorder to get half area
so i think it falls under transformitive work amirite?
http://www.mcs.sdsmt.edu/ecorwin/cs372/programs/geometry.C
*/
long double area(points * p , int n)
{
  int i;
  int j;
  long double result;

  result = 0;
  for (i = 0; i < n; i++)
    {
      j = (i + 1) % n;
      result += p[i].x * p[j].y;
      result -= p[i].y * p[j].x;
    }
  return fabs(result / 4);
  
}

/*shamelessly stolen from Geometry.C 
http://www.mcs.sdsmt.edu/ecorwin/cs372/programs/geometry.C
*/
bool is_convex( points* lst, int cnt)
{

  if( cnt == 3 && ( direction( lst[1], lst[1+1], lst[1+2]) != 0.0  ) )
    {
      return true;
    }
  if (direction( lst[1], lst[1+1], lst[1+2]) < 0)
    {
      for( int i = 0; i+2 < cnt; i++)
	{ 	  
	  if ( direction( lst[i], lst[i+1], lst[i+2]) > 0)
	    return false;
	}  
    }
  else
    { 
      for( int i = 1; i+2 < cnt; i++)
	{ 	  
	  if ( direction( lst[i], lst[i+1], lst[i+2]) < 0)
	    return false;
	}  
    }
  return true;
}

/*shamelessly stolen from Geometry.C 
http://www.mcs.sdsmt.edu/ecorwin/cs372/programs/geometry.C
*/
long double direction(points a, points b, points c)
{
  points ab;
  points bc;
  long double result;

  ab.x = b.x - a.x;
  ab.y = b.y - a.y;
  bc.x = c.x - b.x;
  bc.y = c.y - b.y;
  result =  cross(ab, bc);
  if (fabs(result) < 1.0e-6)
    result = 0.0;
  return result;
}

//helper funtion to create file.out from file.in
char* in_to_out( char* arg )
{
  static char cat[50];
  char outra[] = "out";
  
  strcpy(cat,arg);
  strcpy( &cat[strlen(arg)-2],outra);
    
  
  return cat;
}

/*shamelessly stolen from Geometry.C */
long double cross(points a, points b)
{
  return a.x * b.y - a.y * b.x;
}

/*
void midpoint
I hear this function is called "interpalation"
ins:
     a: psudoreturn value where we store the midpoint
     b: iterator to one of the points in the list that makes the line we are interpalating
     c: iterator to one of the points in the list that makes the line we are interpalating 
     x: value at which we are interpolating
outs
     none void function
*/
void midpoint( points & a, list<points>::iterator b, list<points>::iterator c, long double x)
{
  a.x = x; 
  long double rise = ( b->y - c->y);
  long double run =  (b->x - c->x);
  long double slope = rise/run;
  long double intercept = c->y - slope * c->x;
  a.y = slope * x + intercept ;

  return;

}
 
/*
long double bisect
"recursivly" cuts the shape by adjusting high and low points all binaryily like.

ins:
     lst: list of our points
     cnt: count of the ammount of points we have in this case
     ldex: index of the lowest x valued point
     hdex: index of the highest x valued point
     want: Half of the area of the total polygon
     joke: what do you call a pet shop that had its parrots stolen?
outs
     long double: x value that cuts the polygon in half
     punchline: a Poly-gone
*/
long double bisect( points * lst, const int cnt, int ldex, int hdex, const long double want)
{
  //holds x value we are trying for half
  long double mid;
  //diffrence between desired area and tested half value
  long double delta;

  //values for highest and lowest x we are testing
  long double high = lst[hdex].x;
  long double low = lst[ldex].x;
 

  //cout << "Highest point is " << high << ".\nSmallest is " << low << endl;
  do
    {  
      //claculate midpoint between high and low
      mid = ( low + high )/2.0;
      //calculate area chopped at mid value and subtracs it from want
      long double barea = bounded( lst, cnt, ldex , mid);
      delta = (barea - want);
   
      //If delta is too big then out mid was 
      if(delta > 0)
	{
	  //cout << "too far, adjusting highpoint\n";
	  high = mid;
	}
      else
	{
	  //cout << "not far enough, adjusting lowpoint\n";
	  low = mid;
	}
      delta = abs(delta);
    }while( delta > 1.0e-6);

  //cout << "Mid pint found " << mid << endl;
  return mid;
};

/*
long double vecarea
Uses a list structrue and iterates through as if it contained a polygon very similar to our area funtion. -See area() for details
Notes: called VecArea because i originally used vectors.

ins:
    Mylist: STL list of points 
outs
    Long Double: Area of the particular passed pollgon
*/
long double vecarea( list<points> mylist )
{
  long double result = 0;
  //next point iterator
  list<points>::iterator jt;
  //iterate through list
  for( list<points>::iterator it = mylist.begin(); it != mylist.end(); it++)
    {
      //get next element
      jt = it;
      ++jt;
      //make sure we didnt step over
      if( jt == mylist.end() )
	jt = mylist.begin();
      //ITS A TRAP-EZOID
      result += it->x * jt->y;
      result -= it->y * jt->x;
    }
  return fabs(result/2);  


}

/*
long double Bounded
iterates through the points and finds the ones on the left side and the segements that cross and puts the midpoints into there. You may be wondering why we pass in the points as P[] and use them as a list<points>.

ins:
     p: list of points
     cnt: number of points in p
     n: index of lowest x point
     m: x value we are chopping on
outs
     long double: outputs area of the divided polygon
*/

long double bounded(points * p , int cnt , int n ,long double m)
{
  //temp point we push into list
  points temp;
  int j = n;
  int k = (j + 1) % cnt;
  bool leftside = true;
  list<points> mylist;

  //push all points into my list in order.
  for( int i = 0; i < cnt+1; i++)
    {
      j  = ( i + n ) % cnt;
      mylist.push_back( p[j] );
    }

  //used for calculating midpoints
  list<points>::iterator prev;
  //iterators used to delete points
  list<points>::iterator p1, p2;

  for( list<points>::iterator it = mylist.begin(); it != mylist.end(); it++)
    {
 //if on right side find midpoint 
      // if( leftside && it->x > m ) 
      // 	{
      // 	  prev = it;
      // 	  --prev;
      // 	  midpoint( temp, it , prev, m);
      // 	  mylist.insert( it, temp);
      // 	  p1 = it;
      // 	  leftside = false;
      // 	}
      // else if(!leftside &&  it->x < m ) 
      // 	{
      // 	  prev = it;
      // 	  --prev;
      // 	  midpoint( temp, it , prev, m);
      // 	  mylist.insert( it, temp);
      // 	  p2 = it;
      // 	  --p2;
      // 	  break;
      // 	}
 

      //if on right side find midpoint 
      if( it->x > m ) 
      	{
      	  prev = it;
      	  --prev;
      	  midpoint( temp, it , prev, m);
      	  mylist.insert( it, temp);
      	  p1 = it;
      	  //yo dawg i heard you liek forloops:
      	  //could have done this loop with a leftside bool
      	  for( ; it != mylist.end(); it++)
      	    {
      	      if( it->x < m ) 
      		{
      		  prev = it;
      		  --prev;
      		  midpoint( temp, it , prev, m);
      		  mylist.insert( it, temp);
      		  p2 = it;
      		  --p2;
      		  break;
      		}
      	    }
      	  break;
      	  it--;
      	}
    }
  // there was a weird problem with segfaults if i didnt have a pad at the end
  mylist.pop_back();
  //rid self of other side points
  mylist.erase(p1, p2);
  //calculate and return area
  return vecarea( mylist );
}
