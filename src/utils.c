#include "utils.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>

void print_tokens(token_t *head)
{
    while (head)
    {
        if (head->type == TOKEN_WORD)
            printf("[WORD: %s]\n", head->value);
        else if (head->type == TOKEN_PIPE)
            printf("[PIPE]\n");

        head = head->next;
    }
}

void print_command(command_t *cmd)
{
    size_t i = 0;

    if (!cmd)
    {
        printf("NULL command\n");
        return;
    }

    printf("COMMAND:\n");

    while (cmd->argv[i])
    {
        printf("  argv[%zu] = \"%s\"\n", i, cmd->argv[i]);
        i++;
    }
}
