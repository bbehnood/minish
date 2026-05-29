#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC
} token_type_t;

typedef struct token
{
    token_type_t type;
    char *value;
    struct token *next;
} token_t;

/**
 * @brief Creates a new token.
 * @param type Type of the token.
 * @param value The string(value) of the token.
 * @return Pointer to the new token.
 */
token_t *new_token(token_type_t type, char *value);
/**
 * @brief Appends a token to a linked-list of tokens.
 * @param head The head of the linked list.
 * @param new Pointer to the new token.
 */
void append_token(token_t **head, token_t *new);
/**
 * @brief Frees all the allocated memory for tokens.
 * @param head The head of the linked-list.
 */
void free_tokens(token_t *head);

#endif // !TOKEN_H
