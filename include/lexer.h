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
 * @brief Initializes the lexer.
 * @param lexer Pointer to a lexer struct.
 * @param input String for lexing.
 */
void lexer_init(lexer_t *lexer, char *input);
/**
 * @brief Advances the lexer
 * @param lexer Pointer to the lexer struct.
 */
void lexer_advance(lexer_t *lexer);
/**
 * @brief Tokenize the input.
 * @param input The input string.
 */
token_t *tokenize(char *input);

#endif // !LEXER_H
