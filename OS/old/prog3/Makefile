CC = cc
RM=\rm -f

OBJS = dipc.o 
FLAGS= -pthread -c -g -Wall

main: $(OBJS)
	$(CC) -pthread -o  dipc -g  $(OBJS) 

dipc.o:dipc.c
	$(CC)  $(FLAGS) dipc.c
dipcrm: dipcrm.c
	$(CC) -pthread dipcrm.c -o dipcrm

clean:
	-$(RM) dipc
	-$(RM) *.o
	-$(RM) *~
	-$(RM) core
