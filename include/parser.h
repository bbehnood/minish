#ifndef PARSER_H
#define PARSER_H

#include "shell.h"
#include "token.h"

typedef struct command
{
    char **argv;

    char *infile;
    char *outfile;
    char *heredoc;
    int append;

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
command_t *parse_line(shell_t *shell);
/**
 * @brief Frees a list of commands.
 * @param head Pointer to the head of the commands linked-list.
 */
void free_command_list(command_t *head);

#endif // !PARSER_H
