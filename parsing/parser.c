#include "../minishell.h"

int count_args(char **tokens, int start)
{
    int count = 0;
    while (tokens[start]
        && tokens[start][0] != '|'
        && tokens[start][0] != '<'
        && tokens[start][0] != '>')
    {
        count++;
        start++;
    }
    return (count);
}
t_command *parse_tokens(char **tokens)
{
    t_command   *head = NULL;
    t_command   *current = NULL;
    int         i = 0;

    while (tokens[i])
    {
        t_command *cmd = calloc(1, sizeof(t_command));
        if (!cmd)
            return (NULL);

        int argc = count_args(tokens, i);
        cmd->args = malloc(sizeof(char *) * (argc + 1));
        if (!cmd->args)
            return (NULL);

        int j = 0;
        while (tokens[i]
            && tokens[i][0] != '|'
            && tokens[i][0] != '<'
            && tokens[i][0] != '>')
        {
            char *stripped = strip_quotes(tokens[i]);
            cmd->args[j++] = stripped;
            i++;
        }
        cmd->args[j] = NULL;

        if (cmd->args[0])
            cmd->cmd = ft_strdup(cmd->args[0]);

        while (tokens[i] && tokens[i][0] != '|')
        {
            if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
            {
                i++;
                cmd->infile = strip_quotes(tokens[i]);
            }
            else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
            {
                i++;
                cmd->infile = strip_quotes(tokens[i]);
                cmd->heredoc = 1;
            }
            else if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
            {
                i++;
                cmd->outfile = strip_quotes(tokens[i]);
                cmd->append = 0;
            }
            else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
            {
                i++;
                cmd->outfile = strip_quotes(tokens[i]);
                cmd->append = 1;
            }
            i++;
        }

        if (tokens[i] && tokens[i][0] == '|')
            i++;

        if (!head)
            head = cmd;
        else
            current->next = cmd;
        current = cmd;
    }
    return (head);
}