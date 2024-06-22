#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
    Token *tokens;
    size_t count;
} RPNExpression;

RPNExpression *to_rpn(const TokenList *tokens);
double evaluate_rpn(const RPNExpression *rpn, double x);
void free_rpn_expression(RPNExpression *rpn);

#endif
