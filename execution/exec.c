/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:34 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/05 18:28:58 by jesse            ###   ########.fr       */
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
			ctx->last_status = builtins(env, curr->args, ctx->prev_pwd);
		restore_std_fds(saved_in, saved_out);
	}
	close_heredoc_fds(curr->redirections);
	return (0);
}

static int	exec_pipeline(t_command *curr, t_env **env, t_exec *ctx)
{
	while (curr)
	{
		if (curr->next && pipe(ctx->pipe_fd) == -1)
		{
			perror("pipe");
			break ;
		}
		if (creat_a_child(curr, env, ctx) == 1)
			return (1);
		curr = curr->next;
	}
	close_fd(ctx->prev_fd);
	return (0);
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

	if (!exec_init(cmds, ctx, env))
		return ;
	curr = cmds;
	if (!curr->cmd || curr->cmd[0] == '\0')
	{
		ctx->last_status = 0; // Bash: success for empty command
		return;
	}
	if (!exec_builtin(curr, env, ctx))
		return ;
	if (exec_pipeline(curr, env, ctx))
		return ;
	exec_finalize(cmds, ctx);
}

// void execution(t_command *cmds, t_env **env, t_exec *ctx) 
// {
// 	if (!process_all_heredocs(cmds, ctx->last_status, env))
// 	{
//         ctx->last_status = 1;
//         return;
//     }
//     t_command	*curr;

// 	ctx->prev_fd = -1;
// 	curr = cmds;
// 	signal(SIGINT, SIG_IGN);
//     signal(SIGQUIT, SIG_IGN);

//     if (is_builtins(curr->args) && !curr->next)
// 	{
//         int saved_stdin = dup(STDIN_FILENO);
//         int saved_stdout = dup(STDOUT_FILENO);
//         int redir_success = 1;

//         if (saved_stdin == -1 || saved_stdout == -1)
// 		{
//             perror("minishell: dup");
//             redir_success = 0;
//         }
//         if (redir_success)
//             redir_success = apply_redirection(curr);
//         if (redir_success)
//             ctx->last_status = builtins(env, curr->args, ctx->prev_pwd);
// 		else 
//             ctx->last_status = 1;
//         if (saved_stdin != -1)
// 		{
//             dup2(saved_stdin, STDIN_FILENO);
//             close(saved_stdin);
//         }
//         if (saved_stdout != -1)
// 		{
//             dup2(saved_stdout, STDOUT_FILENO);
//             close(saved_stdout);
//         }
//         t_redirection *r = curr->redirections;
//         while (r)
// 		{
//             if (r->type == TOKEN_HEREDOC && r->heredoc_fd != -1)
// 			{
//                 close(r->heredoc_fd);
//                 r->heredoc_fd = -1;
//             }
//             r = r->next;
//         }
//         return;
//     }
// 	while (curr)
// 	{
// 		if (curr->next && pipe(ctx->pipe_fd) == -1)
// 		{
// 			perror("pipe");
// 			break;
// 		}
// 		creat_a_child(curr, env, ctx);
// 		curr = curr->next;
// 	}
// 	close_fd(ctx->prev_fd);
//     t_command *c = cmds;
//     while (c)
//     {
//         t_redirection *r = c->redirections;
//         while (r)
//         {
//             if (r->type == TOKEN_HEREDOC && r->heredoc_fd >= 0)
//             {
//                 close(r->heredoc_fd);
//                 r->heredoc_fd = -1;
//             }
//             r = r->next;
//         }
//         c = c->next;
//     }
// 	ft_wait(ctx);
// }