#include "../minishell.h"

void ft_pwd(t_env *env, char *prev_pwd)
{
    t_env *node;
    char *pwd;
    int i = 0;

    pwd = NULL;
    node = env;
    pwd = getcwd(NULL,0);
    if (!pwd)
        pwd = strdup(prev_pwd);
    while (pwd[i])
        i++;
    write(1, pwd, i);
    write(1, "\n", 1);
}

