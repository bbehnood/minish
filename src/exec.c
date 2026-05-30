#include "exec.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_command(command_t *cmd)
{
    pid_t pid;
    int status = 0;

    if (!cmd)
    {
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        execvp(cmd->argv[0], cmd->argv);

        perror("execvp");
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
