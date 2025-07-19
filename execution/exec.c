/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:34 by skhallou          #+#    #+#             */
/*   Updated: 2025/07/19 06:29:12 by oukhanfa         ###   ########.fr       */
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
	char **envp = ft_malloc((count + 1) * sizeof(char *), MALLOC);
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

char *ft_check1(char *cmd)
{
    int   dirfd;

    if (!cmd)
        return (NULL);
    dirfd = open(cmd, O_DIRECTORY);
    if (dirfd >= 0)
    {
        close(dirfd);
        fprintf(stderr, "minishell: %s: is a directory\n", cmd);
        return (NULL);
    }

    if (access(cmd, X_OK) == 0)
        return (strdup(cmd));

    if (strchr(cmd, '/'))
        fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);

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
	d = ft_check1(cmds->cmd);
	if (d)
		return (d);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PATH"))
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

int redirect_input(char *filename)
{
    int f = open(filename, O_RDONLY);
    if (f == -1)
	{
        fprintf(stderr, "minishell: %s: No such file or directory\n", filename);
        return 0;
    }
    if (dup2(f, STDIN_FILENO) == -1)
	{ 
        perror("minishell: dup2");
        close(f);
        return 0;
    }
    close(f);
    return 1;
}
int append_mode(const char *filename)
{
    int f = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (f == -1) {
        perror("minishell: append_mode");
        return 0;
    }
    if (dup2(f, STDOUT_FILENO) == -1)
	{ 
        perror("minishell: dup2");
        close(f);
        return 0; 
    }
    close(f);
    return 1; 
}
int redirect_output(const char *filename)
{
    int f = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (f == -1) {
        perror("minishell: redirect_output");
        return 0; 
    }
    if (dup2(f, STDOUT_FILENO) == -1)
	{ 
        perror("minishell: dup2");
        close(f);
        return 0;
    }
    close(f);
    return 1;
}


void handler_heredoc()
{
	write(1, "\n", 1);
	exit(1);
}


