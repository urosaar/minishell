#include "minishell.h"

void ft_env(t_env **env)
{
    t_env *tmp;

    tmp = *env;
    while (tmp)
    {
        if (strchr(tmp->line, '='))
            printf("%s\n", tmp->line);
        tmp = tmp->next;
    }
    
}
