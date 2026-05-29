#include "exec.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        printf("minish> ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        char **args = parse_line(line);
        execute_command(args);
    }

    free(line);

    return EXIT_SUCCESS;
}
