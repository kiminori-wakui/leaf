#ifndef PUBLIC_LF_H_INCLUDED
#define PUBLIC_LF_H_INCLUDED
#include <stdio.h>

typedef struct LF_Interpreter_tag LF_Interpreter;

LF_Interpreter *LF_create_interpreter(void);
void LF_compile(LF_Interpreter *interpreter, FILE *fp);
void LF_interpret(LF_Interpreter *interpreter);
void LF_dispose_interpreter(LF_Interpreter *interpreter);

#endif /* PUBLIC_LF_H_INCLUDED */
