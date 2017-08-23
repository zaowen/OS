# Author: Christer Karlsson
# Written Spring 2016 for CSC317 COA.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = cc 

FLAGS = -g -Wall

OBJS = b18.o
# the build target executable:
TARGET = b18

# specific targets
b18: $(OBJS)
	$(CC) $(FLAGS) -o $@ $(OBJS) -lm

b18.o:b18.c
	$(CC) -c $(FLAGS) $< -lm

# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
