#include "exec.h"
#include "builtins.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

static void wait_child(shell_t *shell, pid_t pid)
{
    int status;

    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
        shell->last_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->last_status = 128 + WTERMSIG(status);
    else
        shell->last_status = 1;
}

static void wait_children(shell_t *shell, pid_t *pids, int count)
{
    int status;
    int i;

    for (i = 0; i < count; i++)
    {
        waitpid(pids[i], &status, 0);

        if (i == count - 1)
        {
            if (WIFEXITED(status))
                shell->last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->last_status = 128 + WTERMSIG(status);
            else
                shell->last_status = 1;
        }
    }
}

static int command_count(command_t *cmd)
{
    int count;

    count = 0;

    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }

    return count;
}

static int command_is_last(command_t *cmd)
{
    return cmd->next == NULL;
}

static void apply_redirections(command_t *cmd)
{
    int fd;

    if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd < 0)
        {
            perror(cmd->infile);
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->outfile)
    {
        if (cmd->append)
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0)
        {
            perror(cmd->outfile);
            exit(EXIT_FAILURE);
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    if (cmd->heredoc)
    {
        // TODO: Implement full heredoc
        fprintf(stderr, "heredoc not implemented yet: %s\n", cmd->heredoc);
        exit(EXIT_FAILURE);
    }
}

static pid_t spawn_command(command_t *cmd, int input_fd, int output_fd)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_fd != STDOUT_FILENO)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        apply_redirections(cmd);

        execvp(cmd->argv[0], cmd->argv);

        perror(cmd->argv[0]);
        exit(127);
    }

    return pid;
}

static void execute_single(shell_t *shell)
{
    pid_t pid;

    if (handle_builtins(shell))
        return;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        shell->last_status = 1;
        return;
    }

    if (pid == 0)
    {
        apply_redirections(shell->cmd);

        execvp(shell->cmd->argv[0], shell->cmd->argv);

        perror(shell->cmd->argv[0]);
        exit(127);
    }

    wait_child(shell, pid);
}

static void execute_pipeline(shell_t *shell)
{
    command_t *cmd;
    pid_t *pids;

    int prev_read;
    int pipefd[2];

    int count;
    int i;

    count = command_count(shell->cmd);

    pids = malloc(sizeof(pid_t) * count);

    if (!pids)
    {
        perror("malloc");
        shell->last_status = 1;
        return;
    }

    prev_read = STDIN_FILENO;
    cmd = shell->cmd;
    i = 0;

    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                free(pids);
                shell->last_status = 1;
                return;
            }
        }
        else
        {
            pipefd[0] = -1;
            pipefd[1] = STDOUT_FILENO;
        }

        pids[i] = spawn_command(cmd, prev_read, pipefd[1]);
        if (pids[i] == -1)
        {
            free(pids);
            shell->last_status = 1;
            return;
        }

        i++;

        if (prev_read != STDIN_FILENO)
            close(prev_read);

        if (cmd->next)
            close(pipefd[1]);

        prev_read = pipefd[0];

        cmd = cmd->next;
    }

    wait_children(shell, pids, count);

    free(pids);
}

void execute_command(shell_t *shell)
{
    if (!command_is_last(shell->cmd))
        execute_pipeline(shell);
    else
        execute_single(shell);
}
