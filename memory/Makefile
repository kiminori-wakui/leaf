TARGET = mem.o
CC=gcc
CFLAGS = -c -g -DDEBUG -Wall -ansi -pedantic
OBJS = memory.o storage.o
INCLUDES = -I../include

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
clean:
	rm -f *.o *~
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $*.c
main.o: main.c ../MEM.h
memory.o: memory.c memory.h ../include/MEM.h
storage.o: storage.c memory.h ../include/MEM.h
