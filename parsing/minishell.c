/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:03:46 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/01 21:26:44 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_status = 0;

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void	free_strarray(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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

void	signals(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

static t_exec	*init_exec_env(char **envp, t_env **env)
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

static int	handle_args_error(int ac, char **av, t_exec *exec)
{
	(void)ac;
	if (av[1])
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", av[1]);
		exec->last_status = 127;
		return (127);
	}
	return (0);
}

static char	*read_and_preprocess(t_command *cmds, t_env *env, t_exec *exec)
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

static char	**lex_and_validate(char *raw, t_exec *exec)
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

static int	has_heredoc(t_command *cmds)
{
	if (cmds && cmds->redirections
		&& cmds->redirections->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

static void	parse_expand_execute(char **tokens, t_env **env, t_exec *exec)
{
	t_command	*cmds;
	t_command	*c;

	cmds = parse_tokens(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		exec->last_status = 2;
		return ;
	}
	g_status = 0;
	c = cmds;
	while (c)
	{
		expand_command_vars(c, exec->last_status, env);
		c = c->next;
	}
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
	save_shell_termios();
	run_shell(&env, exec);
	ft_malloc(0, FREE);
	return (exec->last_status);
}

// int main(int ac, char **av, char **envp)
// {
// 	char      *raw = NULL;
// 	char      **tokens;
// 	t_exec    *exec;
// 	t_command *cmds = NULL;
// 	t_env     *env = NULL;

// 	exec = ft_malloc(sizeof(t_exec), MALLOC);
// 	if (!exec)
// 	{
// 		perror("minishell");
// 		return (1);
// 	}
// 	exec->last_status = 0;
// 	exec->prev_pwd = NULL;
// 	g_status = 0;

// 	if (av[1])
// 	{
// 		fprintf(stderr, "minishell: %s: No such file or directory\n", av[1]);
// 		exec->last_status = 127;
// 		return (127);
// 	}
// 	copy_env(envp, &env);
// 	while (1)
//     {
//         signals();
//         raw = get_input();
//         if (!raw)
//             handler_eof(cmds, env, g_status);

//         if (is_only_whitespace(raw))
//         {
//             free(raw);
//             continue;
//         }

//         if (check_unclosed_quotes(raw))
//         {
//             printf("minishell: syntax error: unclosed quotes\n");
//             exec->last_status = 258;
//             free(raw);
//             continue;
//         }
//         tokens = lexer(raw);
//         // free(raw); 
//         if (!tokens)
//         {
//             exec->last_status = 1;
//             continue;
//         }
//         if (check_syntax_errors(raw,tokens))
//         {
//             exec->last_status = 2;
//             free_tokens(tokens);
// 			free(raw);
//             continue;
//         }

//      cmds = parse_tokens(tokens);
// 	if (!cmds)
// 	 {
// 		 free_tokens(tokens);
// 		exec->last_status = 2;
// 		 continue;
// 	}

//     g_status = 0;

//     t_command *c = cmds;
//     while (c)
//     {
//         expand_command_vars(c, exec->last_status, &env);
//         c = c->next;
//     }

//     if (cmds && cmds->redirections && 
//cmds->redirections->type == TOKEN_HEREDOC)
//     {
//         exec->last_status = 0;
//         g_status = 1;
//     }

//     if (cmds)
//     {
//         check_for_pwd(&exec->prev_pwd);
//         execution(cmds, &env, exec);
//         free_commands(cmds);
// 		free(raw);
// 		raw = NULL;
//         cmds = NULL;
//     }
//         g_status = exec->last_status;
//         free_tokens(tokens);
//     }
// 	ft_malloc(0, FREE);
// 	return (exec->last_status);
// }
