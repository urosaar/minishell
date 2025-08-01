/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:58:38 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/01 20:47:44 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	child_heredoc(t_heredoc_child *data)
{
	int	line_count;

	signal(SIGINT, handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	line_count = 0;
	while (1)
	{
		if (!heredoc_iteration(data, &line_count))
			break ;
	}
	close(data->write_fd);
	exit(0);
}

static int	parent_heredoc(pid_t pid, int pipefd[2])
{
	int	status;

	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (-1);
	}
	else if (WIFSIGNALED(status))
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

static int	start_heredoc_child(t_heredoc_child *data, int *pipefd)
{
	close(pipefd[0]);
	data->write_fd = pipefd[1];
	child_heredoc(data);
	return (-1);
}

int	handle_heredoc(char *delimiter, int quoted, int last_status, t_env **env)
{
	int				pipefd[2];
	pid_t			pid;
	t_heredoc_child	data;

	if (!delimiter || !env)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		data.delimiter = delimiter;
		data.quoted = quoted;
		data.last_status = last_status;
		data.env = env;
		return (start_heredoc_child(&data, pipefd));
	}
	return (parent_heredoc(pid, pipefd));
}

int	process_all_heredocs(t_command *cmds, int last_status, t_env **env)
{
	t_command		*curr;
	t_redirection	*redir;

	curr = cmds;
	while (curr)
	{
		redir = curr->redirections;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				redir->heredoc_fd = handle_heredoc(
						redir->filename, curr->heredoc_quoted,
						last_status, env);
				if (redir->heredoc_fd < 0)
					return (0);
			}
			redir = redir->next;
		}
		curr = curr->next;
	}
	return (1);
}
