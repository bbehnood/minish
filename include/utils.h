#ifndef UTILS_H
#define UTILS_H

#include "parser.h"
#include "shell.h"
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
 * @brief Prints a pipeline.
 * @param head Head of the pipeline linked-list.
 */
void print_pipeline(command_t *head);
/**
 * @brief Copies the environment variables.
 * @param envp The list of environment variables provided by the OS.
 * @return A copy of the environment variables.
 */
char **copy_env(char **envp);
/**
 * @brief Gets a value from the environment variables.
 * @param shell Pointer to the shell struct.
 * @param key The environment variable key to search for.
 * @return The value of the variable.
 */
char *env_get(shell_t *shell, const char *key);
/**
 * @brief Sets a value to an environment variable.
 * @param shell Pointer to the shell struct.
 * @param key The environment variable's key.
 * @param value The environment variable's value.
 * @return 0 on success. -1 if there was an error.
 */
int env_set(shell_t *shell, const char *key, const char *value);

#endif // !UTILS_H
