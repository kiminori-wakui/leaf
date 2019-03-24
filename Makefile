TARGET = leaf
CC=gcc
OBJS = \
  ./compiler/lex.yy.o\
  ./compiler/y.tab.o\
  ./interpreter/main.o\
  ./share/interpreter_manager.o\
  ./compiler/create.o\
  ./interpreter/execute.o\
  ./share/eval.o\
  ./share/string.o\
  ./share/string_pool.o\
  ./share/util.o\
  ./interpreter/native.o\
  ./share/error.o\
  ./share/error_message.o\
  ./memory/mem.o\
  ./debug/dbg.o
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
INCLUDES = \

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm
clean:
	rm -f *.o *~

