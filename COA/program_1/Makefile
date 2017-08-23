# Author: Christer Karlsson
# Written Spring 2016 for CSC317 COA.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = g++
# LIBS = -lpthread
FLAGS = -g -std=c++11 -Wall

# the build target executable:
TARGET = make_array tape_sort sort2 sort1

all: $(TARGET)

# specific targets
make_array:	make_array.cpp
		$(CC) $(FLAGS) -o $@ $? $(LIBS) -lm

sort1:		sort1.cpp
		$(CC) $(FLAGS) -o $@ $? $(LIBS) -lm

sort2:		sort2.cpp
		$(CC) $(FLAGS) -o $@ $? $(LIBS) -lm

tape_sort:	tape_sort.cpp
		$(CC) $(FLAGS) -o $@ $? $(LIBS) -lm

debugging:	tape_sort.cpp
		$(CC) $(FLAGS) -DDEBUG -o tape_sort $? $(LIBS) -lm

# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
