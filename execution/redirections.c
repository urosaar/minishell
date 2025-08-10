/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:50:47 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/10 19:54:42 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redirect_input(char *filename)
{
	int	f;

	f = open(filename, O_RDONLY);
	if (f == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
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

int	redirect_output(char *filename)
{
	int		f;
	char	*temp;

	if ((filename[0] == (char)127 && filename[1] == '\0'))
	{
		temp = ft_strdup("");
		filename = temp;
	}
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

int	apply_redirect(t_redirection *tmp, char *clean)
{
	int	ok;

	if (tmp->type == TOKEN_REDIRECT_IN)
		ok = redirect_input(clean);
	else if (tmp->type == TOKEN_REDIRECT_OUT)
		ok = redirect_output(clean);
	else if (tmp->type == TOKEN_REDIRECT_APPEND)
		ok = append_mode(clean);
	else
		ok = 1;
	return (ok);
}

int	apply_redirection(t_command *curr, t_env **env)
{
	t_redirection	*tmp;

	tmp = curr->redirections;
	while (tmp)
	{
		if (!handle_single_redirection(tmp, env))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
