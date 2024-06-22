#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef enum { TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_FUNCTION, TOKEN_VARIABLE, TOKEN_LPAREN, TOKEN_RPAREN } TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct {
    Token *tokens;
    size_t count;
    size_t capacity;
} TokenList;

TokenList *tokenize(const char *expression);
void free_token_list(TokenList *list);

#endif
