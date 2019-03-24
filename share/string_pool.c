#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "leaf.h"

static LF_String *
alloc_lf_string(LF_Interpreter *inter, char *str, LF_Boolean is_literal)
{
    LF_String *ret;

    ret = MEM_malloc(sizeof(LF_String));
    ret->ref_count = 0;
    ret->is_literal = is_literal;
    ret->string = str;

    return ret;
}

LF_String *
lf_literal_to_lf_string(LF_Interpreter *inter, char *str)
{
    LF_String *ret;

    ret = alloc_lf_string(inter, str, LF_TRUE);
    ret->ref_count = 1;

    return ret;
}

void
lf_refer_string(LF_String *str)
{
    str->ref_count++;
}

void
lf_release_string(LF_String *str)
{
    str->ref_count--;

    DBG_assert(str->ref_count >= 0, ("str->ref_count..%d\n",
                                     str->ref_count));
    if (str->ref_count == 0) {
        if (!str->is_literal) {
            MEM_free(str->string);
        }
        MEM_free(str);
    }
}

LF_String *
lf_create_leaf_string(LF_Interpreter *inter, char *str)
{
    LF_String *ret = alloc_lf_string(inter, str, LF_FALSE);
    ret->ref_count = 1;

    return ret;
}
