#include "../minishell.h"

char	**build_env_array(t_env *env)
{
	int		count = 0;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	char **envp = malloc((count + 1) * sizeof(char *));
	tmp = env;
	int i = 0;
	while (tmp)
	{
		envp[i] = ft_join(tmp->key, "=");
		envp[i] = ft_join(envp[i], tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	*check_if_exist(t_env *env, t_command *cmds)
{
	char	**path;
	t_env	*tmp;
	int		i;

	i = 0;
	path = NULL;
	tmp = env;
	if (!cmds->cmd)
		return (NULL);
	if (access(cmds->cmd, X_OK) == 0)
		return ft_strdup(cmds->cmd);
	if (strchr(cmds->cmd, '/'))
	{
		printf("minishell: %s: No such file or directory\n", cmds->cmd);
		return (NULL);
	}
	while (tmp)
	{
		if (!strcmp(tmp->key, "PATH"))
		{
			path = ft_split(tmp->value, ':');
			break;
		}
		tmp = tmp->next;
	}
	if (!path)
		return (NULL);
	while (path[i])
	{
		char *s = ft_join("/", cmds->cmd);
		char *d = ft_join(path[i], s);
		free(s);
		if (access(d, X_OK) == 0)
		{
			free_array(path);
			return d;
		}
		free(d);
		i++;
	}
	free_array(path);
	return (NULL);
}

int	ft_size_node(t_command *cmds)
{
	t_command	*tmp;
	int			i;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
void is_outfile(char *d, t_command *curr)
{
	int f = open(curr->redirections->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (d || is_builtins(curr->args))
	{
		dup2(f, STDOUT_FILENO);
		close(f);
	}
}

void is_infile(char *d, t_command *curr)
{
	int f = open(curr->redirections->filename, O_RDWR, 0777);
	if (f == -1)
	{
		printf("minishell: %s: No such file or directory\n", curr->redirections->filename);
		return;
	}
	if (d || is_builtins(curr->args))
	{
		dup2(f, STDIN_FILENO);
		close(f);
	}
}

void is_append(char *d, t_command *curr)
{
	int f = open(curr->redirections->filename, O_CREAT | O_RDWR | O_APPEND, 0777); 
	if (d || is_builtins(curr->args))
	{
		dup2(f, STDOUT_FILENO);
		close(f);
	}
}

void	execution(t_env *env, t_command *cmds, char *prev_pwd)
{
	t_command	*curr;
	char		*d;
	char		**envp;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;

	curr = cmds;
	prev_fd = -1;
	if (is_builtins(curr->args) && !curr->next && !curr->redirections)
	{
		builtins(&env, curr->args, prev_pwd);
		return;
	}
	while (curr)
	{
		if (curr->next && pipe(pipe_fd) == -1)
			return;
		pid = fork();
		if (pid == -1)
			return;
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (curr->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			d = check_if_exist(env, curr);
			while (curr->redirections)
			{
				if (curr->redirections->filename && curr->redirections->type == 3)
					is_outfile(d, curr);
				if (curr->redirections->filename && curr->redirections->type == 4)
					is_append(d, curr);
				if (curr->redirections->filename && curr->redirections->type == 2)
					is_infile(d, curr);
				curr->redirections = curr->redirections->next;
			}
			if (is_builtins(curr->args))
			{
				builtins(&env, curr->args, prev_pwd);
				exit(0);
			}
			if (!d)
			{
				if (!strchr(curr->cmd, '/'))
					printf("minishell: %s: command not found\n", curr->cmd);
				exit(2);
			}
			envp = build_env_array(env);
			if (execve(d, curr->args, envp) == -1)
				exit(2);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (curr->next)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		curr = curr->next;
	}
	while (wait(NULL) > 0);
}
