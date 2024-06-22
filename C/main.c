#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "plotter.h"

int main() {
    char expression[256];
    printf("Enter the expression: ");
    scanf("%255s", expression);

    TokenList *tokens = tokenize(expression);
    if (!tokens) {
        fprintf(stderr, "Invalid expression\n");
        return 1;
    }
    //  for(int i = 0; i < tokens->count; ++i){
    //     printf(" main token %s ", tokens->tokens[i].value);
    // }
    RPNExpression *rpn = to_rpn(tokens);
    for(int i = 0; i < rpn->count; ++i){
        printf(" main %s ", rpn->tokens[i].value);
    }
    if (!rpn) {
        fprintf(stderr, "Failed to convert to RPN\n");
        return 1;
    }
    plot(rpn);
    free_rpn_expression(rpn);
    free_token_list(tokens);
    return 0;
}
