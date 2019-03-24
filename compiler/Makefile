TARGET1 = lex.yy.o
TARGET2 = y.tab.o
TARGET3 = create.o

all:TARGET1 TARGET2 TARGET3

CC=gcc
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
INCLUDES = -I../include

clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~
TARGET1:lex.yy.c ../include/leaf.h ../include/MEM.h ../include/y.tab.h
	$(CC) -c -g lex.yy.c $(INCLUDES)
TARGET2:y.tab.c ../include/leaf.h ../include/MEM.h
	$(CC) -c -g y.tab.c $(INCLUDES)
TARGET3:create.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) create.c $(INCLUDES)

