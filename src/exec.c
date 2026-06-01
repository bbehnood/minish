#include "exec.h"
#include "builtins.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_command(shell_t *shell)
{
    pid_t pid;
    int status = 0;

    if (!shell->cmd)
    {
        return;
    }

    if (handle_builtins(shell))
    {
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        execvp(shell->cmd->argv[0], shell->cmd->argv);

        perror(shell->cmd->argv[0]);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}
