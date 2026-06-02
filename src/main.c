#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "shell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define C_RESET "\033[0m"
#define C_GREEN "\033[32m"
#define C_BLUE "\033[34m"

int main(int argc, char **argv, char **envp)
{
    shell_t shell;
    size_t len = 0;

    shell_init(&shell, envp);

    while (shell.running)
    {
        printf("%s%s@%s%s:%s%s%s$ ", C_GREEN, shell.username, shell.hostname,
               C_RESET, C_BLUE, shell.cwd, C_RESET);
        fflush(stdout);

        if (getline(&shell.input_line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        shell.tokens = tokenize(&shell);
        if (shell.tokens)
        {
            shell.cmd = parse_line(&shell);
            if (shell.cmd)
            {
                execute_command(&shell);
            }
        }

        shell_reset(&shell);
    }

    shell_cleanup(&shell);

    return EXIT_SUCCESS;
}
