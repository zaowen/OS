#!/bin/python

#Python script to do the simple star programming problem
#*    *
# *  *
#  **
# *  *
#*    *


def col( x):
	if ( x == 1):
		print(" ." + str(x) + " DONE")
		return
	elif( x % 2 == 0):
		print("c." + str(x))
		col( int( x/2 ) )
	else:
		print(" ." + str(x))
		col( int( x*3 +1 ) )


def star( x ):
	dex1 = 0
	dex2 = x

	while( dex2 >= 0 ):
		index = 0
		
		while( index <= x ):
			if( index == dex1 or index == dex2):
				print("*", end="")
			else:
				print(" ", end="" )
			index = index + 1
		print("")

		dex1 = dex1 + 1
		dex2 = dex2 - 1
		if( dex1 - dex2 == 1):
			dex1 = dex1 + 1
			dex2 = dex2 - 1

star( 10 )
