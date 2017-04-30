# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

#OBJECT FILES
OBJS = main.o student.o ta.o hangout.o help_student.o

#FLAGS
FLAGS = -g

#TARGET FILES
TARGET = sleepingta

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -pthread $(FLAGS) -o $(TARGET) $(OBJS)

main.o: main.c ta.h
	$(CC) -pthread $(FLAGS) -c main.c

student.o: student.c ta.h
	$(CC) -pthread  $(FLAGS) -c student.c

ta.o: ta.c ta.h
	$(CC) -pthread  $(FLAGS) -c ta.c

hangout.o: hangout.c
	$(CC) -c $(FLAGS) hangout.c

help_student.o: help_student.c
	$(CC) -c $(FLAGS) help_student.c

clean:
	$(RM) *.o *~ $(TARGET)
