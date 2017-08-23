# Use the GNU C/C++ compiler:
CC = cc

#OBJECT FILES
OBJS = bankers.o bank.o customer.o 

#FLAGS
FLAGS = -W

LIBS = -lpthread

#TARGET FILES
TARGET = bankers

all: $(TARGET)

debug: CC += -D DEBUG -g
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LIBS) $(FLAGS) -o $(TARGET) $(OBJS)

bankers.o: bankers.c bank.c
	$(CC) $(LIBS) $(FLAGS) -c bankers.c

bank.o: bank.c
	$(CC) $(LIBS)  $(FLAGS) -c bank.c

customer.o: customer.c bank.c
	$(CC) $(LIBS)  $(FLAGS) -c customer.c

clean:
	$(RM) *.o *~ $(TARGET)
