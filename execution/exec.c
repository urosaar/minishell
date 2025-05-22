#include "../minishell.h"

char *check_if_exist(t_env *env, t_command *cmds)
{
    char **path = NULL;
    char **args;
    t_env *tmp;
    char *s;
    char *d;
    int i;
    int flag;

    args = cmds->args;
    tmp = env;
    if (access(cmds->cmd, X_OK) == 0)
        return (cmds->cmd);
    else if (access(cmds->cmd, X_OK) == -1 && strchr(cmds->cmd, '/'))
    {
        printf("minishell: %s: No such file or directory\n", cmds->cmd);
        return (NULL);
    }
    while (tmp)
    {
        if (!strcmp(tmp->key, "PATH"))
            path = ft_split(tmp->value, ':');
        tmp = tmp->next;
    }
    i = 0;
    while (path && path[i])
    {
        s = ft_join("/", cmds->cmd);
        d = ft_join(path[i], s);
        if (access(d, X_OK) == 0)
            return (d);
        free(s);
        free(d);
        s = NULL;
        d = NULL;
        i++;
    }
    if (!path || !path[i])
        return (ft_strdup(cmds->cmd));
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

void execution(t_env *env, t_command *cmds)
{
    int fd[2];
    int pid;
    int i;
    char **path = NULL;
    char *d;

    pipe(fd);
    d = check_if_exist(env, cmds);
    if (d)
    {
        pid = fork();
        if (pid == -1) 
            return ;
        if (pid == 0)
        {
            if (ft_size_node(cmds) == 1)
            {
                if (execve(d, cmds->args, NULL) == -1)
                    {
                        printf("minishell: %s: command not found\n", cmds->cmd);
                        exit(2);
                    }
                free_array(path);
            }
            else
                printf("HERE\n");
        }
        else
            waitpid(pid, NULL, 0);
    }
    else
        return; 
}
