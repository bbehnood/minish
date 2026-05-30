#include "exec.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    token_t *tokens;
    command_t *cmd;

    while (1)
    {
        printf("minish> ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        tokens = tokenize(line);
        print_tokens(tokens);
        cmd = parse_command(tokens);
        print_command(cmd);
        free_tokens(tokens);
        free_command(cmd);
    }

    free(line);

    return EXIT_SUCCESS;
}
