#include "MEM.h"
#include "DBG.h"
#define GLOBAL_VARIABLE_DEFINE
#include "leaf.h"

static void
add_native_functions(LF_Interpreter *inter)
{
    LF_add_native_function(inter, "print", lf_nv_print_proc);
    LF_add_native_function(inter, "fopen", lf_nv_fopen_proc);
    LF_add_native_function(inter, "fclose", lf_nv_fclose_proc);
    LF_add_native_function(inter, "fgets", lf_nv_fgets_proc);
    LF_add_native_function(inter, "fputs", lf_nv_fputs_proc);
}

LF_Interpreter *
LF_create_interpreter(void)
{
    MEM_Storage storage;
    LF_Interpreter *interpreter;

    storage = MEM_open_storage(0);
    interpreter = MEM_storage_malloc(storage,
                                     sizeof(struct LF_Interpreter_tag));
    interpreter->interpreter_storage = storage;
    interpreter->execute_storage = NULL;
    interpreter->variable = NULL;
    interpreter->function_list = NULL;
    interpreter->statement_list = NULL;
    interpreter->current_line_number = 1;

    lf_set_current_interpreter(interpreter);
    add_native_functions(interpreter);

    return interpreter;
}

void
LF_compile(LF_Interpreter *interpreter, FILE *fp)
{
    extern int yyparse(void);
    extern FILE *yyin;

    lf_set_current_interpreter(interpreter);

    yyin = fp;
    if (yyparse()) {
        /* BUGBUG */
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
    lf_reset_string_literal_buffer();
}

void
LF_interpret(LF_Interpreter *interpreter)
{
    interpreter->execute_storage = MEM_open_storage(0);
    lf_add_std_fp(interpreter);
    lf_execute_statement_list(interpreter, NULL, interpreter->statement_list);
}

static void
release_global_strings(LF_Interpreter *interpreter) {
    while (interpreter->variable) {
        Variable *temp = interpreter->variable;
        interpreter->variable = temp->next;
        if (temp->value.type == LF_STRING_VALUE) {
            lf_release_string(temp->value.u.string_value);
        }
    }
}

void
LF_dispose_interpreter(LF_Interpreter *interpreter)
{
    release_global_strings(interpreter);

    if (interpreter->execute_storage) {
        MEM_dispose_storage(interpreter->execute_storage);
    }

    MEM_dispose_storage(interpreter->interpreter_storage);
}

void
LF_add_native_function(LF_Interpreter *interpreter,
                        char *name, LF_NativeFunctionProc *proc)
{
    FunctionDefinition *fd;

    fd = lf_malloc(sizeof(FunctionDefinition));
    fd->name = name;
    fd->type = NATIVE_FUNCTION_DEFINITION;
    fd->u.native_f.proc = proc;
    fd->next = interpreter->function_list;

    interpreter->function_list = fd;
}
