#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define INITIAL_CAPACITY 10

typedef struct {
    void **items;
    int top;
    int capacity;
} Stack;

Stack *create_stack() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->items = (void **)malloc(INITIAL_CAPACITY * sizeof(void *));
    stack->top = -1;
    stack->capacity = INITIAL_CAPACITY;
    return stack;
}

void push(Stack *stack, void *item) {
    if (stack->top == stack->capacity - 1) {
        stack->capacity *= 2;
        stack->items = (void **)realloc(stack->items, stack->capacity * sizeof(void *));
    }
    stack->items[++stack->top] = item;
}

void *pop(Stack *stack) {
    if (stack->top == -1) return NULL;
    return stack->items[stack->top--];
}

void *top(const Stack *stack) {
    if (stack->top == -1) return NULL;
    return stack->items[stack->top];
}

void free_stack(Stack *stack) {
    free(stack->items);
    free(stack);
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
}

int is_operator(const char *token) {
    return strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 || strcmp(token, "/") == 0;
}

RPNExpression *to_rpn(const TokenList *tokens) {
    Stack *operator_stack = create_stack();
    Token *output = (Token *)malloc(tokens->count * sizeof(Token));
    size_t output_count = 0;

    for (size_t i = 0; i < tokens->count; i++) {
        Token token = tokens->tokens[i];
        printf("i: %d val: %s\n", i, tokens->tokens[i].value);
        if (token.type == TOKEN_NUMBER || token.type == TOKEN_VARIABLE) {
            output[output_count++] = token;
        } else if (token.type == TOKEN_FUNCTION) {
            push(operator_stack, &tokens->tokens[i]);
            //printf("i: %d val: %s\n", i, tokens->tokens[i].value);
        } else if (token.type == TOKEN_OPERATOR) {
            while (top(operator_stack) && ((Token *)top(operator_stack))->type == TOKEN_OPERATOR &&
                   precedence(((Token *)top(operator_stack))->value[0]) >= precedence(token.value[0])) {
                output[output_count++] = *((Token *)pop(operator_stack));
            }
            push(operator_stack, &tokens->tokens[i]);
        } else if (token.type == TOKEN_LPAREN) {
            push(operator_stack, &tokens->tokens[i]);
        } else if (token.type == TOKEN_RPAREN) {
            while (top(operator_stack) && ((Token *)top(operator_stack))->type != TOKEN_LPAREN) {
                output[output_count++] = *((Token *)pop(operator_stack));
            }
            pop(operator_stack);
            if (top(operator_stack) && ((Token *)top(operator_stack))->type == TOKEN_FUNCTION) {
                output[output_count++] = *((Token *)pop(operator_stack));
            }
        }
    }

    while (top(operator_stack)) {
        output[output_count++] = *((Token *)pop(operator_stack));
    }
    for(int i = 0; i < output_count; ++i){
        printf(" %s ", output[i].value);
    }
    RPNExpression *rpn = (RPNExpression *)malloc(sizeof(RPNExpression));
    rpn->tokens = output;
    rpn->count = output_count;

    free_stack(operator_stack);
    // for(int i = 0; i < output_count; ++i){
    //     printf(" %s ", rpn->tokens[i].value);
    // }
    return rpn;
}

double evaluate_rpn(const RPNExpression *rpn, double x) {
    Stack *value_stack = create_stack();

    for (size_t i = 0; i < rpn->count; i++) {
        Token token = rpn->tokens[i];
        //printf("val: %s \n", rpn->tokens[i].value);
        if (token.type == TOKEN_NUMBER) {
            double value = atof(token.value);
            push(value_stack, &value);
        } else if (token.type == TOKEN_VARIABLE) {
            push(value_stack, &x);
        } else if (token.type == TOKEN_FUNCTION) {
            double arg = *((double *)pop(value_stack));
            double result = 0;
            if (strcmp(token.value, "sin") == 0) result = sin(arg);
            else if (strcmp(token.value, "cos") == 0) result = cos(arg);
            else if (strcmp(token.value, "tan") == 0) result = tan(arg);
            else if (strcmp(token.value, "ctg") == 0) result = 1.0 / tan(arg);
            else if (strcmp(token.value, "sqrt") == 0) result = sqrt(arg);
            else if (strcmp(token.value, "ln") == 0) result = log(arg);
            push(value_stack, &result);
        } else if (token.type == TOKEN_OPERATOR) {
            double a = 0, b;
            //printf("b: %lf \n", top(value_stack));
            printf("top1: %d ", value_stack->top);
            b = *((double *)pop(value_stack));
            printf("b: %lf \n", b);
            printf("top2: %d ", value_stack->top);
            if(value_stack->top != -1) a = *((double *)pop(value_stack));
            printf("a: %lf \n", a);
            
            double result = 0;
            if (strcmp(token.value, "+") == 0) result = a + b;
            else if (strcmp(token.value, "-") == 0 && !a) result = a - b;
            else if (strcmp(token.value, "-") == 0 && value_stack->top == -1) result = -b;
            else if (strcmp(token.value, "*") == 0) result = a * b;
            else if (strcmp(token.value, "/") == 0) result = a / b;
            push(value_stack, &result);
            printf("\nres: %lf ", result);
        }
    }
    printf("\n================\n");
    double result = *((double *)pop(value_stack));
    free_stack(value_stack);
    return result;
}

void free_rpn_expression(RPNExpression *rpn) {
    for (size_t i = 0; i < rpn->count; i++) free(rpn->tokens[i].value);
    free(rpn->tokens);
    free(rpn);
}
