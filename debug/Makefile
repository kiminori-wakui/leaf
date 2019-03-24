TARGET = dbg.o
CC=gcc
CFLAGS = -c -g -Wall -DDBG_NO_DEBUG -ansi -pedantic
OBJS = debug.o
INCLUDES = -I../include

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
clean:
	rm -f *.o *~
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $*.c
debug.o: debug.c ../include/MEM.h debug.h ../include/DBG.h
