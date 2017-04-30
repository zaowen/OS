CC = cc
RM=\rm -f

OBJS = dsh.o 
FLAGS= -pthread -c -g

main: $(OBJS)
	$(CC) -pthread -o  dsh -g  $(OBJS) 

dsh.o:dsh.c
	$(CC)  $(FLAGS) dsh.c

clean:
	-$(RM) dsh
	-$(RM) *.o
	-$(RM) *~
	-$(RM) core
