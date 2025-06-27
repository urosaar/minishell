#include "../minishell.h"


void free_tokens(char **tokens)
{
	int i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}
void print_commands(t_command *cmd)
{
	int idx = 0;
	while (cmd)
	{
		printf("------- Command #%d -------\n", idx++);
		printf("cmd: %s\n", cmd->cmd);
		for (int i = 0; cmd->args[i]; i++)
			printf("  arg[%d]: %s\n", i, cmd->args[i]);

		t_redirection *redir = cmd->redirections;
		while (redir)
		{
			char *type_str = NULL;
			if (redir->type == TOKEN_REDIRECT_IN)
				type_str = "redirect_in (<)";
			else if (redir->type == TOKEN_HEREDOC)
				type_str = "heredoc (<<)";
			else if (redir->type == TOKEN_REDIRECT_OUT)
				type_str = "redirect_out (>)";
			else if (redir->type == TOKEN_REDIRECT_APPEND)
				type_str = "redirect_append (>>)";
			else
				type_str = "unknown";

			printf("  redirection: %s, filename: %s\n", type_str, redir->filename);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
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
void expand_tokens(char **tokens, int last_status, t_env **env)
{
    int skip_next = 0;
    
    for (int i = 0; tokens[i]; i++)
    {
        if (skip_next)
        {
            skip_next = 0;
            continue;
        }

        if (ft_strcmp(tokens[i], "<<") == 0)
        {
            skip_next = 1;
        }
        else
        {
            char *expanded = expand_variables(tokens[i], last_status, env);
            if (expanded)
            {
                free(tokens[i]);
                tokens[i] = expanded;
            }
        }
    }
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
        expand_tokens(tokens, g_status, &env);

        if (check_syntax_errors(raw,tokens))
        {
            exec->last_status = 2;
            free_tokens(tokens);
            continue;
        }

        cmds = parse_tokens(tokens);
        g_status = 0;
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
