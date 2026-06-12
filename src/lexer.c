#include "lexer.h"
#include "shell.h"
#include "token.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int is_operator(char c)
{
    return c == '|' || c == '>' || c == '<';
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

static token_t *handle_redirection(lexer_t *lexer)
{
    token_t *new = NULL;

    if (lexer->current == '>')
    {
        lexer_advance(lexer);

        if (lexer->current == '>')
        {
            lexer_advance(lexer);
            new = new_token(TOKEN_APPEND, NULL);
            if (!new)
                return NULL;
        }
        else
        {
            new = new_token(TOKEN_REDIR_OUT, NULL);
            if (!new)
                return NULL;
        }
    }
    else if (lexer->current == '<')
    {
        lexer_advance(lexer);

        if (lexer->current == '<')
        {
            lexer_advance(lexer);
            new = new_token(TOKEN_HEREDOC, NULL);
            if (!new)
                return NULL;
        }
        else
        {
            new = new_token(TOKEN_REDIR_IN, NULL);
            if (!new)
                return NULL;
        }
    }

    return new;
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
            token_t *new = new_token(TOKEN_PIPE, NULL);
            if (!new)
                return NULL;

            append_token(&tokens, new);
            lexer_advance(&lexer);
        }
        else if (lexer.current == '>' || lexer.current == '<')
        {
            token_t *new = handle_redirection(&lexer);
            if (!new)
                return NULL;

            append_token(&tokens, new);
        }
        else
        {
            word = collect_word(&lexer);

            token_t *new = new_token(TOKEN_WORD, word);
            if (!new || !word)
                return NULL;

            append_token(&tokens, new);

            free(word);
        }
    }

    return tokens;
}
