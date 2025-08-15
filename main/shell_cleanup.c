/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:08:59 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/12 21:09:01 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void	free_commands(t_command *cmds)
{
	t_command		*next;
	t_redirection	*redir;
	t_redirection	*next_redir;

	while (cmds)
	{
		next = cmds->next;
		free(cmds->cmd);
		free_strarray(cmds->args);
		free(cmds->infile);
		free(cmds->outfile);
		redir = cmds->redirections;
		while (redir)
		{
			next_redir = redir->next;
			free(redir->filename);
			free(redir);
			redir = next_redir;
		}
		free(cmds);
		cmds = next;
	}
}

int	ft_exit_status(int status)
{
	if (status == 1)
		return (1);
	return (0);
}

void	handler_eof(t_command *cmds, t_env *env, int last_status)
{
	if (cmds)
		free_commands(cmds);
	cmds = NULL;
	if (env)
		free_env(env);
	env = NULL;
	printf("exit\n");
	exit(last_status);
}

void	handler(int signal)
{
	if (signal == SIGINT)
	{
		g_status = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