int handle_heredoc(char *delimiter, int quoted, int last_status, t_env **env)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return (-1);
    
    pid_t pid = fork();
    if (pid == -1)
	{
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    
    if (pid == 0)
	{
        signal(SIGINT, handler_heredoc);
        signal(SIGQUIT, SIG_IGN);
        close(pipefd[0]);
        
        char *line;
        int line_count = 0;
        while (1) {
            if (line_count >= HEREDOC_MAX_LINES)
			{
                ft_putstr_fd("minishell: heredoc limit exceeded\n", STDERR_FILENO);
                close(pipefd[1]);
                exit(1);
            }
            
            line = readline("> ");
              if (!line)
            {
                // write(STDOUT_FILENO, CTRLD, 7);  // Move cursor up and right
                break;
            }
            
            if (ft_strcmp(line, delimiter) == 0)
			{
                free(line);
                break;
            }
            
            if (!quoted)
			{
                char *expanded = expand_variables(line, last_status, env);
                if (expanded)
				{
                    write(pipefd[1], expanded, ft_strlen(expanded));
                    write(pipefd[1], "\n", 1);
                    free(expanded);
                } else
				{
                    write(pipefd[1], line, ft_strlen(line));
                    write(pipefd[1], "\n", 1);
                }
            } 
			else
			{
                write(pipefd[1], line, ft_strlen(line));
                write(pipefd[1], "\n", 1);
            }
            
            free(line);
            line_count++;
        }
        close(pipefd[1]);
        exit(0);
    } 
	else 
	{
        int status;
        close(pipefd[1]);
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        
        waitpid(pid, &status, 0);
        
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        if (WIFEXITED(status))
		{
            if (WEXITSTATUS(status) != 0) 
			{
                close(pipefd[0]);
                return -1;
            }
        } 
		else if (WIFSIGNALED(status))
		{
            close(pipefd[0]);
            return -1;
        }
        return pipefd[0];
    }
}
 int process_all_heredocs(t_command *cmds, int last_status, t_env **env)
 {
     t_command     *curr = cmds;
     while (curr)
     {
         t_redirection *redir = curr->redirections;
         while (redir)
         {
           if (redir->type == TOKEN_HEREDOC)
		   {
                redir->heredoc_fd = handle_heredoc(redir->filename,curr->heredoc_quoted,last_status,env);
               if (redir->heredoc_fd < 0)
                    return 0;  
             }
             redir = redir->next;
         }
         curr = curr->next;
     }
     return 1; 
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

void	ft_execve(t_command *curr, t_env **env, char *d)
{
	char	**envp;

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
int apply_redirection(t_command *curr) {
    t_redirection *tmp = curr->redirections;
    while (tmp) {
        if (tmp->type == TOKEN_HEREDOC && tmp->heredoc_fd >= 0) {
            if (dup2(tmp->heredoc_fd, STDIN_FILENO) == -1) {
                perror("minishell: dup2 heredoc");
                return 0;
            }
            close(tmp->heredoc_fd);
            tmp->heredoc_fd = -1;
        }
        else if (tmp->type == TOKEN_REDIRECT_IN) {
            if (!redirect_input(tmp->filename))
                return 0;
        }
        else if (tmp->type == TOKEN_REDIRECT_OUT) {
            if (!redirect_output(tmp->filename))
                return 0;
        }
        else if (tmp->type == TOKEN_REDIRECT_APPEND) {
            if (!append_mode(tmp->filename))
                return 0;
        }
        tmp = tmp->next;
    }
    return 1;
}



void	creat_a_child(t_command *curr, t_env **env, t_exec *ctx)
{
	char	*d = NULL;

	ctx->pid = fork();
	if (ctx->pid == -1) {
		perror("fork");
		exit(1);
	}

	if (ctx->pid == 0) {
		if (curr->next || ctx->prev_fd != -1)
			dup_if_there_is_pipe(curr->next, ctx->pipe_fd, ctx->prev_fd);
		
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		
		if (!apply_redirection(curr))
			exit(1);

		if (curr->cmd == NULL || curr->cmd[0] == '\0') {
			fprintf(stderr, "minishell: : command not found\n");
			exit(127);
		}
		
		d = check_if_exist(*env, curr);
		if (is_builtins(curr->args)) {
			exit(builtins(env, curr->args, ctx->prev_pwd));
		}
		
		ft_execve(curr, env, d);
	}
	else {
		close_fd(ctx->prev_fd);
		ctx->last_pid = ctx->pid;
		
		if (curr->next) {
			close_fd(ctx->pipe_fd[1]);
			ctx->prev_fd = ctx->pipe_fd[0];
		}
	}
}
void	ft_wait(t_exec *ctx)
{
	int	status;
	int	x;

	while ((x = wait(&status)) > 0)
	{
		if (x == ctx->last_pid)
		{
			if (WIFEXITED(status)) 
				ctx->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status) && g_status == 0) 
			{
				ctx->last_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				if (WTERMSIG(status) == SIGQUIT)
					printf("Quit: 3\n");
        	}
		}
	}
	// printf("STATUS = %d\n", ctx->last_status);
}

void execution(t_command *cmds, t_env **env, t_exec *ctx) 
{
	if (!process_all_heredocs(cmds, ctx->last_status, env))
	{
        ctx->last_status = 1;
        return;
    }
    t_command	*curr;

	ctx->prev_fd = -1;
	curr = cmds;
	signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    if (is_builtins(curr->args) && !curr->next)
	{
        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);
        int redir_success = 1;

        if (saved_stdin == -1 || saved_stdout == -1)
		{
            perror("minishell: dup");
            redir_success = 0;
        }
        if (redir_success)
            redir_success = apply_redirection(curr);
        if (redir_success)
            ctx->last_status = builtins(env, curr->args, ctx->prev_pwd);
		 else 
            ctx->last_status = 1;
        if (saved_stdin != -1)
		{
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }
        if (saved_stdout != -1)
		{
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
        t_redirection *r = curr->redirections;
        while (r)
		{
            if (r->type == TOKEN_HEREDOC && r->heredoc_fd != -1)
			{
                close(r->heredoc_fd);
                r->heredoc_fd = -1;
            }
            r = r->next;
        }
        return;
    }
	while (curr)
	{
		if (curr->next && pipe(ctx->pipe_fd) == -1)
		{
			perror("pipe");
			break;
		}
		creat_a_child(curr, env, ctx);
		curr = curr->next;
	}
	close_fd(ctx->prev_fd);
	ft_wait(ctx);
}
