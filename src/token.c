#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t *new_token(token_type_t type, const char *value)
{
    token_t *token = (token_t *)malloc(sizeof(token_t));
    if (!token)
    {
        fprintf(stderr, "minish: out of memory\n");
        return NULL;
    }

    token->type = type;

    if (value)
        token->value = strdup(value);
    else
        token->value = NULL;

    token->next = NULL;

    return token;
}

void append_token(token_t **head, token_t *new)
{
    token_t *tmp;

    if (!*head)
    {
        *head = new;
        return;
    }

    tmp = *head;

    while (tmp->next)
        tmp = tmp->next;

    tmp->next = new;
}

void free_tokens(token_t *head)
{
    token_t *tmp;

    while (head)
    {
        tmp = head->next;

        free(head->value);
        free(head);

        head = tmp;
    }
}
