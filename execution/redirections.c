/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:50:47 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/01 20:49:34 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redirect_input(char *filename)
{
	int	f;

	f = open(filename, O_RDONLY);
	if (f == -1)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", filename);
		return (0);
	}
	if (dup2(f, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(f);
		return (0);
	}
	close(f);
	return (1);
}

int	append_mode(const char *filename)
{
	int	f;

	f = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (f == -1)
	{
		perror("minishell: append_mode");
		return (0);
	}
	if (dup2(f, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(f);
		return (0);
	}
	close(f);
	return (1);
}

int	redirect_output(const char *filename)
{
	int	f;

	f = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (f == -1)
	{
		perror("minishell: redirect_output");
		return (0);
	}
	if (dup2(f, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(f);
		return (0);
	}
	close(f);
	return (1);
}

static int	handle_single_redirection(t_redirection *tmp)
{
	if (tmp->type == TOKEN_HEREDOC && tmp->heredoc_fd >= 0)
	{
		if (dup2(tmp->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 heredoc");
			return (0);
		}
		close(tmp->heredoc_fd);
		tmp->heredoc_fd = -1;
		return (1);
	}
	if (tmp->type == TOKEN_REDIRECT_IN)
		return (redirect_input(tmp->filename));
	if (tmp->type == TOKEN_REDIRECT_OUT)
		return (redirect_output(tmp->filename));
	if (tmp->type == TOKEN_REDIRECT_APPEND)
		return (append_mode(tmp->filename));
	return (1);
}

int	apply_redirection(t_command *curr)
{
	t_redirection	*tmp;

	tmp = curr->redirections;
	while (tmp)
	{
		if (!handle_single_redirection(tmp))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
