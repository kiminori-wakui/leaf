TARGET1 = error.o
TARGET2 = error_message.o
TARGET3 = string.o
TARGET4 = string_pool.o
TARGET5 = util.o
TARGET6 = interpreter_manager.o
TARGET7 = eval.o
all:TARGET1 TARGET2 TARGET3 TARGET4 TARGET5 TARGET6 TARGET7

CC=gcc
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
INCLUDES = -I../include

clean:
	rm -f *.o *~
TARGET1:error.c ../include/MEM.h ../include/leaf.h \
		../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) error.c $(INCLUDES)
TARGET2:error_message.c ../include/MEM.h ../include/leaf.h \
		../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) error_message.c $(INCLUDES)
TARGET3:string.c ../include/MEM.h ../include/leaf.h \
		../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) string.c $(INCLUDES)
TARGET4:string_pool.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) string_pool.c $(INCLUDES)
TARGET5:util.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) util.c $(INCLUDES)
TARGET6:interpreter_manager.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) interpreter_manager.c $(INCLUDES)
TARGET7:eval.c ../include/MEM.h ../include/DBG.h \
		../include/leaf.h ../include/LF.h ../include/LF_dev.h
	$(CC) $(CFLAGS) eval.c $(INCLUDES)
