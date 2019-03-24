TARGET1 = main.o
TARGET2 = execute.o
TARGET3 = native.o

all:TARGET1 TARGET2 TARGET3

CC=gcc
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
INCLUDES = -I../include

clean:
	rm -f *.o *~
TARGET1:main.c ../include/LF.h ../include/MEM.h
	$(CC) $(CFLAGS) main.c $(INCLUDES)
TARGET2:execute.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) execute.c $(INCLUDES)
TARGET3:native.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) native.c $(INCLUDES)

