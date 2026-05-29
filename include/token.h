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

token_t *new_token(token_type_t type, char *value);
void append_token(token_t **head, token_t *new);
void free_tokens(token_t *head);

#endif // !TOKEN_H
