#include "MEM.h"
#include "DBG.h"
#include "leaf.h"

void
lf_function_define(char *identifier, ParameterList *parameter_list,
                    Block *block)
{
    FunctionDefinition *f;
    LF_Interpreter *inter;

    if (lf_search_function(identifier)) {
        lf_compile_error(FUNCTION_MULTIPLE_DEFINE_ERR,
                          STRING_MESSAGE_ARGUMENT, "name", identifier,
                          MESSAGE_ARGUMENT_END);
        return;
    }
    inter = lf_get_current_interpreter();

    f = lf_malloc(sizeof(FunctionDefinition));
    f->name = identifier;
    f->type = CROWBAR_FUNCTION_DEFINITION;
    f->u.leaf_f.parameter = parameter_list;
    f->u.leaf_f.block = block;
    f->next = inter->function_list;
    inter->function_list = f;
}

ParameterList *
lf_create_parameter(char *identifier)
{
    ParameterList       *p;

    p = lf_malloc(sizeof(ParameterList));
    p->name = identifier;
    p->next = NULL;

    return p;
}

ParameterList *
lf_chain_parameter(ParameterList *list, char *identifier)
{
    ParameterList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = lf_create_parameter(identifier);

    return list;
}

ArgumentList *
lf_create_argument_list(Expression *expression)
{
    ArgumentList *al;

    al = lf_malloc(sizeof(ArgumentList));
    al->expression = expression;
    al->next = NULL;

    return al;
}

ArgumentList *
lf_chain_argument_list(ArgumentList *list, Expression *expr)
{
    ArgumentList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = lf_create_argument_list(expr);

    return list;
}

StatementList *
lf_create_statement_list(Statement *statement)
{
    StatementList *sl;

    sl = lf_malloc(sizeof(StatementList));
    sl->statement = statement;
    sl->next = NULL;

    return sl;
}

StatementList *
lf_chain_statement_list(StatementList *list, Statement *statement)
{
    StatementList *pos;

    if (list == NULL)
        return lf_create_statement_list(statement);

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = lf_create_statement_list(statement);

    return list;
}

Expression *
lf_alloc_expression(ExpressionType type)
{
    Expression  *exp;

    exp = lf_malloc(sizeof(Expression));
    exp->type = type;
    exp->line_number = lf_get_current_interpreter()->current_line_number;

    return exp;
}

Expression *
lf_create_assign_expression(char *variable, Expression *operand)
{
    Expression *exp;

    exp = lf_alloc_expression(ASSIGN_EXPRESSION);
    exp->u.assign_expression.variable = variable;
    exp->u.assign_expression.operand = operand;

    return exp;
}

static Expression
convert_value_to_expression(LF_Value *v)
{
    Expression  expr;

    if (v->type == LF_INT_VALUE) {
        expr.type = INT_EXPRESSION;
        expr.u.int_value = v->u.int_value;
    } else if (v->type == LF_DOUBLE_VALUE) {
        expr.type = DOUBLE_EXPRESSION;
        expr.u.double_value = v->u.double_value;
    } else {
        DBG_assert(v->type == LF_BOOLEAN_VALUE,
                   ("v->type..%d\n", v->type));
        expr.type = BOOLEAN_EXPRESSION;
        expr.u.boolean_value = v->u.boolean_value;
    }
    return expr;
}

Expression *
lf_create_binary_expression(ExpressionType operator,
                             Expression *left, Expression *right)
{
    if ((left->type == INT_EXPRESSION
         || left->type == DOUBLE_EXPRESSION)
        && (right->type == INT_EXPRESSION
            || right->type == DOUBLE_EXPRESSION)) {
        LF_Value v;
        v = lf_eval_binary_expression(lf_get_current_interpreter(),
                                       NULL, operator, left, right);
        /* Overwriting left hand expression. */
        *left = convert_value_to_expression(&v);

        return left;
    } else {
        Expression *exp;
        exp = lf_alloc_expression(operator);
        exp->u.binary_expression.left = left;
        exp->u.binary_expression.right = right;
        return exp;
    }
}

Expression *
lf_create_minus_expression(Expression *operand)
{
    if (operand->type == INT_EXPRESSION
        || operand->type == DOUBLE_EXPRESSION) {
        LF_Value       v;
        v = lf_eval_minus_expression(lf_get_current_interpreter(),
                                      NULL, operand);
        /* Notice! Overwriting operand expression. */
        *operand = convert_value_to_expression(&v);
        return operand;
    } else {
        Expression      *exp;
        exp = lf_alloc_expression(MINUS_EXPRESSION);
        exp->u.minus_expression = operand;
        return exp;
    }
}

