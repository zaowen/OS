# Use the GNU C/C++ compiler:
CC = cc

#OBJECT FILES
OBJS = main.o request.o

#FLAGS
FLAGS = -W

#TARGET FILES
TARGET = disk_sched

all: $(TARGET)

debug: CC += -D DEBUG -g
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC)  $(FLAGS) -o $(TARGET) $(OBJS)

main.o: main.c 
	$(CC)  $(FLAGS) -c main.c

request.o: request.c 
	$(CC) $(FLAGS) -c request.c

clean:
	$(RM) *.o *~ $(TARGET)
