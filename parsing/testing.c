/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:03:46 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/22 11:03:54 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_tokens(char **tokens)
{
	int i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void free_commands(t_command *cmd)
{
	t_command *next;
	int       i;

	while (cmd)
	{
		next = cmd->next;
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		cmd->args = NULL;
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->cmd)
			free(cmd->cmd);
		free(cmd);
		cmd = next;
	}
}
int ft_exit_status(int status)
{
	if (status == 1)
		return (1);
	return (0);
}
void handler_eof(t_command *cmds, t_env *env, int last_status)
{
	if (cmds)
		free_commands(cmds);
	if (env)
		free_env(env);
	// write(STDOUT_FILENO, "\033[A\033[2C", 7);  
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

void signals()
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN); /* [ctrl + \] */
}

int main(int ac, char **av, char **envp)
{
	char      *raw;
	char      **tokens;
	t_exec    *exec;
	t_command *cmds;
	t_env     *env = NULL;

	exec = ft_malloc(sizeof(t_exec), MALLOC);
	if (!exec)
	{
		perror("minishell");
		return (1);
	}
	exec->last_status = 0;
	exec->prev_pwd = NULL;
	g_status = 0;

	if (av[1])
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", av[1]);
		exec->last_status = 127;
		return (127);
	}
	copy_env(envp, &env);

	while (1)
    {
        signals();
        raw = get_input();
        if (!raw)
            handler_eof(cmds, env, g_status);

        if (is_only_whitespace(raw))
        {
            free(raw);
            continue;
        }

        if (check_unclosed_quotes(raw))
        {
            printf("minishell: syntax error: unclosed quotes\n");
            exec->last_status = 258;
            free(raw);
            continue;
        }
        tokens = lexer(raw);
        free(raw); 
        
        if (!tokens)
        {
            exec->last_status = 1;
            continue;
        }
        if (check_syntax_errors(raw,tokens))
        {
            exec->last_status = 2;
            free_tokens(tokens);
            continue;
        }

     cmds = parse_tokens(tokens);
	if (!cmds)
	 {
		 free_tokens(tokens);
		exec->last_status = 2;
		 continue;
	}

    g_status = 0;

    t_command *c = cmds;
    while (c)
    {
        expand_command_vars(c, exec->last_status, &env);
        c = c->next;
    }

    if (cmds && cmds->redirections && cmds->redirections->type == TOKEN_HEREDOC)
    {
        exec->last_status = 0;
        g_status = 1;
    }

    if (cmds)
    {
        check_for_pwd(&exec->prev_pwd);
        execution(cmds, &env, exec);
        free_commands(cmds);
        cmds = NULL;
    }
        g_status = exec->last_status;
        free_tokens(tokens);
    }
	ft_malloc(0, FREE);
	return (exec->last_status);
}
