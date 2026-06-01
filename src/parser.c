#include "parser.h"
#include "token.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void parser_init(parser_t *parser, token_t *tokens)
{
    parser->current = tokens;
}

static void parser_advance(parser_t *parser)
{
    parser->current = parser->current->next;
}

static void syntax_error(const char *token)
{
    fprintf(stderr, "minish: syntax error near `%s`\n", token);
}

static int validate_syntax(token_t *tokens)
{
    if (tokens->type == TOKEN_PIPE)
    {
        syntax_error(tokens->value);
        return -1;
    }

    tokens = tokens->next;

    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next)
            {
                syntax_error(tokens->value);
                return -1;
            }

            if (tokens->next && tokens->next->type == TOKEN_PIPE)
            {
                syntax_error(tokens->value);
                return -1;
            }
        }

        tokens = tokens->next;
    }

    return 0;
}
