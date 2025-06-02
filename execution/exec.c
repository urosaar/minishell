#include "../minishell.h"

char **build_env_array(t_env *env)
{
    int count = 0;
    t_env *tmp = env;
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
    return envp;
}

char *check_if_exist(t_env *env, t_command *cmds)
{
    int i;
    char **path;
    t_env *tmp;

    i = 0;
    path = NULL;
    tmp = env;
    if (!cmds->cmd)
        return NULL;
    if (access(cmds->cmd, X_OK) == 0)
        return ft_strdup(cmds->cmd);
    if (strchr(cmds->cmd, '/'))
    {
        printf("minishell: %s: No such file or directory\n", cmds->cmd);
        return NULL;
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
        return NULL;
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
    return NULL;
}

int ft_size_node(t_command *cmds)
{
    int i;
    t_command *tmp;

    i = 0;
    tmp = cmds;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

void execution(t_env *env, t_command *cmds, char *prev_pwd)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    char *d;
    char **envp;
    t_command *curr = cmds;

    if (is_builtins(curr->args) && !curr->next && !curr->outfile && !curr->infile)
    {
        builtins(&env, curr->args, prev_pwd);
        return;
    }
    while (curr)
    {
        if (curr->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return;
        }
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
//redirection-----------------------------------------------------------------------------
            if (curr->outfile && !curr->append)
            {
                int f = open(curr->outfile, O_CREAT | O_RDWR | O_TRUNC, 0777); 
                if (d || is_builtins(curr->args))
                {
                    dup2(f, STDOUT_FILENO);
                    close(f);
                }
            }
            if (curr->outfile && curr->append)
            {
                int f = open(curr->outfile, O_CREAT | O_RDWR | O_APPEND, 0777); 
                if (d || is_builtins(curr->args))
                {
                    dup2(f, STDOUT_FILENO);
                    close(f);
                }
            }
            if (curr->infile && !curr->heredoc)
            {
                int f = open(curr->infile, O_RDWR, 0777);
                if (f == -1)
                {
                    printf("minishell: %s: No such file or directory\n", curr->infile);
                    return;
                }
                if (d || is_builtins(curr->args))
                {
                    dup2(f, STDIN_FILENO);
                    close(f);
                }
            }
//redirection-----------------------------------------------------------------------------
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
