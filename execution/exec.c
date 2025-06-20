/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:34 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/20 16:19:04 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**build_env_array(t_env **env)
{
	int		count = 0;
	t_env	*tmp;

	count = 0;
	tmp = *env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	char **envp = malloc((count + 1) * sizeof(char *));
	tmp = *env;
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
void	free_envp(char **envp)
{
	int i = 0;
	if (!envp)
		return;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
char	*ft_check1(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return ft_strdup(cmd);
	if (strchr(cmd, '/'))
		printf("minishell: %s: No such file or directory\n", cmd);
	return (NULL);
}
char	*ft_check2(char **path, char *cmd)
{
	char	*s;
	char	*d;
	int		i;

	i = 0;
	while (path[i] && cmd)
	{
		s = ft_join("/", cmd);
		d = ft_join(path[i], s);
		free(s);
		if (access(d, X_OK) == 0)
		{
			free_array(path);
			return (d);
		}
		free(d);
		i++;
	}
	return (NULL);
}


char	*check_if_exist(t_env *env, t_command *cmds)
{
	char	**path;
	char	*d;
	t_env	*tmp;

	path = NULL;
	tmp = env;
	ft_check1(cmds->cmd);
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
	d = ft_check2(path, cmds->cmd);
	if (d)
		return (d);
	return (free_array(path), NULL);
}
// int	check_redirections(t_command *cmd)
// {
// 	t_redirection *r = cmd->redirections;

// 	while (r)
// 	{
// 		if (r->type == TOKEN_REDIRECT_IN)
// 		{
// 			if (access(r->filename, R_OK) == -1)
// 			{
// 				// printf("minishellll: %s: No such file or directory\n", r->filename);
// 				return (0); 
// 			}
// 		}
// 		r = r->next;
// 	}
// 	return (1);
// }

int redirect_output(char *d, t_command *curr)
{
	int f = open(curr->redirections->filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (f == -1)
	{
		perror("minishell: redirect_output");
		return (0); 
	}
	if (d || is_builtins(curr->args))
	{
		if (dup2(f, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(f);
			return (0);
		}
	}
	close(f);
	return (1);
}

int append_mode(char *d, t_command *curr)
{
	int f = open(curr->redirections->filename, O_CREAT | O_RDONLY | O_APPEND, 0777);
	if (f == -1)
	{
		perror("minishell: append_mode");
		return (0);
	}
	if (d || is_builtins(curr->args))
	{
		if (dup2(f, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(f);
			return (0); 
		}
	}
	close(f);
	return (1); 
}

int redirect_input(char *d, t_command *curr)
{
	int f = open(curr->redirections->filename, O_RDONLY, 0777);
	if (f == -1)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n", curr->redirections->filename);
		return (0);
	}
	if (d || is_builtins(curr->args))
	{
		if (dup2(f, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(f);
			return (0);
		}
	}
	close(f);
	return (1);
}
void handler_heredoc()
{
	write(1, "\n", 1);
	exit(130);
}

int handle_heredoc(t_command *cmd)
{
	int pipefd[2];
	char *line;
	int total_lines = 0;
	
	if (pipe(pipefd) == -1)
		return (-1);

	pid_t pid = fork();
	if (pid == -1)
		return (-1);

	if (pid == 0)
	{
		close(pipefd[0]);
		while (1)
		{
			signal(SIGINT, handler_heredoc);
			line = readline("> ");
			if (!line || ft_strcmp(line, cmd->infile) == 0)
				break;

			total_lines++;
			if (total_lines > HEREDOC_MAX_LINES)
			{
				printf("minishell: heredoc limit exceeded\n");
				free(line);
				close(pipefd[1]);
				exit(1);
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		free(line);
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		int status;
		close(pipefd[1]);
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			close(pipefd[0]);
			return (-1);
		}
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	return (0);
}

void close_fd(int fd)
{
	if (fd != -1)
		close(fd);
}

void perror_and_exit(const char *msg)
{
	perror(msg);
	exit(1);
}
void	redirection(t_command *curr, t_redirection *r, char *d)
{
	while (r)
	{
		if (r->type == TOKEN_HEREDOC)
		{
			if (handle_heredoc(curr) == -1)
				exit(1);
		}
		else if (r->type == TOKEN_REDIRECT_IN)
		{
			if (!redirect_input(d, curr))
				exit(1);
		}
		else if (r->type == TOKEN_REDIRECT_OUT)
		{
			if (!redirect_output(d, curr))
				exit(1);
		}
		else if (r->type == TOKEN_REDIRECT_APPEND)
		{
			if (!append_mode(d, curr))
				exit(1);
		}
		r = r->next;
	}
}

void	dup_if_there_is_pipe(t_command *curr, int *pipe_fd, int prev_fd)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			perror_and_exit("minishell: dup2");
		close(prev_fd);
	}
	if (curr)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror_and_exit("minishell: dup2");
		close(pipe_fd[1]);
	}
}

void	execution(t_env **env, t_command *cmds, char *prev_pwd, int *last_status)
{
	t_command *curr     = cmds;
	t_redirection	*r;
	char			*d;
	char			**envp;
	int				pipe_fd[2];
	int				prev_fd    = -1;
	bool			error      = false;
	pid_t			pid;

	if (is_builtins(curr->args) && !curr->next && !curr->redirections)
	{
		*last_status = builtins(env, curr->args, prev_pwd);
		return ;
	}
	while (curr)
	{
		if (curr->next && pipe(pipe_fd) == -1)
		{
			perror("minishell: pipe");
			error = true;
			break;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			error = true;
			exit(1);
		}
		if (pid == 0)
		{
			if (curr->next || prev_fd != -1)
				dup_if_there_is_pipe(curr->next, pipe_fd, prev_fd);
			d = check_if_exist(*env, curr);
			r = curr->redirections;
			redirection(curr, r, d);
			// while (r)
			// {
			// 	if (r->type == TOKEN_HEREDOC)
			// 	{
			// 		if (handle_heredoc(curr) == -1)
			// 			exit(1);
			// 	}
			// 	else if (r->type == TOKEN_REDIRECT_IN)
			// 	{
			// 		if (!redirect_input(d, curr))
			// 			exit(1);
			// 	}
			// 	else if (r->type == TOKEN_REDIRECT_OUT)
			// 	{
			// 		if (!redirect_output(d, curr))
			// 			exit(1);
			// 	}
			// 	else if (r->type == TOKEN_REDIRECT_APPEND)
			// 	{
			// 		if (!append_mode(d, curr))
			// 			exit(1);
			// 	}
			// 	r = r->next;
			// }
			if (is_builtins(curr->args))
			{
				*last_status = builtins(env, curr->args, prev_pwd);
				exit(0);
			}
			if (!d)
			{
				if (!strchr(curr->cmd, '/'))
					fprintf(stderr, "minishell: %s: command not found\n", curr->cmd);
				exit(127);
			}
			envp = build_env_array(env);
			if (execve(d, curr->args, envp) == -1)
			{
				free_envp(envp);
				perror("minishell: execve");
				exit(1);
			}
		}
		close_fd(prev_fd);
		if (curr->next)
		{
			close_fd(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		curr = curr->next;
	}
	close_fd(prev_fd);
	while (wait(NULL) > 0);
}
