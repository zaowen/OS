# Author: Christer Karlsson
# Written Spring 2016 for CSC317 COA.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = g++

# the build target executable:
TARGET = sort2 sort1

all: $(TARGET)

# specific targets
sort1:		sort1.cpp
		$(CC)  -o $@ $?  -lm

sort2:		sort2.cpp
		$(CC)  -o $@ $?  -lm


# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
