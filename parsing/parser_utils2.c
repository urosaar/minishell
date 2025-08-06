/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:06:23 by jesse             #+#    #+#             */
/*   Updated: 2025/08/06 17:40:31 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command    *init_command(int argc)
{
    t_command    *cmd;

    cmd = ft_calloc(1, sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->args)
    {
        free(cmd);
        return (NULL);
    }
    cmd->was_quoted = calloc(argc + 1, sizeof(bool));
    if (!cmd->was_quoted)
    {
        free(cmd->args);
        free(cmd);
        return (NULL);
    }
    return (cmd);
}


static int	handle_heredoc(t_redirection *redir, char *token, t_command *cmd)
{
	char	*stripped;

	stripped = strip_quotes(token);
	if (!stripped)
		return (1);
	redir->filename = stripped;
	cmd->heredoc = 1;
	if (strpbrk(token, "\"'") != NULL)
		cmd->heredoc_quoted = 1;
	else
		cmd->heredoc_quoted = 0;
	return (0);
}

static int	handle_redir(t_redirection *redir, char *token, t_command *cmd)
{
	char	*filename;

	filename = strip_quotes(token);
	if (!filename)
		return (1);
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
	return (0);
}

int	setup_redir(t_redirection **redir, char *token)
{
	*redir = ft_calloc(1, sizeof(t_redirection));
	if (!*redir)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		(*redir)->type = TOKEN_REDIRECT_IN;
	else if (ft_strcmp(token, "<<") == 0)
		(*redir)->type = TOKEN_HEREDOC;
	else if (ft_strcmp(token, ">") == 0)
		(*redir)->type = TOKEN_REDIRECT_OUT;
	else
		(*redir)->type = TOKEN_REDIRECT_APPEND;
	return (0);
}

int	process_redir(char **tokens, int *idx,
	t_command *cmd, t_redirection **last)
{
	t_redirection	*redir;
	int				err;

	if (setup_redir(&redir, tokens[*idx]))
		return (1);
	(*idx)++;
	if (!tokens[*idx])
		return (free(redir), 1);
	if (redir->type == TOKEN_HEREDOC)
		err = handle_heredoc(redir, tokens[*idx], cmd);
	else
		err = handle_redir(redir, tokens[*idx], cmd);
	if (err)
		return (free(redir), 1);
	if (!cmd->redirections)
		cmd->redirections = redir;
	else
		(*last)->next = redir;
	*last = redir;
	(*idx)++;
	return (0);
}
