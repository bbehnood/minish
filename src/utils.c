#include "utils.h"
#include "parser.h"
#include "shell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *env_get(shell_t *shell, const char *key)
{
    size_t key_len;
    int i;

    key_len = strlen(key);

    i = 0;
    while (shell->env[i])
    {
        if (strncmp(shell->env[i], key, key_len) == 0 &&
            shell->env[i][key_len] == '=')
        {
            return shell->env[i] + key_len + 1;
        }

        i++;
    }

    return NULL;
}

int env_set(shell_t *shell, const char *key, const char *value)
{
    size_t key_len;
    size_t env_count;
    char *new_entry;
    char **new_env;
    size_t i;

    if (!shell || !key || !value)
        return -1;

    key_len = strlen(key);

    new_entry = malloc(key_len + strlen(value) + 2);
    if (!new_entry)
        return -1;

    sprintf(new_entry, "%s=%s", key, value);

    for (i = 0; shell->env[i]; i++)
    {
        if (strncmp(shell->env[i], key, key_len) == 0 &&
            shell->env[i][key_len] == '=')
        {
            free(shell->env[i]);
            shell->env[i] = new_entry;
            return 0;
        }
    }

    env_count = i;

    new_env = realloc(shell->env, sizeof(char *) * (env_count + 2));
    if (!new_env)
    {
        free(new_entry);
        return -1;
    }

    shell->env = new_env;
    shell->env[env_count] = new_entry;
    shell->env[env_count + 1] = NULL;

    return 0;
}

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

void print_head(command_t *head)
{
    int cmd_index = 0;

    if (!head)
    {
        printf("NULL head\n");
        return;
    }

    while (head)
    {
        printf("COMMAND %d:\n", cmd_index);

        for (int i = 0; head->argv[i]; i++)
            printf("  argv[%d] = \"%s\"\n", i, head->argv[i]);

        head = head->next;

        if (head)
            printf("  |\n");

        cmd_index++;
    }
}

void print_pipeline(command_t *pipeline)
{
    int cmd_index = 0;

    if (!pipeline)
    {
        printf("NULL pipeline\n");
        return;
    }

    while (pipeline)
    {
        printf("COMMAND %d:\n", cmd_index);

        for (int i = 0; pipeline->argv[i]; i++)
            printf("  argv[%d] = \"%s\"\n", i, pipeline->argv[i]);

        pipeline = pipeline->next;

        if (pipeline)
            printf("  |\n");

        cmd_index++;
    }
}
