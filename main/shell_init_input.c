/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 16:12:28 by jesse             #+#    #+#             */
/*   Updated: 2025/08/12 12:58:08 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signals(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

t_exec	*init_exec_env(char **envp, t_env **env)
{
	t_exec	*exec;

	exec = ft_malloc(sizeof(t_exec), MALLOC);
	if (!exec)
	{
		perror("minishell");
		return (NULL);
	}
	exec->last_status = 0;
	exec->prev_pwd = NULL;
	g_status = 0;
	copy_env(envp, env);
	return (exec);
}

int	handle_args_error(int ac, char **av, t_exec *exec)
{
	(void)ac;
	if (av[1])
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(av[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exec->last_status = 127;
		return (127);
	}
	return (0);
}

char	*read_and_preprocess(t_command *cmds, t_env *env, t_exec *exec)
{
	char	*raw;

	raw = get_input();
	if (!raw)
		handler_eof(cmds, env, g_status);
	if (is_only_whitespace(raw))
	{
		free(raw);
		return (NULL);
	}
	if (check_unclosed_quotes(raw))
	{
		printf("minishell: syntax error: unclosed quotes\n");
		exec->last_status = 258;
		free(raw);
		return (NULL);
	}
	return (raw);
}

char	**lex_and_validate(char *raw, t_exec *exec)
{
	char	**tokens;

	tokens = lexer(raw);
	if (!tokens)
	{
		exec->last_status = 1;
		return (NULL);
	}
	if (check_syntax_errors(raw, tokens))
	{
		exec->last_status = 258;
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
