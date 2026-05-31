#include "parser.h"
#include "token.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static size_t count_tokens(token_t *tokens)
{
    size_t count = 0;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD)
            count++;
        else
            break;

        tokens = tokens->next;
    }

    return count;
}

command_t *parse_command(token_t *tokens)
{
    command_t *cmd = NULL;
    size_t argc = 0;
    size_t i = 0;

    argc = count_tokens(tokens);

    if (argc == 0)
        return NULL;

    cmd = (command_t *)malloc(sizeof(command_t));
    if (!cmd)
        return NULL;

    cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
    if (!cmd->argv)
    {
        free(cmd);
        return NULL;
    }

    while (tokens && tokens->type == TOKEN_WORD)
    {
        cmd->argv[i] = strdup(tokens->value);
        if (!cmd->argv[i])
        {
            while (i > 0)
            {
                i--;
                free(cmd->argv[i]);
            }

            free(cmd->argv);
            free(cmd);
            return NULL;
        }
        tokens = tokens->next;
        i++;
    }

    cmd->argv[i] = NULL;

    return cmd;
}

void free_command(command_t *cmd)
{
    size_t i = 0;

    if (!cmd)
        return;

    if (cmd->argv)
    {
        while (cmd->argv[i])
        {
            free(cmd->argv[i]);
            i++;
        }

        free(cmd->argv);
    }

    free(cmd);
}
