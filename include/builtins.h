#ifndef BUILTINS_H
#define BUILTINS_H

#include "parser.h"
#include "shell.h"

typedef void (*builtin_fn)(shell_t *shell, char **argv);

typedef struct builtin
{
    const char *name;
    builtin_fn fn;
} builtin_t;

/**
 * @brief Handles builtin commands if given.
 * @param cmd Pointer to the command struct.
 * @return 1 if builtins found and executed, 0 if not.
 */
int handle_builtins(shell_t *shell);

#endif // !BUILTINS_H
