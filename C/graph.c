#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "plotter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    const char* expression = "sin(cos(2*x))";
    Token tokens[256];
    int num_tokens = 0;

    tokenize(expression, tokens, &num_tokens);
    TokenList rpn = to_rpn(tokens, num_tokens);
    plot_graph(rpn);

    free(rpn.tokens);
    return 0;
}
