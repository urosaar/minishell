#include "minishell.h"

void ft_cd(t_env **env, char **arg)
{
    int i;
    t_env *node;
    char *oldpath;
    char *path;

    node =*env;
    oldpath = getcwd(NULL, 0);
    i = chdir(arg[1]);
    path = getcwd(NULL, 0);
    if (i == -1 && arg[1])
        printf("bash: cd: %s: No such file or directory\n", arg[1]);
    else if (i == 0 && arg[1])
    {
        while (node)
        {
            if (node->line && !(strcmp(node->key, "PWD")))
            {
                free(node->line);
                if (node->value)
                    free(node->value);
                node->value = strdup(path);
                node->line = ft_join(ft_strdup("PWD="), node->value);
            }
            else if (node->line && !(strcmp(node->key, "OLDPWD")))
            {
                free(node->line);
                if (node->value)
                    free(node->value);
                node->value = strdup(oldpath);
                node->line = ft_join(ft_strdup("OLDPWD="), node->value);
            }
            node = node->next;
        }
    }
}
