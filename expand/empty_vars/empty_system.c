#include "../../minishell.h"

void    free_list(char **list)
{
    int i;

    i = 0;
    while (list[i])
    {
        free(list[i]);
        i++;
    }
    free(list);
}

// used in empty system
bool empties_inside(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == (char)127)
            return (true);
        i++;
    }
    return (false);
}

bool one_empty(char *str)
{
    if (str[0] == (char)127 && str[1] == '\0')
        return (true);
    return (false);
}

char    **empty_system(char **argv)
{
    t_empty   *args;
    char    **new_argv;
    int     i;

    args = NULL;
    i = 0;
    while (argv[i])
    {
        if (!one_empty(argv[i]))
        {
            if (add_empty_back(&args, argv[i]) != EXIT_SUCCESS)
                return (free_empty_list(args), free_list(argv), NULL);
        }
        i++;
    }
    new_argv = empty_list_to_argv(args);
    if (!new_argv)
        return (free_empty_list(args),  free_list(argv), NULL);
    return(free_empty_list(args), free_list(argv), new_argv);
}
