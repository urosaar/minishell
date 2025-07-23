#include "../minishell.h"

// Refactored parse_tokens: handle redirections and args in one pass per command
static bool is_redir_token(const char *tok)
{
    return (ft_strcmp(tok, "<") == 0
         || ft_strcmp(tok, "<<") == 0
         || ft_strcmp(tok, ">") == 0
         || ft_strcmp(tok, ">>") == 0);
}

// Count actual arguments (non-redir) between start and next pipe or end
static int count_actual_args(char **tokens, int start)
{
    int count = 0;
    int j = start;
    while (tokens[j] && tokens[j][0] != '|')
    {
        if (is_redir_token(tokens[j]))
            j += 2; 
        else
        {
            count++;
            j++;
        }
    }
    return count;
}
 // Parse a single command from tokens, handling redirections and args
static t_command *parse_one_command(char **tokens, int *idx)
{
    t_command    *cmd = ft_calloc(1, sizeof(t_command));
    int           argc;
    int           arg_i = 0;
    t_redirection *last_redir = NULL;

    if (!cmd)
        return NULL;

    argc = count_actual_args(tokens, *idx);
    cmd->args = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->args)
        return NULL;

     while (tokens[*idx] && tokens[*idx][0] != '|')
    {
        if (is_redir_token(tokens[*idx]))
        {
            t_redirection *redir = ft_calloc(1, sizeof(t_redirection));
            if (!redir)
                return NULL;
                
            if (ft_strcmp(tokens[*idx], "<") == 0)
                redir->type = TOKEN_REDIRECT_IN;
            else if (ft_strcmp(tokens[*idx], "<<") == 0)
                redir->type = TOKEN_HEREDOC;
            else if (ft_strcmp(tokens[*idx], ">") == 0)
                redir->type = TOKEN_REDIRECT_OUT;
            else 
                redir->type = TOKEN_REDIRECT_APPEND;

            (*idx)++;
            if (!tokens[*idx]) {
                free(redir);
                return NULL;
            }

            if (redir->type == TOKEN_HEREDOC)
            {
             char *stripped = strip_quotes(tokens[*idx]);
            if (!stripped)
            {
                free(redir);
               return NULL;
            }
            cmd->heredoc_delimiter = stripped;
            redir->filename        = ft_strdup(stripped);
                cmd->heredoc = 1;
                if (tokens[*idx][0] == '\'' || tokens[*idx][0] == '"')
                    cmd->heredoc_quoted = 1;
                else
                    cmd->heredoc_quoted = 0;
            }
            else
            {
                char *filename = strip_quotes(tokens[*idx]);
                redir->filename = filename;
                
                if (redir->type == TOKEN_REDIRECT_IN)
                    cmd->infile = ft_strdup(filename);
                else if (redir->type == TOKEN_REDIRECT_OUT)
                {
                    cmd->outfile = ft_strdup(filename);
                    cmd->append = 0;
                }
                else 
                {
                    cmd->outfile = ft_strdup(filename);
                    cmd->append = 1;
                }
            }

            if (!cmd->redirections)
                cmd->redirections = redir;
            else
                last_redir->next = redir;
            last_redir = redir;
            (*idx)++;
        }
        else
        {
            cmd->args[arg_i++] = strip_quotes(tokens[*idx]);
            (*idx)++;
        }
    }
    cmd->args[arg_i] = NULL;
    if (cmd->args[0])
        cmd->cmd = ft_strdup(cmd->args[0]);

    return cmd;
}
static int count_total_heredocs(char **tokens)
{
    int count = 0;
    for (int i = 0; tokens[i]; i++)
    {
        if (ft_strcmp(tokens[i], "<<") == 0)
            count++;
    }
    return count;
}
t_command *parse_tokens(char **tokens)
{
    t_command *head = NULL;
    t_command *current = NULL;
    int i = 0;
     if (count_total_heredocs(tokens) > 16)
    {
        ft_putstr_fd("minishell: maximum here-document count exceeded\n", STDERR_FILENO);
        return NULL;
    }

    while (tokens[i])
    {
        t_command *cmd = parse_one_command(tokens, &i);
        if (!cmd)
            return NULL;
        if (!head)
            head = cmd;
        else
            current->next = cmd;
        current = cmd;
        if (tokens[i] && tokens[i][0] == '|')
            i++;
    }
    return head;
}

