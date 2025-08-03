/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 16:14:09 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 18:11:57 by jesse            ###   ########.fr       */
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
	save_shell_term();
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
