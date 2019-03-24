#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "LF_dev.h"
#include "leaf.h"

#define NATIVE_LIB_NAME "leaf.lang.file"

static LF_NativePointerInfo st_native_lib_info = {
    NATIVE_LIB_NAME
};

LF_Value lf_nv_print_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args)
{
    LF_Value value;

    value.type = LF_NULL_VALUE;

    if (arg_count < 1) {
        lf_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 1) {
        lf_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    switch (args[0].type) {
    case LF_BOOLEAN_VALUE:
        if (args[0].u.boolean_value) {
            printf("true");
        } else {
            printf("false");
        }
        break;
    case LF_INT_VALUE:
        printf("%d", args[0].u.int_value);
        break;
    case LF_DOUBLE_VALUE:
        printf("%f", args[0].u.double_value);
        break;
    case LF_STRING_VALUE:
        printf("%s", args[0].u.string_value->string);
        break;
    case LF_NATIVE_POINTER_VALUE:
        printf("(%s:%p)",
               args[0].u.native_pointer.info->name,
               args[0].u.native_pointer.pointer);
        break;
    case LF_NULL_VALUE:
        printf("null");
        break;
    }

    return value;
}

LF_Value lf_nv_fopen_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args)
{
    LF_Value value;
    FILE *fp;

    if (arg_count < 2) {
        lf_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 2) {
        lf_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != LF_STRING_VALUE
        || args[1].type != LF_STRING_VALUE) {
        lf_runtime_error(0, FOPEN_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    
    fp = fopen(args[0].u.string_value->string,
               args[1].u.string_value->string);
    if (fp == NULL) {
        value.type = LF_NULL_VALUE;
    } else {
        value.type = LF_NATIVE_POINTER_VALUE;
        value.u.native_pointer.info = &st_native_lib_info;
        value.u.native_pointer.pointer = fp;
    }

    return value;
}

static LF_Boolean
check_native_pointer(LF_Value *value)
{
    return value->u.native_pointer.info == &st_native_lib_info;
}

LF_Value lf_nv_fclose_proc(LF_Interpreter *interpreter,
                             int arg_count, LF_Value *args)
{
    LF_Value value;
    FILE *fp;

    value.type = LF_NULL_VALUE;
    if (arg_count < 1) {
        lf_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 1) {
        lf_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != LF_NATIVE_POINTER_VALUE
        || !check_native_pointer(&args[0])) {
        lf_runtime_error(0, FCLOSE_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    fp = args[0].u.native_pointer.pointer;
    fclose(fp);

    return value;
}

LF_Value lf_nv_fgets_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args)
{
    LF_Value value;
    FILE *fp;
    char buf[LINE_BUF_SIZE];
    char *ret_buf = NULL;
    int ret_len = 0;

    if (arg_count < 1) {
        lf_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 1) {
        lf_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != LF_NATIVE_POINTER_VALUE
        || !check_native_pointer(&args[0])) {
        lf_runtime_error(0, FGETS_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    fp = args[0].u.native_pointer.pointer;

    while (fgets(buf, LINE_BUF_SIZE, fp)) {
        int new_len;
        new_len = ret_len + strlen(buf);
        ret_buf = MEM_realloc(ret_buf, new_len + 1);
        if (ret_len == 0) {
            strcpy(ret_buf, buf);
        } else {
            strcat(ret_buf, buf);
        }
        ret_len = new_len;
        if (ret_buf[ret_len-1] == '\n')
            break;
    }
    if (ret_len > 0) {
        value.type = LF_STRING_VALUE;
        value.u.string_value = lf_create_leaf_string(interpreter, ret_buf);
    } else {
        value.type = LF_NULL_VALUE;
    }

    return value;
}

LF_Value lf_nv_fputs_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args)
{
    LF_Value value;
    FILE *fp;

    value.type = LF_NULL_VALUE;
    if (arg_count < 2) {
        lf_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 2) {
        lf_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != LF_STRING_VALUE
        || (args[1].type != LF_NATIVE_POINTER_VALUE
            || !check_native_pointer(&args[1]))) {
        lf_runtime_error(0, FPUTS_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    fp = args[1].u.native_pointer.pointer;

    fputs(args[0].u.string_value->string, fp);

    return value;
}

void
lf_add_std_fp(LF_Interpreter *inter)
{
    LF_Value fp_value;

    fp_value.type = LF_NATIVE_POINTER_VALUE;
    fp_value.u.native_pointer.info = &st_native_lib_info;

    fp_value.u.native_pointer.pointer = stdin;
    LF_add_global_variable(inter, "STDIN", &fp_value);

    fp_value.u.native_pointer.pointer = stdout;
    LF_add_global_variable(inter, "STDOUT", &fp_value);

    fp_value.u.native_pointer.pointer = stderr;
    LF_add_global_variable(inter, "STDERR", &fp_value);
}
