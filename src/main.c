#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "shell.h"
#include "utils.h"
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define C_RESET "\033[0m"
#define C_GREEN "\033[32m"
#define C_BLUE "\033[34m"

#define PROMPT_SIZE 1024
#define HISTORY_SIZE 1000

int main(int argc, char **argv, char **envp)
{
    shell_t shell;
    size_t len = 0;
    char prompt[PROMPT_SIZE];
    char history_file[PATH_MAX];

    shell_init(&shell, envp);

    snprintf(history_file, sizeof(history_file), "%s/.minish_history",
             env_get(&shell, "HOME"));

    using_history();
    read_history(history_file);
    stifle_history(HISTORY_SIZE);

    while (shell.running)
    {
        snprintf(prompt, sizeof(prompt), "%s%s@%s%s:%s%s%s$ ", C_GREEN,
                 shell.username, shell.hostname, C_RESET, C_BLUE, shell.cwd,
                 C_RESET);

        shell.input_line = readline(prompt);
        if (!shell.input_line)
        {
            printf("\n");
            break;
        }

        if (*shell.input_line)
        {
            add_history(shell.input_line);
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

    write_history(history_file);

    shell_cleanup(&shell);

    return EXIT_SUCCESS;
}
