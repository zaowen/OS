from Tkinter import *
from ttk import *
import random
import tabOne
import tabThree
	
def gui():
	#declare a TK window
	root = Tk()
	root.title("OS Simulation Program")
	root.geometry("1050x630")
	
	#set up the tab structure and define tabs
	notebook = Notebook(root)
	one = Frame(notebook) 
	two = Frame(notebook)
	three = Frame(notebook) 

	#add tabs to GUI
	notebook.add(one, text = "Process Scheduler")
	notebook.add(two, text = "Memory Management Unit")
	notebook.add(three, text = "Page Replacement")
	notebook.pack(side = TOP)
	random.seed()
	return root, one, two, three	

#########################################################################################################################
####################################    FUNCTIONS FOR TAB ONE: PROCESS SCHEDULER    #####################################
#########################################################################################################################
def genBurst():
	stringOne[0].set(random.randint(1, int(maxB.get())))
	
def genPriority():
	stringTwo[0].set(random.randint(1, int(maxP.get())))	
	
def genBoth():
	genBurst()
	genPriority()
	
def genAll():
	for i in range (M - 1, -1, -1):
		stringOne[i].set(random.randint(1, int(maxB.get())))
		stringTwo[i].set(random.randint(1, int(maxP.get())))
	
def processclearAll():
	for i in range (M - 1, -1, -1):
		stringOne[i].set('')
		stringTwo[i].set('')
	RRtime.set('10')
	maxB.set('100')
	maxP.set('5')
		
def pushList(var):
	for i in range(M - 1, 0, -1):
		var[i].set(var[i - 1].get())
	var[0].set('')
	
def pushBothLists():
	pushList(stringOne)
	pushList(stringTwo)
	
def chartRoundRobin():
	tabOne.RoundRobin(stringOne, M, RRtime)
	
def chartPriority():
	tabOne.Priority(stringOne, stringTwo, M) 
	
def chartShortestFirst():
	tabOne.ShortestFirst(stringOne, M)

