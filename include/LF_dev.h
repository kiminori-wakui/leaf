#ifndef PUBLIC_LF_DEV_H_INCLUDED
#define PUBLIC_LF_DEV_H_INCLUDED
#include "LF.h"

typedef enum {
    LF_FALSE = 0,
    LF_TRUE = 1
} LF_Boolean;

typedef struct LF_String_tag LF_String;

typedef struct {
    char        *name;
} LF_NativePointerInfo;

typedef enum {
    LF_BOOLEAN_VALUE = 1,
    LF_INT_VALUE,
    LF_DOUBLE_VALUE,
    LF_STRING_VALUE,
    LF_NATIVE_POINTER_VALUE,
    LF_NULL_VALUE
} LF_ValueType;

typedef struct {
    LF_NativePointerInfo       *info;
    void                        *pointer;
} LF_NativePointer;

typedef struct {
    LF_ValueType       type;
    union {
        LF_Boolean     boolean_value;
        int             int_value;
        double          double_value;
        LF_String      *string_value;
        LF_NativePointer       native_pointer;
    } u;
} LF_Value;

typedef LF_Value LF_NativeFunctionProc(LF_Interpreter *interpreter,
                                         int arg_count, LF_Value *args);

void LF_add_native_function(LF_Interpreter *interpreter,
                             char *name, LF_NativeFunctionProc *proc);
void LF_add_global_variable(LF_Interpreter *inter,
                             char *identifier, LF_Value *value);

#endif /* PUBLIC_LF_DEV_H_INCLUDED */
