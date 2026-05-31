#ifndef UTILS_H
#define UTILS_H

#include "parser.h"
#include "token.h"

/**
 * @brief Prints the elements of a linked-list of tokens.
 * @param head The head of the linked-list.
 */
void print_tokens(token_t *head);

/**
 * @brief Prints a command.
 * @param cmd Pointer to command struct
 */
void print_command(command_t *cmd);

/**
 * @brief Copies the environment variables.
 * @param envp The list of environment variables provided by the OS.
 * @return A copy of the environment variables.
 */
char **copy_env(char **envp);

#endif // !UTILS_H
