#ifndef PRIVATE_CROWBAR_H_INCLUDED
#define PRIVATE_CROWBAR_H_INCLUDED
#include <stdio.h>
#include "MEM.h"
#include "LF.h"
#include "LF_dev.h"

#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX    (256)
#define LINE_BUF_SIZE           (1024)

typedef enum {
    PARSE_ERR = 1,
    CHARACTER_INVALID_ERR,
    FUNCTION_MULTIPLE_DEFINE_ERR,
    COMPILE_ERROR_COUNT_PLUS_1
} CompileError;

typedef enum {
    VARIABLE_NOT_FOUND_ERR = 1,
    FUNCTION_NOT_FOUND_ERR,
    ARGUMENT_TOO_MANY_ERR,
    ARGUMENT_TOO_FEW_ERR,
    NOT_BOOLEAN_TYPE_ERR,
    MINUS_OPERAND_TYPE_ERR,
    BAD_OPERAND_TYPE_ERR,
    NOT_BOOLEAN_OPERATOR_ERR,
    FOPEN_ARGUMENT_TYPE_ERR,
    FCLOSE_ARGUMENT_TYPE_ERR,
    FGETS_ARGUMENT_TYPE_ERR,
    FPUTS_ARGUMENT_TYPE_ERR,
    NOT_NULL_OPERATOR_ERR,
    DIVISION_BY_ZERO_ERR,
    GLOBAL_VARIABLE_NOT_FOUND_ERR,
    GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,
    BAD_OPERATOR_FOR_STRING_ERR,
    RUNTIME_ERROR_COUNT_PLUS_1
} RuntimeError;

typedef enum {
    INT_MESSAGE_ARGUMENT = 1,
    DOUBLE_MESSAGE_ARGUMENT,
    STRING_MESSAGE_ARGUMENT,
    CHARACTER_MESSAGE_ARGUMENT,
    POINTER_MESSAGE_ARGUMENT,
    MESSAGE_ARGUMENT_END
} MessageArgumentType;

typedef struct {
    char *format;
} MessageFormat;

typedef struct Expression_tag Expression;

typedef enum {
    BOOLEAN_EXPRESSION = 1,
    INT_EXPRESSION,
    DOUBLE_EXPRESSION,
    STRING_EXPRESSION,
    IDENTIFIER_EXPRESSION,
    ASSIGN_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    EQ_EXPRESSION,
    NE_EXPRESSION,
    GT_EXPRESSION,
    GE_EXPRESSION,
    LT_EXPRESSION,
    LE_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    MINUS_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    NULL_EXPRESSION,
    EXPRESSION_TYPE_COUNT_PLUS_1
} ExpressionType;

#define dkc_is_math_operator(operator) \
  ((operator) == ADD_EXPRESSION || (operator) == SUB_EXPRESSION\
   || (operator) == MUL_EXPRESSION || (operator) == DIV_EXPRESSION\
   || (operator) == MOD_EXPRESSION)

#define dkc_is_compare_operator(operator) \
  ((operator) == EQ_EXPRESSION || (operator) == NE_EXPRESSION\
   || (operator) == GT_EXPRESSION || (operator) == GE_EXPRESSION\
   || (operator) == LT_EXPRESSION || (operator) == LE_EXPRESSION)

#define dkc_is_logical_operator(operator) \
  ((operator) == LOGICAL_AND_EXPRESSION || (operator) == LOGICAL_OR_EXPRESSION)

