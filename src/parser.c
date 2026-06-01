#include "parser.h"
#include "token.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void parser_init(parser_t *parser, token_t *tokens)
{
    parser->current = tokens;
}

static void parser_advance(parser_t *parser)
{
    parser->current = parser->current->next;
}
