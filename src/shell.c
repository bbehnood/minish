#include "shell.h"
#include "parser.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char **copy_env(char **envp)
{
    int count = 0;
    char **copy;

    while (envp[count])
        count++;

    copy = malloc(sizeof(char *) * (count + 1));
    if (!copy)
    {
        fprintf(stderr, "minish: out of memory\n");
        return NULL;
    }
    for (int i = 0; i < count; i++)
    {
        copy[i] = strdup(envp[i]);
        if (!copy[i])
        {
            fprintf(stderr, "minish: out of memory\n");
            return NULL;
        }
    }

    copy[count] = NULL;
    return copy;
}

static void free_env(char **env)
{
    size_t i = 0;

    if (!env)
        return;

    while (env[i])
    {
        free(env[i]);
        i++;
    }

    free(env);
}

void shell_init(shell_t *shell, char **envp)
{
    shell->running = 1;
    shell->last_status = 0;
    shell->env = copy_env(envp);
    shell->input_line = NULL;
    shell->tokens = NULL;
    shell->cmd = NULL;

    if (!shell->env)
    {
        fprintf(stderr,
                "minish: failed to copy the environment, aborting...\n");
        shell->running = 0;
    }
}

void shell_cleanup(shell_t *shell)
{
    if (shell->tokens)
        free_tokens(shell->tokens);
    if (shell->cmd)
        free_command(shell->cmd);

    free_env(shell->env);
    free(shell->input_line);
}

void shell_reset(shell_t *shell)
{
    free_tokens(shell->tokens);
    shell->tokens = NULL;
    free_command(shell->cmd);
    shell->cmd = NULL;
}
