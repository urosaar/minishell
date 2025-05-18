#include "minishell.h"

void ft_pwd(void)
{
    char *pwd;
    int i = 0;

    pwd = getcwd(NULL,0);
    if (!pwd)
        return;
    while (pwd[i])
        i++;
    write(1, pwd, i);
    write(1, "\n", 1);
}

