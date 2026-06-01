#include "lexer.h"
#include "shell.h"
#include "token.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int is_operator(char c)
{
    return c == '|';
}

static void lexer_init(lexer_t *lexer, const char *input)
{
    lexer->input = input;
    lexer->pos = 0;
    lexer->current = input[0];
}

static void lexer_advance(lexer_t *lexer)
{
    if (lexer->current != '\0')
    {
        lexer->pos++;
        lexer->current = lexer->input[lexer->pos];
    }
}

static void skip_whitespaces(lexer_t *lexer)
{
    while (lexer->current && isspace(lexer->current))
        lexer_advance(lexer);
}

static char *collect_word(lexer_t *lexer)
{
    size_t start;
    size_t len;
    char *word;

    start = lexer->pos;

    while (lexer->current && !isspace(lexer->current) &&
           !is_operator(lexer->current))
    {
        lexer_advance(lexer);
    }

    len = lexer->pos - start;

    word = (char *)malloc(len + 1);
    if (!word)
        return NULL;

    memcpy(word, lexer->input + start, len);
    word[len] = '\0';

    return word;
}

token_t *tokenize(shell_t *shell)
{
    lexer_t lexer;
    token_t *tokens;
    char *word;

    tokens = NULL;

    lexer_init(&lexer, shell->input_line);

    while (lexer.current)
    {
        skip_whitespaces(&lexer);

        if (!lexer.current)
            break;

        if (lexer.current == '|')
        {
            append_token(&tokens, new_token(TOKEN_PIPE, NULL));
            lexer_advance(&lexer);
        }
        else
        {
            word = collect_word(&lexer);

            append_token(&tokens, new_token(TOKEN_WORD, word));

            free(word);
        }
    }

    return tokens;
}
