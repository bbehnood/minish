#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "shell.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#define C_RESET "\033[0m"
#define C_GREEN "\033[32m"
#define C_BLUE "\033[34m"

#define PROMPT_SIZE 1024

int main(int argc, char **argv, char **envp)
{
    shell_t shell;
    size_t len = 0;
    char prompt[PROMPT_SIZE];

    shell_init(&shell, envp);

    while (shell.running)
    {
        snprintf(prompt, sizeof(prompt), "%s%s@%s%s:%s%s%s$ ", C_GREEN,
                 shell.username, shell.hostname, C_RESET, C_BLUE, shell.cwd,
                 C_RESET);
        fflush(stdout);

        shell.input_line = readline(prompt);
        if (!shell.input_line)
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
