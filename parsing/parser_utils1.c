/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:02:09 by jesse             #+#    #+#             */
/*   Updated: 2025/08/06 17:41:02 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_redir_token(const char *tok)
{
	return (ft_strcmp(tok, "<") == 0
		|| ft_strcmp(tok, "<<") == 0
		|| ft_strcmp(tok, ">") == 0
		|| ft_strcmp(tok, ">>") == 0);
}

int	count_actual_args(char **tokens, int start)
{
	int	count;
	int	j;

	count = 0;
	j = start;
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
	return (count);
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_redirections(t_redirection *redir)
{
	t_redirection	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

void    free_command_partial(t_command *cmd)
{
    if (!cmd)
        return ;
    free_args(cmd->args);
    free(cmd->was_quoted);
    free_redirections(cmd->redirections);
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd);
}

