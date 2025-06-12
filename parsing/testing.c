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
void handler(int signal)
{

	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}


int main(int ac, char **av, char **envp)
{
	char		*raw;
	char		**tokens;
	t_command	*cmds;
	t_env		*env;

	char *prev_pwd = NULL;
	copy_env(envp, &env);
	signal(SIGINT, &handler);
	while (1)
	{
		raw = get_input();
		if (!raw)
			break;
		if (is_only_whitespace(raw))
		{
			free(raw);
			continue;
		}

		char *expanded = expand_variables(raw);
		free(raw);
		if (!expanded)
			continue;

		tokens = lexer(expanded);
		free(expanded);
		if (!tokens)
			continue;

		if (check_syntax_errors(tokens))
		{
			free_tokens(tokens);
			continue;
		}

		cmds = parse_tokens(tokens);
		if (cmds)
		{
			// print_commands(cmds);
			check_for_pwd(&prev_pwd);
			execution(&env, cmds, prev_pwd);
			free_commands(cmds);
		}

		free_tokens(tokens);
	}
	free_env(env);
	return (0);
}
