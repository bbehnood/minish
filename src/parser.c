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

static int count_words(token_t *tokens)
{
    int count = 0;

    while (tokens && tokens->type == TOKEN_WORD)
    {
        tokens = tokens->next;
        count++;
    }

    return count;
}

static command_t *parse_command(parser_t *parser)
{
    command_t *cmd;
    int argc;
    int i;

    argc = count_words(parser->current);

    if (argc == 0)
        return NULL;

    cmd = (command_t *)malloc(sizeof(command_t));
    if (!cmd)
    {
        fprintf(stderr, "minish: out of memory\n");
        return NULL;
    }

    cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
    if (!cmd->argv)
    {
        fprintf(stderr, "minish: out of memory\n");
        free(cmd);
        return NULL;
    }

    i = 0;

    while (parser->current && parser->current->type == TOKEN_WORD)
    {
        cmd->argv[i] = strdup(parser->current->value);
        if (!cmd->argv[i])
        {
            while (i > 0)
            {
                i--;
                free(cmd->argv[i]);
                return NULL;
            }
        }

        i++;
        parser_advance(parser);
    }

    cmd->argv[i] = NULL;

    return cmd;
}
