# Author: Zachary Owen
# Written Fall 2016 for CSC410

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C compiler and linker:
CC = gcc
FLAGS = -fopenmp -Wall

# the build target executable:
TARGET = sieve sat

all: $(TARGET)

# specific targets
sieve:	sieve.c
		$(CC) $(FLAGS) -o $@ $? $(LIBS) 

sat:	sat.c
		$(CC) $(FLAGS) -o $@ $? $(LIBS) 

# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
