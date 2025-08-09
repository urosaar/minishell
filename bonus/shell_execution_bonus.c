/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execution_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 00:00:31 by jesse             #+#    #+#             */
/*   Updated: 2025/08/09 21:28:53 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_heredoc(t_command *cmds)
{
	if (cmds && cmds->redirections
		&& cmds->redirections->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

static void	expand_commands(t_command *cmds, t_exec *exec, t_env **env)
{
	t_command	*c;
	bool		*no_expand;

	g_status = 0;
	c = cmds;
	while (c)
	{
		no_expand = create_no_split_map(c->args);
		expand_command_vars(c, exec->last_status, env);
		expand_wildcards(c, no_expand);
		free(no_expand);
		c = c->next;
	}
}

static void	parse_expand_execute(char **tokens, t_env **env, t_exec *exec)
{
	t_command	*cmds;

	cmds = parse_tokens(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		exec->last_status = 2;
		return ;
	}
	expand_commands(cmds, exec, env);
	if (has_heredoc(cmds))
	{
		exec->last_status = 0;
		g_status = 1;
	}
	check_for_pwd(&exec->prev_pwd);
	execution(cmds, env, exec);
	free_commands(cmds);
}

static void	run_shell(t_env **env, t_exec *exec)
{
	char	*raw;
	char	**tokens;

	while (1)
	{
		signals();
		raw = read_and_preprocess(NULL, *env, exec);
		if (!raw)
			continue ;
		if (g_status == 1)
		{
			exec->last_status = 1;
			g_status = 0;
		}
		tokens = lex_and_validate(raw, exec);
		free(raw);
		if (!tokens)
			continue ;
		parse_expand_execute(tokens, env, exec);
		g_status = exec->last_status;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_exec	*exec;
	int		ret;

	env = NULL;
	exec = init_exec_env(envp, &env);
	if (!exec)
		return (1);
	ret = handle_args_error(ac, av, exec);
	if (ret)
		return (ret);
	run_shell(&env, exec);
	ft_malloc(0, FREE);
	return (exec->last_status);
}
