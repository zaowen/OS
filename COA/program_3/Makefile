# Author: Christer Karlsson
# Written Spring 2016 for CSC317 COA.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = g++

FLAGS = -g  -std=c++11

OBJS = b17.o operators.o
# the build target executable:
TARGET = b17

# specific targets
b17: $(OBJS)
	$(CC) $(FLAGS) -o $@ $(OBJS) -lm

b17.o:b17.cpp
	$(CC) -c $(FLAGS) $< -lm

operators.o:operators.cpp
	$(CC) -c $(FLAGS) $< -lm

# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
