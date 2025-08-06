/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:07:47 by jesse             #+#    #+#             */
/*   Updated: 2025/08/05 18:23:50 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_arg(char **tokens, int *idx, t_command *cmd, int *arg_i)
{
	char	*arg;

	arg = ft_strdup(tokens[*idx]);
	if (!arg)
		return (1);
	cmd->args[(*arg_i)++] = arg;
	(*idx)++;
	return (0);
}

void	finalize_command(t_command *cmd, int arg_i)
{
	cmd->args[arg_i] = NULL;
	if (cmd->args[0])
		cmd->cmd = ft_strdup(cmd->args[0]);
}

t_command	*parse_one_command(char **tokens, int *idx)
{
	t_command		*cmd;
	int				argc;
	int				arg_i;
	t_redirection	*last_redir;

	argc = count_actual_args(tokens, *idx);
	cmd = init_command(argc);
	if (!cmd)
		return (NULL);
	arg_i = 0;
	last_redir = NULL;
	while (tokens[*idx] && tokens[*idx][0] != '|')
	{
		if (is_redir_token(tokens[*idx]))
		{
			if (process_redir(tokens, idx, cmd, &last_redir))
				return (free_command_partial(cmd), NULL);
		}
		else if (process_arg(tokens, idx, cmd, &arg_i))
			return (free_command_partial(cmd), NULL);
	}
	finalize_command(cmd, arg_i);
	return (cmd);
}

int	count_total_heredocs(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "<<") == 0)
			count++;
		i++;
	}
	return (count);
}

int	process_and_link(char **tokens, int *i,
	t_command **head, t_command **current)
{
	t_command	*cmd;

	cmd = parse_one_command(tokens, i);
	if (!cmd)
	{
		free_commands(*head);
		return (1);
	}
	if (!*head)
		*head = cmd;
	else
		(*current)->next = cmd;
	*current = cmd;
	if (tokens[*i] && tokens[*i][0] == '|')
		(*i)++;
	return (0);
}
