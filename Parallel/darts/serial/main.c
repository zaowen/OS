#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX_FEDELITY 1000000

int main( int argc, char * argv)
{	
	int darts,hits, i, x, y;	
	double dx,dy;
	time_t t;

	printf("Enter number of points to sample\n");
	scanf( "%d" , &darts );

	printf("Throwing\n");

	srand( (unsigned) time(&t));
		
	hits = 0;

	for(i = 0; i < darts; i++)
	{
		x = rand();
		y = rand();
		dx = (double)x / RAND_MAX;	
		dy = (double)y / RAND_MAX;	

		//printf("(x,y) =  (%e,%e)\n", dx , dy);
		if(  dy*dy + dx*dx < 1 )
		{
			//printf("HIT at (%e,%e)\n", dx , dy);
			hits++;
		}
	}

	printf( "Your approximation is %e." , (double)(hits * 4) / darts );
}
