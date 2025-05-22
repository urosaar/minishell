#include "../minishell.h"

void check_option(char **splited, int *flag, int *t)
{
    int i = 1;
    int j = 0;
    
    while (splited[i] && splited[i][j] && splited[i][j] == '-')
    {
        while (splited[i][++j])
        {
            if (splited[i][j] == 'n')
            {
                if (splited[i][j + 1] == '\0')
                {
                    (*t)++;
                    *flag = 0;
                }
            }
            else
                return;
        } 
        if (!splited[i][j] && splited[i][j - 1] == '-')
            return;
        j = 0;
        i++;
    }
}

void ft_echo(char **splited)
{
    int i = 1;
    int flag = 1;

    check_option(splited, &flag, &i);
    while (splited[i])
    {
        write(1, splited[i], ft_strlen(splited[i]));
        if (splited[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (flag)
        write(1, "\n", 1);
}
