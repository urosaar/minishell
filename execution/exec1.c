/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:07:08 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/03 19:12:04 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	child_process(t_command *curr, t_env **env, t_exec *ctx)
{
	char	*d;

	if (curr->next || ctx->prev_fd != -1)
		dup_if_there_is_pipe(curr->next, ctx->pipe_fd, ctx->prev_fd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
    restore_shell_term(ctx);
	if (!apply_redirection(curr))
		exit(1);
	if (curr->cmd == NULL || curr->cmd[0] == '\0')
	{
		if (curr->redirections != NULL)
			exit(0);
		fprintf(stderr, "minishell: : command not found\n");
		exit(127);
	}
	d = check_if_exist(*env, curr);
	if (is_builtins(curr->args))
		exit(builtins(env, curr->args, ctx->prev_pwd));
	ft_execve(curr, env, d);
}

static void	parent_process(t_command *curr, t_exec *ctx)
{
	close_fd(ctx->prev_fd);
	ctx->last_pid = ctx->pid;
	if (curr->next)
	{
		close_fd(ctx->pipe_fd[1]);
		ctx->prev_fd = ctx->pipe_fd[0];
	}
}

int	creat_a_child(t_command *curr, t_env **env, t_exec *ctx)
{
	ctx->pid = fork();
	if (ctx->pid == -1)
	{
		perror("fork");
		return(1);
	}
	if (ctx->pid == 0)
		child_process(curr, env, ctx);
	else
		parent_process(curr, ctx);
	return (0);
}

void	ft_wait(t_exec *ctx)
{
	int	status;
	int	x;

	x = wait(&status);
	while (x > 0)
	{
		if (x == ctx->last_pid)
		{
    		restore_shell_term(ctx);
			if (WIFEXITED(status))
				ctx->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status) && g_status == 0)
			{
				ctx->last_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				if (WTERMSIG(status) == SIGQUIT)
					printf("Quit: 3\n");
			}
		}
		x = wait(&status);
	}
}

void	restore_std_fds(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}
