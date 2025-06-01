#include "minishell.h"

void free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i])
        free(tokens[i++]);
    free(tokens);
}
void print_commands(t_command *cmd)
{
    int idx = 0;
    while (cmd)
    {
        printf("------- Command #%d -------\n", idx++);
        printf("cmd: %s\n", cmd->cmd);

        int i = 0;
        while (cmd->args[i])
        {
            printf("  arg[%d]: %s\n", i, cmd->args[i]);
            i++;
        }

        if (cmd->infile)
        {
            if (cmd->heredoc)
                printf("  infile (heredoc): %s\n", cmd->infile);
            else
                printf("  infile: %s\n", cmd->infile);
        }
        if (cmd->outfile)
        {
            if (cmd->append)
                printf("  outfile (append): %s\n", cmd->outfile);
            else
                printf("  outfile (truncate): %s\n", cmd->outfile);
        }
        cmd = cmd->next;
    }
}

void free_commands(t_command *cmd)
{
    t_command *next;
    int       i;

    while (cmd)
    {
        next = cmd->next;
        i = 0;
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        if (cmd->cmd)
            free(cmd->cmd);
        free(cmd);
        cmd = next;
    }
}

int main(void)
{
    char        *raw;
    char        **tokens;
    t_command   *cmds;

    while (1)
    {
        raw = get_input();
        if (!raw)
            break;
        if (is_only_whitespace(raw))
        {
            free(raw);
            continue;
        }

        char *expanded = expand_variables(raw);
        free(raw);
        if (!expanded)
            continue;

        tokens = lexer(expanded);
        free(expanded);
        if (!tokens)
            continue;

        if (check_syntax_errors(tokens))
        {
            free_tokens(tokens);
            continue;
        }

        cmds = parse_tokens(tokens);
        if (cmds)
        {
            print_commands(cmds);
            free_commands(cmds);
        }

        free_tokens(tokens);
    }
    return (0);
}
