#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>

typedef struct lexer
{
    char *input;
    size_t pos;
    char current;
} lexer_t;

void lexer_init(lexer_t *lexer, char *input);
void lexer_advance(lexer_t *lexer);
token_t *tokenize(char *input);

#endif // !LEXER_H
