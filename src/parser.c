#include "parser.h"
#include <string.h>

#define MAX_TOKENS 64

char **parse_line(char *line)
{
    static char *tokens[MAX_TOKENS];
    int position = 0;

    char *token = strtok(line, " \t\n");

    while (token && position < MAX_TOKENS)
    {
        tokens[position++] = token;
        token = strtok(NULL, " \t\n");
    }

    tokens[position] = NULL;

    return tokens;
}
