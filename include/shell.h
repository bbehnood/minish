#ifndef SHELL_H
#define SHELL_H

#include "parser.h"
#include "token.h"

typedef struct
{
    int running;
    int last_status;

    char **env;
    char *input_line;

    token_t *tokens;
    command_t *cmd;
} shell_t;

/**
 * @brief Initializes the shell context.
 * @param shell Pointer to the shell context struct.
 */
void shell_init(shell_t *shell, char **envp);
/**
 * @brief Performs the pre-shutdown cleanup.
 * @param shell Pointer to the shell context struct.
 */
void shell_cleanup(shell_t *shell);
/**
 * @brief Resets the per-iteration properties of the shell.
 * @param shell Pointer to the shell context struct.
 */
void shell_reset(shell_t *shell);

#endif // !SHELL_H
