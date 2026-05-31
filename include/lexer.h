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

/**
 * @brief Tokenize the input.
 * @param input The input string.
 */
token_t *tokenize(char *input);

#endif // !LEXER_H
