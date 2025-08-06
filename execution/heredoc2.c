/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:59:46 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/06 17:59:39 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(1);
}

static void	process_heredoc_line(t_heredoc_child *data, char *line)
{
	char	*expanded;

	expanded = NULL;
	if (!data->quoted)
	{
		expanded = expand_variables(line, data->last_status, data->env);
		if (expanded)
		{
			write(data->write_fd, expanded, ft_strlen(expanded));
			write(data->write_fd, "\n", 1);
			free(expanded);
		}
		else
		{
			write(data->write_fd, line, ft_strlen(line));
			write(data->write_fd, "\n", 1);
		}
	}
	else
	{
		write(data->write_fd, line, ft_strlen(line));
		write(data->write_fd, "\n", 1);
	}
}

int	heredoc_iteration(t_heredoc_child *data, int *line_count)
{
	char	*line;

	if (*line_count >= HEREDOC_MAX_LINES)
	{
		ft_putstr_fd("minishell: heredoc limit exceeded\n", STDERR_FILENO);
		close(data->write_fd);
		exit(1);
	}
	line = readline("> ");
	if (!line)
		return (0);
	if (ft_strcmp(line, data->delimiter) == 0)
	{
		free(line);
		return (0);
	}
	process_heredoc_line(data, line);
	free(line);
	(*line_count)++;
	return (1);
}

void	close_heredoc_fds(t_redirection *r)
{
	while (r)
	{
		if (r->type == TOKEN_HEREDOC && r->heredoc_fd != -1)
		{
			close(r->heredoc_fd);
			r->heredoc_fd = -1;
		}
		r = r->next;
	}
}