def frameOne():
	##########################################################################
	#####################  STACK FRAMES VERTICALLY   #########################
	##########################################################################
	arrival = Frame(one)
	arrival.pack(side = TOP)
	burst = Frame(one)
	burst.pack(side = TOP)
	priority = Frame(one)
	priority.pack(side = TOP)
	setMax = Frame(one)
	setMax.pack(side = TOP)
	rand = Frame(one)
	rand.pack(side = TOP)
	push = Frame(one)
	push.pack(side = TOP)
	info = Frame(one)
	info.pack(side = TOP)
	cmds = Frame(one)
	cmds.pack(side = TOP)
	info2 = Frame(one)
	info2.pack(side = TOP)

	##########################################################################
	######################           ARRIVAL         #########################
	##########################################################################
	arrivalLabel = []
	arrivalEntry = []	
	
	Label(arrival, text = "Process Number\t\t\t").pack(side = LEFT)
	
	for i in range(M):
		arrivalLabel.append(Label(arrival, width = 8, text = str( i)))
		arrivalLabel[i].pack(side = LEFT)	
	
	##########################################################################
	#######################           BURST         ##########################
	##########################################################################
	stringOne = []
	entryOne = []	
	
	Label(burst, text = "Burst Time\t\t").pack(side = LEFT)
	
	for i in range(M):
		stringOne.append(StringVar())
		entryOne.append(Entry(burst, width = 8, textvariable = stringOne[i]))
		entryOne[i].pack(side = LEFT)
	
	##########################################################################
	#######################          PRIORITY        #########################
	##########################################################################
	stringTwo = []
	entryTwo = []
	
	Label(priority, text = "Priority\t\t\t").pack(side = LEFT)
	
	for i in range(M):
		stringTwo.append(StringVar())
		entryTwo.append(Entry(priority, width = 8, textvariable = stringTwo[i]))
		entryTwo[i].pack(side = LEFT)
	
	##########################################################################	
	######################           SET MAX         #########################
	##########################################################################
	Label(setMax, text = "Max Burst Time  ").pack(side = LEFT)

	maxBString = StringVar()
	maxBString.set(100)
	maxBEntry = Entry(setMax, width = 8, textvariable = maxBString)
	maxBEntry.pack(side = LEFT)
	
	Label(setMax, text = "   Max Priority  ").pack(side = LEFT)
	
	maxPString = StringVar()
	maxPString.set(5)
	maxPEntry = Entry(setMax, width = 8, textvariable = maxPString)
	maxPEntry.pack(side = LEFT)

	##########################################################################
	#######################            RAND          #########################
	##########################################################################
	Button(rand, width = 20, text = "Generate Burst Time", command = genBurst).pack(side = LEFT)		
	Button(rand, width = 15, text = "Generate Both", command = genBoth).pack(side = LEFT)	
	Button(rand, width = 20, text = "Generate Priority", command = genPriority).pack(side = LEFT)
	
	##########################################################################
	#######################            PUSH          #########################
	##########################################################################
	Button(push, width = 15, text = "Push Burst Time", command = lambda: pushList(stringOne)).pack(side = LEFT)		
	Button(push, width = 15, text = "Push Both", command = pushBothLists).pack(side = LEFT)		
	Button(push, width = 15, text = "Push Priority", command = lambda: pushList(stringTwo)).pack(side = LEFT)

	##########################################################################
	#######################            INFO          #########################
	##########################################################################
	Button(info, width = 15, text = "Generate All", command = genAll).pack(side = TOP)
	Button(info, width = 15, text = "Clear All", command = processclearAll).pack(side = TOP)
	Label(info, text = "\n\n\nPROCESS SCHEDULING ALGORITHMS\n").pack(side = TOP)
	Label(info, text = "Pick an algorithm to generate a Gantt Chart\n").pack(side=TOP)
	
	Label(info, text = "Round Robin Time").pack(side = TOP)
	RRtime = StringVar()
	RRtime.set(5)
	RRtimeEntry = Entry(info, width = 8, textvariable = RRtime)
	RRtimeEntry.pack(side = TOP)
		
	##########################################################################
	#######################            CMDS          #########################
	##########################################################################
	Button(cmds, width = 15, text = "Round Robin", command = chartRoundRobin).pack(side = LEFT)	
	Button(cmds, width = 15, text = "Priority", command = chartPriority).pack(side = LEFT)	
	Button(cmds, width = 15, text = "Shortest Job First", command = chartShortestFirst).pack(side = LEFT)
	
	##########################################################################
	#######################           INFO2          #########################
	##########################################################################
	
	Label(info2, text = "\nROUND ROBIN: The round robin algorithm starts with the first process to enter the queue and gives it CPU time for the interval \nspecified in the 'Round Robin Time' box, (Default 5) before moving on to the next process.\n").pack(side = TOP)
	Label(info2, text = "\nPRIORITY: The priority algorithm completes the processes with the highest priority (1 = highest priority) first. In the case of a tie, \nit defaults to the shortest job first algorithm.\n").pack(side = TOP)
	Label(info2, text = "\nSHORTEST JOB FIRST: The shortest job first algorithm completes the processes with the lowest burst time (1 = lowest burst time) \nfirst. In the case of a tie, it picks the first process to enter the queue.\n").pack(side = TOP)
		
	return stringOne, stringTwo, maxBString, maxPString, RRtime
	
#########################################################################################################################
##################################    END FUNCTIONS FOR TAB ONE: PROCESS SCHEDULER    ###################################
#########################################################################################################################





#########################################################################################################################
##################################      FUNCTIONS FOR TAB TWO: MEMORY MANAGEMENT      ###################################
#########################################################################################################################

def genAddress():
	address.set(random.randint(1, int(maxA.get())))
	
def genOffset():
	offset.set(random.randint(1, int(maxO.get())))
	
def genAddressAndOffset():
	genAddress()
	genOffset()

