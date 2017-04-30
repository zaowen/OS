from Tkinter import *
from ttk import *
import random 

def FIFO(stringReference, frames, pages):
	chart = Tk()
	chart.title("First In First Out Page Replacement")
	stringRef = stringReference[::-1]
	string, pageFaults, frameset = Initialize(frames, len(stringRef), chart)
	string[0][0].set("hoho")
	pageFaults[0].set("mimi")

	global numPageFaults
	numPageFaults = 0
	col = 0
	index = 0
	isNew = 1
	pageFault = 0
	drawstring = []
	for p in range (frames):
		drawstring.append("")
	
	for i in stringRef:
		isNew = 1
		if i.get() != "":
			if index == frames:
				index = 0
			print drawstring
			#case where the first page in the frames is the same page that is coming in
			if drawstring[index] == i:
				print i
				pageFault = 0
				Draw(drawstring, frames, col, pageFault, string, pageFaults)
				col = col + 1
				index = index + 1	
				
			else:
				for q in drawstring:
					#case where the page coming in already exists in a frame
					if q == i:
						pageFault = 0
						Draw(drawstring, frames, col, pageFault, string, pageFaults)
						isNew = 0
						col = col + 1
				if isNew == 1:
					print "\t\treplacing"
					drawstring[index] = i.get()
					pageFault = 1
					Draw(drawstring, frames, col, pageFault, string, pageFaults)
					col = col + 1
					index = index + 1
	return
	
def Optimal(stringRef, frames, pages):
	chart = Tk()
	chart.title("Optimal Page Replacement")
	string, pageFaults = Initialize(frames, len(stringRef), chart)
	return
	
def LRU(stringRef, frames, pages):
	chart = Tk()
	chart.title("Least Recently Used Page Replacement")
	string, pageFaults = Initialize(frames, len(stringRef), chart)
	return
	
def LFU(stringRef, frames, pages):
	chart = Tk()
	chart.title("Least Frequently Used Page Replacement")
	string, pageFaults = Initialize(frames, len(stringRef), chart)
	return
	
def NRU(stringRef, frames, pages):
	chart = Tk()
	chart.title("Not Recently Used Page Replacement")
	string, pageFaults = Initialize(frames, len(stringRef), chart)
	return
	
def Initialize(rows, columns, chart):
	chart.geometry("1000x300")
	
	frameset = []
	string = []
	#entry = []	
	faults = []
	faultsEntry = []
	title = Frame(chart)
	title.pack(side = TOP)
	
	Label(title, text = "\n\n").pack(side = TOP)
	
	for j in range (rows):
		frameset.append(Frame(chart))
		frameset[j].pack(side=TOP)
		string.append(addLine(frameset[j], columns))
		print string[j]
			
		#string.append(newstring)
		#entry.append(newentry)

	frameset.append(Frame(chart))
	frameset[rows].pack(side = TOP)
	for n in range (columns + 1):	
		Label(frameset[rows], text = "\t").pack(side = LEFT)
		faults.append(StringVar())
		faultsEntry.append(Entry(frameset[rows], width = 5, textvariable = faults[n]))
		faultsEntry[n].pack(side = LEFT)			
		faultsEntry[n].configure(state = 'readonly')
			
	return string, faults, frameset
	
def addLine(frame, cols):
	print cols
	newstring = []
	newentry = []
	for i in range (cols):
		Label(frame, text = "\t").pack(side = LEFT)
		newstring.append(StringVar())
		newstring[i].set(50)
		print newstring[i].get()
		newentry.append(Entry(frame, width = 5, textvariable = newstring[i]))
		newentry[i].pack(side = LEFT)			
		newentry[i].configure(state = 'readonly')
	return newstring

def Draw(draw, rows, column, fault, string, pageFaults):
	print rows
	print column
	print pageFaults
	
	for i in range (rows):
		try:
			string[i][column].set(draw[i])
			print string[i][column]
		except TypeError:
			print i, string[i]
		
	if fault == 1:
		pageFaults[column + 1].set('X')
		numPageFaults = numPageFaults + 1
		