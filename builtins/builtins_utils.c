#include "../minishell.h"

void copy_env(char **envp, t_env **env) 
{
    int i = 0;
    t_env *new_node;

    while (envp[i])
    {
        new_node = ft_newnode(strdup(envp[i]), '=');
        ft_lstadd_back(env, new_node);
        i++;
    }
}

t_env *copy_for_expo(t_env *env)
{
    t_env *copy = NULL;
    t_env *tmp;
    char *value;

    while (env)
    {
        tmp = ft_newnode(strdup(env->line), '=');
        ft_lstadd_back(&copy, tmp);
        env = env->next;
    }
    return copy;
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        if (env->line)
        free(env->line);
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void check_for_pwd(char **prev_pwd)
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    int i = chdir(pwd);
    if (!i)
        *prev_pwd = strdup(pwd);
    if (i == -1)
        return;
}