typedef struct ArgumentList_tag {
    Expression *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

typedef struct {
    char        *variable;
    Expression  *operand;
} AssignExpression;

typedef struct {
    Expression  *left;
    Expression  *right;
} BinaryExpression;

typedef struct {
    char                *identifier;
    ArgumentList        *argument;
} FunctionCallExpression;

struct Expression_tag {
    ExpressionType type;
    int line_number;
    union {
        LF_Boolean             boolean_value;
        int                     int_value;
        double                  double_value;
        char                    *string_value;
        char                    *identifier;
        AssignExpression        assign_expression;
        BinaryExpression        binary_expression;
        Expression              *minus_expression;
        FunctionCallExpression  function_call_expression;
    } u;
};

typedef struct Statement_tag Statement;

typedef struct StatementList_tag {
    Statement   *statement;
    struct StatementList_tag    *next;
} StatementList;

typedef struct {
    StatementList       *statement_list;
} Block;

typedef struct IdentifierList_tag {
    char        *name;
    struct IdentifierList_tag   *next;
} IdentifierList;

typedef struct {
    IdentifierList      *identifier_list;
} GlobalStatement;

typedef struct Elsif_tag {
    Expression  *condition;
    Block       *block;
    struct Elsif_tag    *next;
} Elsif;

typedef struct {
    Expression  *condition;
    Block       *then_block;
    Elsif       *elsif_list;
    Block       *else_block;
} IfStatement;

typedef struct {
    Expression  *condition;
    Block       *block;
} WhileStatement;

typedef struct {
    Expression  *init;
    Expression  *condition;
    Expression  *post;
    Block       *block;
} ForStatement;

typedef struct {
    Expression *return_value;
} ReturnStatement;

typedef enum {
    EXPRESSION_STATEMENT = 1,
    GLOBAL_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

struct Statement_tag {
    StatementType       type;
    int                 line_number;
    union {
        Expression      *expression_s;
        GlobalStatement global_s;
        IfStatement     if_s;
        WhileStatement  while_s;
        ForStatement    for_s;
        ReturnStatement return_s;
    } u;
};

typedef struct ParameterList_tag {
    char        *name;
    struct ParameterList_tag *next;
} ParameterList;

typedef enum {
    CROWBAR_FUNCTION_DEFINITION = 1,
    NATIVE_FUNCTION_DEFINITION
} FunctionDefinitionType;

typedef struct FunctionDefinition_tag {
    char                *name;
    FunctionDefinitionType      type;
    union {
        struct {
            ParameterList       *parameter;
            Block               *block;
        } leaf_f;
        struct {
            LF_NativeFunctionProc      *proc;
        } native_f;
    } u;
    struct FunctionDefinition_tag       *next;
} FunctionDefinition;

typedef struct Variable_tag {
    char        *name;
    LF_Value   value;
    struct Variable_tag *next;
} Variable;

typedef enum {
    NORMAL_STATEMENT_RESULT = 1,
    RETURN_STATEMENT_RESULT,
    BREAK_STATEMENT_RESULT,
    CONTINUE_STATEMENT_RESULT,
    STATEMENT_RESULT_TYPE_COUNT_PLUS_1
} StatementResultType;

typedef struct {
    StatementResultType type;
    union {
        LF_Value       return_value;
    } u;
} StatementResult;

typedef struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct {
    Variable    *variable;
    GlobalVariableRef   *global_variable;
} LocalEnvironment;

struct LF_String_tag {
    int         ref_count;
    char        *string;
    LF_Boolean is_literal;
};

typedef struct {
    LF_String  *strings;
} StringPool;

struct LF_Interpreter_tag {
    MEM_Storage         interpreter_storage;
    MEM_Storage         execute_storage;
    Variable            *variable;
    FunctionDefinition  *function_list;
    StatementList       *statement_list;
    int                 current_line_number;
};


/* create.c */
void lf_function_define(char *identifier, ParameterList *parameter_list,
                         Block *block);
ParameterList *lf_create_parameter(char *identifier);
ParameterList *lf_chain_parameter(ParameterList *list,
                                   char *identifier);
ArgumentList *lf_create_argument_list(Expression *expression);
ArgumentList *lf_chain_argument_list(ArgumentList *list, Expression *expr);
StatementList *lf_create_statement_list(Statement *statement);
StatementList *lf_chain_statement_list(StatementList *list,
                                        Statement *statement);
Expression *lf_alloc_expression(ExpressionType type);
Expression *lf_create_assign_expression(char *variable,
                                             Expression *operand);
Expression *lf_create_binary_expression(ExpressionType operator,
                                         Expression *left,
                                         Expression *right);
Expression *lf_create_minus_expression(Expression *operand);
Expression *lf_create_identifier_expression(char *identifier);
Expression *lf_create_function_call_expression(char *func_name,
                                                ArgumentList *argument);
Expression *lf_create_boolean_expression(LF_Boolean value);
Expression *lf_create_null_expression(void);
Statement *lf_create_global_statement(IdentifierList *identifier_list);
IdentifierList *lf_create_global_identifier(char *identifier);
IdentifierList *lf_chain_identifier(IdentifierList *list, char *identifier);
Statement *lf_create_if_statement(Expression *condition,
                                    Block *then_block, Elsif *elsif_list,
                                    Block *else_block);
Elsif *lf_chain_elsif_list(Elsif *list, Elsif *add);
Elsif *lf_create_elsif(Expression *expr, Block *block);
Statement *lf_create_while_statement(Expression *condition, Block *block);
Statement *lf_create_for_statement(Expression *init, Expression *cond,
                                    Expression *post, Block *block);
Block *lf_create_block(StatementList *statement_list);
Statement *lf_create_expression_statement(Expression *expression);
Statement *lf_create_return_statement(Expression *expression);
Statement *lf_create_break_statement(void);
Statement *lf_create_continue_statement(void);

/* string.c */
char *lf_create_identifier(char *str);
void lf_open_string_literal(void);
void lf_add_string_literal(int letter);
void lf_reset_string_literal_buffer(void);
char *lf_close_string_literal(void);

/* execute.c */
StatementResult
lf_execute_statement_list(LF_Interpreter *inter,
                           LocalEnvironment *env, StatementList *list);

/* eval.c */
LF_Value lf_eval_binary_expression(LF_Interpreter *inter,
                                 LocalEnvironment *env,
                                 ExpressionType operator,
                                 Expression *left, Expression *right);
LF_Value lf_eval_minus_expression(LF_Interpreter *inter,
                                LocalEnvironment *env, Expression *operand);
LF_Value lf_eval_expression(LF_Interpreter *inter,
                          LocalEnvironment *env, Expression *expr);

/* string_pool.c */
LF_String *lf_literal_to_lf_string(LF_Interpreter *inter, char *str);
void lf_refer_string(LF_String *str);
void lf_release_string(LF_String *str);
LF_String *lf_search_lf_string(LF_Interpreter *inter, char *str);
LF_String *lf_create_leaf_string(LF_Interpreter *inter, char *str);

/* util.c */
LF_Interpreter *lf_get_current_interpreter(void);
void lf_set_current_interpreter(LF_Interpreter *inter);
void *lf_malloc(size_t size);
void *lf_execute_malloc(LF_Interpreter *inter, size_t size);
Variable *lf_search_local_variable(LocalEnvironment *env,
                                    char *identifier);
Variable *
lf_search_global_variable(LF_Interpreter *inter, char *identifier);
void lf_add_local_variable(LocalEnvironment *env,
                            char *identifier, LF_Value *value);
LF_NativeFunctionProc *
lf_search_native_function(LF_Interpreter *inter, char *name);
FunctionDefinition *lf_search_function(char *name);
char *lf_get_operator_string(ExpressionType type);

/* error.c */
void lf_compile_error(CompileError id, ...);
void lf_runtime_error(int line_number, RuntimeError id, ...);

/* native.c */
LF_Value lf_nv_print_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args);
LF_Value lf_nv_fopen_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args);
LF_Value lf_nv_fclose_proc(LF_Interpreter *interpreter,
                             int arg_count, LF_Value *args);
LF_Value lf_nv_fgets_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args);
LF_Value lf_nv_fputs_proc(LF_Interpreter *interpreter,
                            int arg_count, LF_Value *args);
void lf_add_std_fp(LF_Interpreter *inter);

#endif /* PRIVATE_CROWBAR_H_INCLUDED */