def frameTwo():
	##########################################################################
	#####################  STACK FRAMES VERTICALLY   #########################
	##########################################################################	
	add = Frame(two)
	add.pack(side = TOP)
	setSpecs = Frame(two)
	setSpecs.pack(side = TOP)
	gen = Frame(two)
	gen.pack(side = TOP)
	info = Frame(two)
	info.pack(side = TOP)
	cmds = Frame(two)
	cmds.pack(side = TOP)
	info2 = Frame(two)
	info2.pack(side = TOP)
	##########################################################################	
	######################          SET SPECS        #########################
	##########################################################################
	Label(add, width = 30, text = "Virtual Page Number").pack(side = LEFT)

	address = StringVar()
	address.set(3)
	addressEntry = Entry(add, width = 8, textvariable = address)
	addressEntry.pack(side = LEFT)
	
	Label(add, width = 30, text = "Physical Page Number").pack(side = LEFT)
	
	offset = StringVar()
	offset.set(5)
	offsetEntry = Entry(add, width = 8, textvariable = offset)
	offsetEntry.pack(side = LEFT)
	offsetEntry.configure(state = 'readonly')
	
	##########################################################################	
	######################          SET SPECS        #########################
	##########################################################################
	Label(setSpecs, width = 30, text = "Memory Size (Number of Frames)  ").pack(side = LEFT)

	numFrames = StringVar()
	numFrames.set(3)
	numFramesEntry = Entry(setSpecs, width = 8, textvariable = numFrames)
	numFramesEntry.pack(side = LEFT)
	
	Label(setSpecs, width = 30, text = "Address Size (Number of Pages)  ").pack(side = LEFT)
	
	numPages = StringVar()
	numPages.set(5)
	numPagesEntry = Entry(setSpecs, width = 8, textvariable = numPages)
	numPagesEntry.pack(side = LEFT)
	
	##########################################################################
	#######################            GEN           #########################
	##########################################################################	
	Button(gen, width = 20, text = "Generate Address", command = genAddress).pack(side = LEFT)	
	Button(gen, width = 20, text = "Generate Both", command = genAddressAndOffset).pack(side = LEFT)	
	Button(gen, width = 20, text = "Generate Offset", command = genOffset).pack(side = LEFT)	
	
	##########################################################################
	#######################           INFO           #########################
	##########################################################################
	Label(info, text = "\n\nMEMORY MANAGEMENT\n").pack(side = TOP)
	Label(info, text = "Click 'Run \n").pack(side=TOP)
		
	##########################################################################
	#######################            CMDS          #########################
	##########################################################################
	#Button(cmds, width = 10, text = "FIFO", command = faultsFIFO).pack(side = LEFT)	
	#Button(cmds, width = 10, text = "Optimal", command = faultsOptimal).pack(side = LEFT)	
	
	##########################################################################
	#######################           INFO2          #########################
	##########################################################################
	
	Label(info2, text = "\nMEMORY MANAGEMENT: \n").pack(side = TOP)

	return address, offset, numFrames, numPages
	
#########################################################################################################################
##################################    END FUNCTIONS FOR TAB TWO: MEMORY MANAGEMENT    ###################################
#########################################################################################################################




#########################################################################################################################
##################################      FUNCTIONS FOR TAB THREE: PAGE REPLACEMENT     ###################################
#########################################################################################################################

def genReferenceString():
	for i in range(M - 1, -1, -1):
		stringReference[i].set(random.randint(1, int(numPages.get())))	
		
def clearAll():
	for i in range(M - 1, -1, -1):
		stringReference[i].set('')
	numFrames.set('3')
	numPages.set('5')

def faultsFIFO():
	tabThree.FIFO(stringReference, int(numFrames.get()), numPages)
def faultsOptimal():
	tabThree.Optimal(stringReference, int(numFrames.get()), numPages)
def faultsLRU():
	tabThree.LRU(stringReference, int(numFrames.get()), numPages)
def faultsLFU():
	tabThree.LFU(stringReference, int(numFrames.get()), numPages)
def faultsNRU():
	tabThree.NRU(stringReference, int(numFrames.get()), numPages)

