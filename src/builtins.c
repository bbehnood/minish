#include "builtins.h"
#include "shell.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void builtin_cd(shell_t *shell, char **argv);
static void builtin_exit(shell_t *shell, char **argv);
static void builtin_pwd(shell_t *shell, char **argv);

builtin_t builtins[] = {
    {"cd", builtin_cd},
    {"exit", builtin_exit},
    {"pwd", builtin_pwd},
    {NULL, NULL},
};

int handle_builtins(shell_t *shell)
{
    int i = 0;

    while (builtins[i].name)
    {
        if (strcmp(shell->cmd->argv[0], builtins[i].name) == 0)
        {
            builtins[i].fn(shell, shell->cmd->argv);
            return 1;
        }

        i++;
    }

    return 0;
}

static void builtin_cd(shell_t *shell, char **argv)
{
    char *path = NULL;

    if (argv[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    if (!argv[1])
    {
        path = env_get(shell, "HOME");
    }
    else
    {
        path = argv[1];
    }

    if (chdir(path) == -1)
    {
        perror("cd");
        shell->last_status = 1;
        return;
    }

    shell->last_status = 0;
}

static void builtin_exit(shell_t *shell, char **argv)
{
    shell_cleanup(shell);
    exit(EXIT_SUCCESS);
}

static void builtin_pwd(shell_t *shell, char **argv)
{
    char *cwd;

    if (argv[1])
    {
        fprintf(stderr, "pwd: too many arguments\n");
        shell->last_status = 1;
        return;
    }

    cwd = getcwd(NULL, 0);

    if (cwd == NULL)
    {
        perror("pwd");
        return;
    }

    printf("%s\n", cwd);
    free(cwd);
}
