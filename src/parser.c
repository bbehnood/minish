#include "parser.h"
#include "shell.h"
#include "token.h"
#include <stddef.h>
#include <stdio.h>
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

static void syntax_error(const char *token)
{
    fprintf(stderr, "minish: syntax error near `%s`\n", token);
}

static int is_redirection(token_type_t type)
{
    return type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
           type == TOKEN_APPEND || type == TOKEN_HEREDOC;
}

static int validate_syntax(token_t *tokens)
{
    if (tokens->type == TOKEN_PIPE)
    {
        syntax_error(tokens->value);
        return -1;
    }

    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next)
            {
                syntax_error(tokens->value);
                return -1;
            }

            if (tokens->next->type == TOKEN_PIPE)
            {
                syntax_error(tokens->value);
                return -1;
            }
        }
        else if (is_redirection(tokens->type))
        {
            if (!tokens->next)
            {
                syntax_error(tokens->value);
                return -1;
            }

            if (tokens->next->type == TOKEN_PIPE ||
                is_redirection(tokens->next->type))
            {
                syntax_error(tokens->value);
                return -1;
            }
        }

        tokens = tokens->next;
    }

    return 0;
}

static command_t *command_new(void)
{
    command_t *cmd = (command_t *)malloc(sizeof(command_t));
    if (!cmd)
    {
        fprintf(stderr, "minish: out of memory\n");
        return NULL;
    }

    cmd->argv = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->heredoc = NULL;
    cmd->append = 0;
    cmd->next = NULL;

    return cmd;
}

static command_t *command_last(command_t *head)
{
    while (head->next)
    {
        head = head->next;
    }

    return head;
}

static void command_append(command_t **head, command_t *new)
{
    command_t *last_command;

    if (!*head)
    {
        *head = new;
        return;
    }

    last_command = command_last(*head);

    last_command->next = new;
}

static void free_command(command_t *cmd)
{
    int i = 0;

    while (cmd->argv[i])
    {
        free(cmd->argv[i]);
        i++;
    }

    free(cmd->argv);
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd->heredoc);
    free(cmd);
}

void free_command_list(command_t *head)
{
    command_t *next;

    while (head)
    {
        next = head->next;
        free_command(head);
        head = next;
    }
}

static int parse_redirection(parser_t *parser, command_t *cmd)
{
    token_t *t = parser->current;

    if (!t || !is_redirection(t->type))
        return 0;

    token_type_t type = t->type;
    parser_advance(parser);

    if (!parser->current || parser->current->type != TOKEN_WORD)
    {
        syntax_error("newline");
        return -1;
    }

    char *filename = parser->current->value;
    parser_advance(parser);

    if (type == TOKEN_REDIR_IN)
        cmd->infile = strdup(filename);

    else if (type == TOKEN_REDIR_OUT)
    {
        cmd->outfile = strdup(filename);
        cmd->append = 0;
    }
    else if (type == TOKEN_APPEND)
    {
        cmd->outfile = strdup(filename);
        cmd->append = 1;
    }
    else if (type == TOKEN_HEREDOC)
    {
        cmd->heredoc = strdup(filename);
    }

    return 0;
}

static command_t *parse_command(parser_t *parser)
{
    command_t *cmd;
    int argc = 0;
    token_t *tmp;

    tmp = parser->current;

    while (tmp && tmp->type != TOKEN_PIPE)
    {
        if (tmp->type == TOKEN_WORD)
            argc++;

        tmp = tmp->next;
    }

    if (argc == 0)
        return NULL;

    cmd = command_new();
    if (!cmd)
        return NULL;

    cmd->argv = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->argv)
    {
        free(cmd);
        return NULL;
    }

    int i = 0;

    while (parser->current && parser->current->type != TOKEN_PIPE)
    {
        if (parser->current->type == TOKEN_WORD)
        {
            cmd->argv[i++] = strdup(parser->current->value);
            parser_advance(parser);
        }
        else if (is_redirection(parser->current->type))
        {
            if (parse_redirection(parser, cmd) == -1)
            {
                free_command(cmd);
                return NULL;
            }
        }
        else
        {
            break;
        }
    }

    cmd->argv[i] = NULL;
    return cmd;
}

static command_t *parse_pipeline(parser_t *parser)
{
    command_t *pipeline = NULL;
    command_t *cmd;

    while (1)
    {
        cmd = parse_command(parser);
        if (!cmd)
            break;

        command_append(&pipeline, cmd);

        if (!parser->current || parser->current->type != TOKEN_PIPE)
            break;

        parser_advance(parser);
    }

    return pipeline;
}

command_t *parse_line(shell_t *shell)
{
    parser_t parser;
    command_t *pipeline;

    if (validate_syntax(shell->tokens) == -1)
        return NULL;

    parser_init(&parser, shell->tokens);

    pipeline = parse_pipeline(&parser);

    return pipeline;
}
