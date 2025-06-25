#include "../minishell.h"

int g_status = 0;

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
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();

}

void signals()
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN); /* [ctrl + \] */
}

// void leak_fd(void)
// {
// 	int fd = 3;
// 	while (fd < OPEN_MAX)
// 	{
// 		close(fd);
// 		fd++;
// 	}
// }

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
    printf("status = %d\n", exec->last_status);
    exec->prev_pwd     = NULL;

    if (av[1])
    {
        fprintf(stderr, "minishell: %s: No such file or directory\n", av[1]);
        exec->last_status = 127;
        return (127);
    }
    // rl_catch_signals = 0;
    copy_env(envp, &env);

    while (1)
    {
        signals();
        exec->last_status = ft_exit_status(g_status);
        printf("HIIII\n");
        raw = get_input();
        if (!raw)
            handler_eof(cmds, env, exec->last_status);

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

        char *expanded = expand_variables(raw, exec->last_status, &env);
        if (!expanded)
        {
            exec->last_status = 1;
            free(raw);
            continue;
        }

        tokens = lexer(expanded);
        if (!tokens)
        {
            exec->last_status = 1;
            free(raw);
            free(expanded);
            continue;
        }

        if (check_syntax_errors(raw, tokens))
        {
            exec->last_status = 2;
            free(raw);
            free(expanded);
            free_tokens(tokens);
            continue;
        }

        free(raw);
        free(expanded);

        cmds = parse_tokens(tokens);
        if (cmds)
        {
            check_for_pwd(&exec->prev_pwd);
            execution(cmds, &env, exec);
            free_commands(cmds);
            cmds = NULL;
        }
        free_tokens(tokens);
    }
	ft_malloc(0, FREE);
    return (exec->last_status);
}
