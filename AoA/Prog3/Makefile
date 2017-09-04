

OBJS=prog3.o 

LFLAGS=

CFLAGS= -std=c++11

BFLAGS= 

main: $(OBJS)
	g++ $(BFLAGS) -g -o $@ $(OBJS) 

.cpp.o:
	g++ $(CFLAGS) -g -c  $<


clean:
	rm -f *.o *~ alpha tmp.data main
