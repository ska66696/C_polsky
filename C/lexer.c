#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 20

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

TokenList *tokenize(const char *expression) {
    TokenList *list = (TokenList *)malloc(sizeof(TokenList));
    if (!list) return NULL;

    list->tokens = (Token *)malloc(INITIAL_CAPACITY * sizeof(Token));
    if (!list->tokens) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
    const char *p = expression;

    while (*p) {
        if (is_space(*p)) {
            p++;
            continue;
        }

        if (is_digit(*p) || (*p == '.' && is_digit(*(p + 1)))) {
            const char *start = p;
            while (is_digit(*p) || *p == '.') p++;
            size_t length = p - start;
            char *value = (char *)malloc((length + 1) * sizeof(char));
            strncpy(value, start, length);
            value[length] = '\0';
            list->tokens[list->count++] = (Token){TOKEN_NUMBER, value};
        } else if (is_alpha(*p)) {
            const char *start = p;
            while (is_alpha(*p)) p++;
            size_t length = p - start;
            char *value = (char *)malloc((length + 1) * sizeof(char));
            strncpy(value, start, length);
            value[length] = '\0';
            TokenType type = strcmp(value, "x") == 0 ? TOKEN_VARIABLE : TOKEN_FUNCTION;
            list->tokens[list->count++] = (Token){type, value};
        } else if (strchr("+-*/()", *p)) {
            char *value = (char *)malloc(2 * sizeof(char));
            value[0] = *p;
            value[1] = '\0';
            TokenType type = *p == '(' ? TOKEN_LPAREN : (*p == ')' ? TOKEN_RPAREN : TOKEN_OPERATOR);
            list->tokens[list->count++] = (Token){type, value};
            p++;
        } else {
            free_token_list(list);
            return NULL;
        }
    }

    return list;
}

void free_token_list(TokenList *list) {
    if (!list) return;
    for (size_t i = 0; i < list->count; i++) free(list->tokens[i].value);
    free(list->tokens);
    free(list);
}
