#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "shell.h"
#include "token.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
    shell_t shell;
    size_t len = 0;

    shell_init(&shell, envp);

    while (1)
    {
        printf("minish> ");
        fflush(stdout);

        if (getline(&shell.input_line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        shell.tokens = tokenize(shell.input_line);
        shell.cmd = parse_command(shell.tokens);
        execute_command(shell.cmd);

        shell_reset(&shell);
    }

    shell_cleanup(&shell);

    return EXIT_SUCCESS;
}
