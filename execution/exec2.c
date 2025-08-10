/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:20:28 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/10 19:53:48 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_fd(int fd)
{
	if (fd != -1)
		close(fd);
}

void	perror_and_exit(const char *msg)
{
	perror(msg);
	exit(1);
}

void	ft_execve(t_command *curr, t_env **env, char *d)
{
	char	**envp;

	if (!d)
	{
		if (!ft_strchr(curr->cmd, '/'))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(curr->cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		exit(127);
	}
	if (curr->cmd[0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		exit (127);
	}
	envp = build_env_array(env);
	if (execve(d, curr->args, envp) == -1)
	{
		free_envp(envp);
		perror("minishell: execve");
		exit(1);
	}
}

void	dup_if_there_is_pipe(t_command *curr, int *pipe_fd, int prev_fd)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			perror_and_exit("minishell: dup2");
		close(prev_fd);
	}
	if (curr)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror_and_exit("minishell: dup2");
		close(pipe_fd[1]);
	}
}
