# Use the GNU C/C++ compiler:
CC = cc

#OBJECT FILES
OBJS = memman.o

#FLAGS
FLAGS = -g

#TARGET FILES
TARGET = memman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LIBS) $(FLAGS) -o $(TARGET) $(OBJS)

memman.o: memman.c
	$(CC) $(LIBS) $(FLAGS) -c memman.c

clean:
	$(RM) *.o *~ $(TARGET)
