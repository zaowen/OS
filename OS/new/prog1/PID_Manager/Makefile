# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

# COMPILER OPTIONS:
CFLAGS = -c -g -Wall

#OBJECT FILES
OBJS = test.o pid.o

#TARGET FILES
TARGET = testpid

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -lpthread -o $(TARGET) $(OBJS)

pid.o: pid.c pid.h
	$(CC) $(CFLAGS) pid.c

test.o: test.c pid.h
	$(CC) -lpthread $(CFLAGS) test.c

clean:
	$(RM) *.o *~ $(TARGET)
