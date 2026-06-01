#ifndef PARSER_H
#define PARSER_H

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
 * @brief Parses a linked-list of tokens.
 * @param line The command String.
 * @return Pointer to a command struct.
 */
command_t *parse_command(token_t *tokens);
/**
 * @brief Frees the allocated memory for a command.
 * @param cmd Pointer to the command struct.
 */
void free_command(command_t *cmd);

#endif // !PARSER_H
