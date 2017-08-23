from Tkinter import *
from ttk import *
import random 




def RoundRobin(burst, queueLength, RRtime):
	chart = Tk()
	chart.title("Round Robin Gantt Chart")
	chart.geometry("1000x300")
	
	myburst = list(burst)

	mylist = []	
	realLen = 0
	temp = 0
	totalTime = 0
	for i in myburst:
		if(  i.get() != "" ):
			realLen = realLen + 1
			totalTime = int(i.get()) + totalTime	
			mylist.append( ( temp, int(i.get())  ))
		temp = temp + 1

	quanta = int(RRtime.get())
	final = []
	
	while ( mylist ):
		for i in mylist:
			if( i[1] > quanta ):
				final.append( (quanta , i[0] ))
				index = mylist.index( i ) 
				mylist[index] = ( i[0], i[1] - quanta )
			elif(i != -1):
				final.append( (i[1] , i[0] ) )
				index = mylist.index(i )
				mylist[index] = -1
		mylist = [x for x in mylist if x != -1]

	canvas = Canvas(chart)
	canvas.create_rectangle( 50 , 50 , 950, 200, fill="light blue")
	drawgant( canvas, final, totalTime, len(final), 50, 50,900, 200)

	canvas.pack( fill=BOTH, expand=1)

	
	return
	
def Priority(burst, priority, queueLength):
	chart = Tk()
	chart.title("Priority Gantt Chart")
	chart.geometry("1000x300")

	myburst = list (burst)	
	myprior	= list (priority)
	
	mylist = zip( burst, priority)

	mylist2 = []
	temp = 0
	realLen = 0
	totalTime = 0;

	for i in mylist:
		if( i[0].get() != "" ):
			realLen = realLen + 1
			totalTime = int(i[0].get()) + totalTime	
			mylist2.append( ( int(i[0].get()),temp , int(i[1].get()) ) )
		temp = temp + 1
	mylist2.sort()
	mylist = sorted( mylist2 , key = lambda x: int(x[2]) )
	
	canvas = Canvas(chart)
	canvas.create_rectangle( 50 , 50 , 950, 200, fill="light blue")
	drawgant( canvas, mylist, totalTime, realLen, 50, 50,900, 200)

	canvas.pack( fill=BOTH, expand=1)


	return
	
def ShortestFirst(burst, queueLength):
	chart = Tk()
	chart.title("Shortest First Gantt Chart")
	chart.geometry("1000x300")
	
	newburst = list(burst)
	mylist = []
	realLen = 0
	totalTime = 0
	for i in newburst:
		if(  i.get() != "" ):
			realLen = realLen + 1
			totalTime = int(i.get()) + totalTime	
			temp = newburst.index(i)
			mylist.append( (int(i.get()),temp ) ) 		
			newburst[temp] = 0

	canvas = Canvas(chart)
	canvas.create_rectangle( 50 , 50 , 950 , 200, fill="light blue" )

	mylist.sort()
	drawgant( canvas, mylist,totalTime, realLen, 50, 50, 900, 200 )	


	canvas.pack(fill=BOTH, expand=1)

	return

def drawgant( mycanvas, drawlist, totalTime, listlen, sx, sy, width, height ):
	R= width - 2 * listlen - 2
	timepast = 0
	startx = sx+2
	for i in drawlist:
		p = float(i[0])/totalTime
		timepast = i[0] + timepast
		thiswidth = p*R
		mycanvas.create_rectangle( startx , sy+2 , thiswidth+ startx, height-2 , fill = 'red' )

		if(thiswidth < 30 ):
			mycanvas.create_text( startx + thiswidth/2 , sy - 10,  font=("Calibri", 12), text="P"+str(i[1]) )
		else:
			mycanvas.create_text( startx + thiswidth/2 , sy+height/3,  font=("Calibri", 12), text="P"+str(i[1]) )

		mycanvas.create_text( startx+ thiswidth, sy + height*.75 + 10, font=("Calibri", 7 ) , text = int( timepast ) ) 

		startx = startx + thiswidth + 2
	return	
