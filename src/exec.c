#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_command(char **args)
{
    if (!args || !args[0])
        return;

    pid_t pid = fork();
    int status = 0;

    if (pid == 0)
    {
        execvp(args[0], args);

        perror("exec");
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
