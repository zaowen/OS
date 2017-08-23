from Tkinter import *
from ttk import *
import random 

tlbFrames = 3
pages = 30
frames = 10

def genAddress(address):
	address.set(random.randint(1, pages))
	print address.get()

def playPageFetch():
	return
	
def genTables(numPages, numFrames, TLBFrames):
	chart = Tk()
	chart.title("Memory Management Unit")
	
	first = Frame(chart)
	first.pack(side = TOP)
	second = Frame(chart)
	second.pack(side = TOP)
	third = Frame(chart)
	third.pack(side = TOP)
	
	tlbFrames = TLBFrames
	pages = numPages
	frames = numFrames
	
	Label(first, width = 30, text = "Virtual Page Number").pack(side = LEFT)

	address = StringVar()
	address.set(3)
	addressEntry = Entry(first, width = 8, textvariable = address)
	addressEntry.pack(side = LEFT)
	
	Label(first, text = "\t\t").pack(side = LEFT)
	
	Button(first, width = 30, text = "Generate Address", command = lambda: genAddress(address)).pack(side = LEFT)
	
	#make: virtual address Entry
	
			#tlb
			#memory
			