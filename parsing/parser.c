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
        t_command *cmd = ft_calloc(1, sizeof(t_command));
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

        t_redirection *last_redir = NULL;
        while (tokens[i] && tokens[i][0] != '|')
        {
            t_redirection *redir = NULL;
            if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
            {
                i++;
                redir = ft_calloc(1, sizeof(t_redirection));
                redir->type = TOKEN_REDIRECT_IN;
                redir->filename = strip_quotes(tokens[i]);
            }
            else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
            {
                i++;
                redir = ft_calloc(1, sizeof(t_redirection));
                redir->type = TOKEN_HEREDOC;
                redir->filename = strip_quotes(tokens[i]);
            }
            else if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
            {
                i++;
                redir = ft_calloc(1, sizeof(t_redirection));
                redir->type = TOKEN_REDIRECT_OUT;
                redir->filename = strip_quotes(tokens[i]);
            }
            else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
            {
                i++;
                redir = ft_calloc(1, sizeof(t_redirection));
                redir->type = TOKEN_REDIRECT_APPEND;
                redir->filename = strip_quotes(tokens[i]);
            }
            else
            {
                i++;
            }

            if (redir)
            {
                if (!cmd->redirections)
                    cmd->redirections = redir;
                else
                    last_redir->next = redir;
                last_redir = redir;
            }
        }
        if (!head)
            head = cmd;
        else
            current->next = cmd;
        current = cmd;

        if (tokens[i] && tokens[i][0] == '|')
            i++;
    }

    return (head);
}