Expression *
lf_create_identifier_expression(char *identifier)
{
    Expression  *exp;

    exp = lf_alloc_expression(IDENTIFIER_EXPRESSION);
    exp->u.identifier = identifier;

    return exp;
}

Expression *
lf_create_function_call_expression(char *func_name, ArgumentList *argument)
{
    Expression  *exp;

    exp = lf_alloc_expression(FUNCTION_CALL_EXPRESSION);
    exp->u.function_call_expression.identifier = func_name;
    exp->u.function_call_expression.argument = argument;

    return exp;
}

Expression *
lf_create_boolean_expression(LF_Boolean value)
{
    Expression *exp;

    exp = lf_alloc_expression(BOOLEAN_EXPRESSION);
    exp->u.boolean_value = value;

    return exp;
}

Expression *
lf_create_null_expression(void)
{
    Expression  *exp;

    exp = lf_alloc_expression(NULL_EXPRESSION);

    return exp;
}

static Statement *
alloc_statement(StatementType type)
{
    Statement *st;

    st = lf_malloc(sizeof(Statement));
    st->type = type;
    st->line_number = lf_get_current_interpreter()->current_line_number;

    return st;
}

Statement *
lf_create_global_statement(IdentifierList *identifier_list)
{
    Statement *st;

    st = alloc_statement(GLOBAL_STATEMENT);
    st->u.global_s.identifier_list = identifier_list;

    return st;
}

IdentifierList *
lf_create_global_identifier(char *identifier)
{
    IdentifierList      *i_list;

    i_list = lf_malloc(sizeof(IdentifierList));
    i_list->name = identifier;
    i_list->next = NULL;

    return i_list;
}

IdentifierList *
lf_chain_identifier(IdentifierList *list, char *identifier)
{
    IdentifierList *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = lf_create_global_identifier(identifier);

    return list;
}

Statement *
lf_create_if_statement(Expression *condition,
                        Block *then_block, Elsif *elsif_list,
                        Block *else_block)
{
    Statement *st;

    st = alloc_statement(IF_STATEMENT);
    st->u.if_s.condition = condition;
    st->u.if_s.then_block = then_block;
    st->u.if_s.elsif_list = elsif_list;
    st->u.if_s.else_block = else_block;

    return st;
}

Elsif *
lf_chain_elsif_list(Elsif *list, Elsif *add)
{
    Elsif *pos;

    for (pos = list; pos->next; pos = pos->next)
        ;
    pos->next = add;

    return list;
}

Elsif *
lf_create_elsif(Expression *expr, Block *block)
{
    Elsif *ei;

    ei = lf_malloc(sizeof(Elsif));
    ei->condition = expr;
    ei->block = block;
    ei->next = NULL;

    return ei;
}

Statement *
lf_create_while_statement(Expression *condition, Block *block)
{
    Statement *st;

    st = alloc_statement(WHILE_STATEMENT);
    st->u.while_s.condition = condition;
    st->u.while_s.block = block;

    return st;
}

Statement *
lf_create_for_statement(Expression *init, Expression *cond,
                         Expression *post, Block *block)
{
    Statement *st;

    st = alloc_statement(FOR_STATEMENT);
    st->u.for_s.init = init;
    st->u.for_s.condition = cond;
    st->u.for_s.post = post;
    st->u.for_s.block = block;

    return st;
}

Block *
lf_create_block(StatementList *statement_list)
{
    Block *block;

    block = lf_malloc(sizeof(Block));
    block->statement_list = statement_list;

    return block;
}

Statement *
lf_create_expression_statement(Expression *expression)
{
    Statement *st;

    st = alloc_statement(EXPRESSION_STATEMENT);
    st->u.expression_s = expression;

    return st;
}

Statement *
lf_create_return_statement(Expression *expression)
{
    Statement *st;

    st = alloc_statement(RETURN_STATEMENT);
    st->u.return_s.return_value = expression;

    return st;
}

Statement *lf_create_break_statement(void)
{
    return alloc_statement(BREAK_STATEMENT);
}

Statement *lf_create_continue_statement(void)
{
    return alloc_statement(CONTINUE_STATEMENT);
}

