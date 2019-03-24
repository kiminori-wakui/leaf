#include <stdio.h>
#include "LF.h"
#include "MEM.h"

int
main(int argc, char **argv)
{
    LF_Interpreter     *interpreter;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }
    interpreter = LF_create_interpreter();
    LF_compile(interpreter, fp);
    LF_interpret(interpreter);
    LF_dispose_interpreter(interpreter);

    MEM_dump_blocks(stdout);

    return 0;
}
