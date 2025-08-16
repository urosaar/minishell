/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:59:46 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/16 05:46:43 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(1);
}

static char	*process_char(char *out, const char *line, size_t *i,
	t_expand_ctx *ctx)
{
	char	*tmp;

	if (line[*i] == '$')
	{
		tmp = handle_dollar(out, line, i, ctx);
		if (!tmp)
		{
			free(out);
			return (NULL);
		}
		out = tmp;
	}
	else
	{
		tmp = apend_char(out, line[*i]);
		if (!tmp)
		{
			free(out);
			return (NULL);
		}
		out = tmp;
		(*i)++;
	}
	return (out);
}

char	*heredoc_expand_line(const char *line, int last_status, t_env **env)
{
	size_t			i;
	char			*out;
	t_expand_ctx	ctx;

	i = 0;
	out = NULL;
	ctx.last_status = last_status;
	ctx.env = env;
	while (line[i])
	{
		out = process_char(out, line, &i, &ctx);
		if (!out)
			return (NULL);
	}
	if (!out)
		out = ft_strdup("");
	return (out);
}

static void	process_heredoc_line(t_heredoc_child *data, char *line)
{
	char	*expanded;

	if (!data->quoted)
	{
		expanded = heredoc_expand_line(line, data->last_status, data->env);
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
