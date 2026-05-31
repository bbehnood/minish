#ifndef EXEC_H
#define EXEC_H

#include "parser.h"

/**
 * @brief Executes a command.
 * @param cmd Pointer to the command struct.
 */
void execute_command(command_t *cmd);

#endif // !EXEC_H
