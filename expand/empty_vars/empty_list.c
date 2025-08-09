#include "../../minishell.h"

static size_t empty_list_size(t_empty *curr)
{
    size_t size;

    size = 0;
    while (curr)
    {
        size++;
        curr = curr->next;
    }
    return (size);
}

void free_empty_list(t_empty *empty)
{
    t_empty   *tmp;

    if (!empty)
        return ;
    while (empty)
    {
        tmp = empty->next;
        free(empty->value);
        free(empty);
        empty = tmp;
    }
}
// gets used in empty system
int  add_empty_back(t_empty **head, char *str)
{
    t_empty   *new;
    t_empty   *curr;

    new = malloc (sizeof(t_empty));
    if (!new)
        return (EXIT_FAILURE);
    new->value = ft_strdup(str);
    if (!new->value)
        return (free(new), EXIT_FAILURE);
    new->next = NULL;
    if (!*head)
        *head = new;
    else
    {
        curr = *head;
        while (curr->next)
            curr = curr->next;
        curr->next = new;
    }
    return (EXIT_SUCCESS);
}

char **empty_list_to_argv(t_empty *head)
{
    size_t  size;
    t_empty   *curr;
    char    **argv;
    size_t  i;

    i = 0;
    size = empty_list_size(head);
    argv = malloc(sizeof(char *) * (size + 1));
    if (!argv)
        return (NULL);
    curr = head;
    while (i < size)
    {
        argv[i] = ft_strdup(curr->value);
        if (!argv[i])
        {
            while (--i >= 0)
                free(argv[i]);
            return (free(argv), NULL);
        }
        curr = curr->next;
        i++;
    }
    argv[i] = NULL;
    return (argv);
}