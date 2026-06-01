#ifndef PARSER_H
#define PARSER_H

#include "shell.h"
#include "token.h"

typedef struct command
{
    char **argv;
    struct command *next;
} command_t;

typedef struct parser
{
    token_t *current;
} parser_t;

/**
 * @brief Parses a line of command.
 * @param shell Pointer to the shell context struct.
 * @return Pointer to the parsed command struct.
 */
command_t *parse_line(shell_t shell);

#endif // !PARSER_H
