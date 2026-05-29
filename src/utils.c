#include "utils.h"

void print_tokens(token_t *head)
{
    while (head)
    {
        if (head->type == TOKEN_WORD)
            printf("[WORD: %s]\n", head->value);
        else if (head->type == TOKEN_PIPE)
            printf("[PIPE]\n");

        head = head->next;
    }
}
