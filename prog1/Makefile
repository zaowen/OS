CC = cc
RM=\rm -f

OBJS = dsh.o 

main: $(OBJS)
	$(CC) -o dsh $(OBJS) 

main.o: main.c
	$(CC)  -c dsh.c

clean:
	-$(RM) dsh
	-$(RM) *.o
	-$(RM) *~
	-$(RM) core