def frameThree():
	##########################################################################
	#####################  STACK FRAMES VERTICALLY   #########################
	##########################################################################
	arrival = Frame(three)
	arrival.pack(side = TOP)
	page = Frame(three)
	page.pack(side = TOP)
	setSpecs = Frame(three)
	setSpecs.pack(side = TOP)
	info = Frame(three)
	info.pack(side = TOP)
	cmds = Frame(three)
	cmds.pack(side = TOP)
	info2 = Frame(three)
	info2.pack(side = TOP)

	##########################################################################
	######################           ARRIVAL         #########################
	##########################################################################
	arrivalLabel = []
	arrivalEntry = []	
	
	Label(arrival, text = "Process Number\t\t\t").pack(side = LEFT)
	
	for i in range(M):
		arrivalLabel.append(Label(arrival, width = 8, text = str( i)))
		arrivalLabel[i].pack(side = LEFT)	
	
	##########################################################################
	#######################           PAGE           #########################
	##########################################################################
	string = []
	entry = []
	
	Label(page, text = "Frame\t\t\t").pack(side = LEFT)
	
	for i in range(M):
		string.append(StringVar())
		entry.append(Entry(page, width = 8, textvariable = string[i]))
		entry[i].pack(side = LEFT)
	
	##########################################################################	
	######################          SET SPECS        #########################
	##########################################################################
	Label(setSpecs, text = "Number of Frames  ").pack(side = LEFT)

	numFrames = StringVar()
	numFrames.set(3)
	numFramesEntry = Entry(setSpecs, width = 8, textvariable = numFrames)
	numFramesEntry.pack(side = LEFT)
	
	Label(setSpecs, text = "   Number of Pages  ").pack(side = LEFT)
	
	numPages = StringVar()
	numPages.set(5)
	numPagesEntry = Entry(setSpecs, width = 8, textvariable = numPages)
	numPagesEntry.pack(side = LEFT)
	
	##########################################################################
	#######################            INFO          #########################
	##########################################################################
	Label(info, text = "\n").pack(side = TOP)
	Button(info, width = 30, text = "Generate Reference String", command = genReferenceString).pack(side = TOP)	
	Button(info, width = 30, text = "Clear All", command = clearAll).pack(side = TOP)

	Label(info, text = "\n\nPAGE REPLACEMENT ALGORITHMS\n").pack(side = TOP)
	Label(info, text = "Pick an algorithm to view Page Faults\n").pack(side=TOP)
		
	##########################################################################
	#######################            CMDS          #########################
	##########################################################################
	Button(cmds, width = 10, text = "FIFO", command = faultsFIFO).pack(side = LEFT)	
	Button(cmds, width = 10, text = "Optimal", command = faultsOptimal).pack(side = LEFT)	
	Button(cmds, width = 10, text = "LRU", command = faultsLRU).pack(side = LEFT)	
	Button(cmds, width = 10, text = "LFU", command = faultsLFU).pack(side = LEFT)	
	Button(cmds, width = 10, text = "NRU", command = faultsNRU).pack(side = LEFT)
	
	##########################################################################
	#######################           INFO2          #########################
	##########################################################################
	
	Label(info2, text = "\nFIRST IN FIRST OUT: The FIFO algorithm replaces the page that was put into a frame first. It will only not replace a page when the page comming \nin is the same page selected to be replaced.\n").pack(side = TOP)
	Label(info2, text = "\nOPTIMAL: The optimal algorithm replaces the page that will not be used for the longest amount of time.\t\t\t\t\t\n").pack(side = TOP)
	Label(info2, text = "\nLEAST RECENTLY USED: The LRU algorithm replaces the page that was least recently used.\t\t\t\t\t\t\t\n").pack(side = TOP)
	Label(info2, text = "\nLEAST FREQUENTLY USED: The LFU algorithm replaces the page that has been used the least. In the case of a tie, it defaults to the FIFO algorithm.\n").pack(side = TOP)
	Label(info2, text = "\nNOT RECENTLY USED: The NRU algorithm replaces a page that has not recently been used. It does this by keeping an array of bools, 1 for 'has been \nrecently used' and 0 for 'has not been recently used'").pack(side = TOP)

	return string, numFrames, numPages
	
#########################################################################################################################
##################################    END FUNCTIONS FOR TAB THREE: PAGE REPLACEMENT   ###################################
#########################################################################################################################
	
#def frameTwo():
	
#def frameThree():
	
M = 15

root,one,two,three = gui()
stringOne, stringTwo, maxB, maxP, RRtime = frameOne()
address, offset, maxO, maxA = frameTwo()
stringReference, numFrames, numPages = frameThree()

root.mainloop()
