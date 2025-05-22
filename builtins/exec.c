#include "../minishell.h"

// void execution(t_env *env, t_command *cmds)
// {
//     int fd[2];
//     int exec;
//     int pid;
//     int i;
//     char **path = NULL;
//     char **args;
//     t_env *tmp;
//     t_env *tmp1;
//     char *s;
//     char *d;

//     args = cmds->args;
//     pipe(fd);
//     pid = fork();
//     tmp = env;
//     if (!pid == -1) 
//         return ;
//     if (pid == 0)
//     {
//         // char *argv[] = {"ls", NULL};
//         close(fd[0]);
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[1]);
//         // exec = execve("/bin/ls", argv, NULL);
//         while (tmp)
//         {
//             if (!strcmp(tmp->key, "PATH"))
//                 path = ft_split(tmp->value, ':');
//             tmp = tmp->next;
//         }
//         i = 0;
//         while (path && path[i])
//         {
//             s = ft_join("/", cmds->cmd);
//             d = ft_join(path[i], s);
//             if (access(d, X_OK) == 0)
//             {
//                 execve(d, cmds->args, NULL);
//                 free_array(path);
//             }
//             free(s);
//             free(d);
//             s = NULL;
//             d = NULL;
//             i++;
//         }
//         if (exec == -1)
//         {
//             write (2, "dhfkjshd\n", 6);
//             printf("could not find program to execute\n");
//             exit(2);
//         }
//         if (fork() == 0)
//         {
//             close(fd[1]);
//             dup2(fd[0], STDIN_FILENO);
//             close(fd[0]);
//             while (tmp1)
//             {
//                 if (!strcmp(tmp1->key, "PATH"))
//                     path = ft_split(tmp1->value, ':');
//                 tmp1 = tmp1->next;
//             }
//             i = 0;
//             while (path && path[i])
//             {
//                 s = ft_join("/", cmds->cmd);
//                 d = ft_join(path[i], s);
//                 if (access(d, X_OK) == 0)
//                 {
//                     execve(d, cmds->args, NULL);
//                     free_array(path);
//                 }
//                 free(s);
//                 free(d);
//                 s = NULL;
//                 d = NULL;
//                 i++;
//             }
//             if (exec == -1)
//             {
//                 write (2, "dhfkjshd\n", 6);
//                 printf("could not find program to execute\n");
//                 exit(2);
//             }
//         }
//     }
//     else
//     {
//         close(fd[0]);
//         close(fd[1]);
//         wait(NULL);
//         wait(NULL);
//     }
// }

// void exec_with_pipe(t_env *env, t_command *cmds)
// {
//     int i;
//     int exec;
//     int fd[2];
//     t_env *tmp;
//     char **path;
//     char *s;
//     char *d;

//     tmp = env;
//     close(fd[0]);
//     dup2(fd[1], STDOUT_FILENO);
//     close(fd[1]);
//     while (tmp)
//     {
//         if (!strcmp(tmp->key, "PATH"))
//             path = ft_split(tmp->value, ':');
//         tmp = tmp->next;
//     }
//     i = 0;
//     while (path && path[i])
//     {
//         s = ft_join("/", cmds->cmd);
//         d = ft_join(path[i], s);
//         if (access(d, X_OK) == 0)
//         {
//             if (execve(d, cmds->args, NULL) == -1)
//             {
//                 printf("could not find program to execute\n");
//                 exit(2);
//             }
//             free_array(path);
//         }
//         free(s);
//         free(d);
//         s = NULL;
//         d = NULL;
//         i++;
//     }
//     if (exec == -1)
//     {
//         printf("could not find program to execute\n");
//         exit(2);
//     }
// }

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
    // else
    // {
    //     if (fork() == 0)
    //     {
    //         close(fd[1]);
    //         dup2(fd[0], STDIN_FILENO);
    //         close(fd[0]);
    //         execlp("wc", "wc", "-l", NULL);
    //     }
    //     else
    //     {
    //         close(fd[0]);
    //         close(fd[1]);
    //         wait(NULL);
    //         wait(NULL);
    //     }
    // } 
}
