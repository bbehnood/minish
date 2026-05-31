#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>

typedef struct lexer
{
    const char *input;
    size_t pos;
    char current;
} lexer_t;

/**
 * @brief Tokenizes the input.
 * @param input The input string.
 */
token_t *tokenize(const char *input);

#endif // !LEXER_H
