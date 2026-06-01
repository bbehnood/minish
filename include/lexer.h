#ifndef LEXER_H
#define LEXER_H

#include "shell.h"
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
 * @param shell Pointer to the shell struct.
 */
token_t *tokenize(shell_t *shell);

#endif // !LEXER_H
