/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:34 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/11 14:42:39 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exec_init(t_command *cmds, t_exec *ctx, t_env **env)
{
	if (!process_all_heredocs(cmds, ctx->last_status, env))
	{
		ctx->last_status = 1;
		return (0);
	}
	ctx->prev_fd = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

static int	exec_builtin(t_command *curr, t_env **env, t_exec *ctx)
{
	int	saved_in;
	int	saved_out;
	int	ok;

	if (!is_builtins(curr->args) || curr->next)
		return (1);
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	ok = (saved_in != -1 && saved_out != -1);
	if (!ok)
	{
		perror("minishell: dup");
		ctx->last_status = 1;
	}
	else
	{
		if (!apply_redirection(curr, env))
			ctx->last_status = 1;
		else
			ctx->last_status = builtins(env, curr->args, ctx->prev_pwd, ctx->f);
		restore_std_fds(saved_in, saved_out);
	}
	close_heredoc_fds(curr->redirections);
	return (0);
}

static void	exec_pipeline(t_command *curr, t_env **env, t_exec *ctx)
{
	while (curr)
	{
		ctx->f = 0;
		if (curr->next && pipe(ctx->pipe_fd) == -1)
		{
			perror("pipe");
			break ;
		}
		if (creat_a_child(curr, env, ctx) == 1)
		{
			close(ctx->pipe_fd[0]);
			close(ctx->pipe_fd[1]);
			break ;
		}
		curr = curr->next;
	}
	close_fd(ctx->prev_fd);
}

static void	exec_finalize(t_command *start, t_exec *ctx)
{
	t_command		*c;
	t_redirection	*r;

	c = start;
	while (c)
	{
		r = c->redirections;
		while (r)
		{
			if (r->type == TOKEN_HEREDOC && r->heredoc_fd >= 0)
			{
				close(r->heredoc_fd);
				r->heredoc_fd = -1;
			}
			r = r->next;
		}
		c = c->next;
	}
	ft_wait(ctx);
}

void	execution(t_command *cmds, t_env **env, t_exec *ctx)
{
	t_command	*curr;
	int			saved_in;
	int			saved_out;

	ctx->f = 1;
	if (!exec_init(cmds, ctx, env))
		return ;
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	curr = cmds;
	if ((!curr->cmd || curr->cmd[0] == '\0')
		&& handle_empty_cmd(curr, env, ctx))
	{
		restore_std_fds(saved_in, saved_out);
		return ;
	}
	restore_std_fds(saved_in, saved_out);
	if (!exec_builtin(curr, env, ctx))
		return ;
	exec_pipeline(curr, env, ctx);
	exec_finalize(cmds, ctx);
}
// void	execution(t_command *cmds, t_env **env, t_exec *ctx)
// {
// 	t_command	*curr;
// 	int			saved_in;
// 	int			saved_out;

// 	if (!exec_init(cmds, ctx, env))
// 		return ;
// 	saved_in = dup(STDIN_FILENO);
// 	saved_out = dup(STDOUT_FILENO);
// 	curr = cmds;
// 	if ((!curr->cmd || curr->cmd[0] == '\0') && curr->redirections)
// 	{
// 		if (apply_redirection(curr, env))
// 			ctx->last_status = 0;
// 		else
// 			ctx->last_status = 1;
// 		restore_std_fds(saved_in, saved_out);
// 		// return ;
// 	}
// 	if ((!curr->cmd || curr->cmd[0] == '\0') && !curr->next)
// 	{
// 		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
// 		ctx->last_status = 127;
// 		restore_std_fds(saved_in, saved_out);
// 		return ;
// 	}
// 	restore_std_fds(saved_in, saved_out);
// 	if (!exec_builtin(curr, env, ctx))
// 		return ;
// 	exec_pipeline(curr, env, ctx);
// 	exec_finalize(cmds, ctx);
// }
