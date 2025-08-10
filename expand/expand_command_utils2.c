/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command2_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 05:16:47 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/10 05:19:46 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int count_unquoted_tokens(const char *s)
{
    t_locals v;
    ft_bzero(&v, sizeof(v));

    while (s[v.i])
    {
        while (s[v.i] && s[v.i] == ' ')
            v.i++;
        if (!s[v.i])
            break;
        v.count++;
        while (s[v.i] && ((v.in_single || v.in_double) || s[v.i] != ' '))
        {
            if (s[v.i] == '\'' && !v.in_double)
                v.in_single = !v.in_single;
            else if (s[v.i] == '"' && !v.in_single)
                v.in_double = !v.in_double;
            v.i++;
        }
    }
    return (v.count);
}

static void re_init_split_unquoted(t_locals *v)
{
    v->start = v->i;
    v->in_single = 0;
    v->in_double = 0;
}

char **split_unquoted_tokens(const char *s)
{
    t_locals v;

    ft_bzero(&v, sizeof(v));
    if (!s)
        return (NULL);
    v.tokens = count_unquoted_tokens(s);
    v.res = malloc(sizeof(char *) * (v.tokens + 1));
    if (!v.res)
        return (NULL);
    while (s[v.i] && v.t < v.tokens)
    {
        while (s[v.i] && s[v.i] == ' ')
            v.i++;
        re_init_split_unquoted(&v);
        while (s[v.i] && ((v.in_single || v.in_double) || s[v.i] != ' '))
        {
            if (s[v.i] == '\'' && !v.in_double)
                v.in_single = !v.in_single;
            else if (s[v.i] == '\"' && !v.in_single)
                v.in_double = !v.in_double;
            v.i++;
        }
        v.res[v.t++] = ft_strndup(s + v.start, (size_t)(v.i - v.start));
    }
    return (v.res[v.t] = NULL, v.res);
}

int has_expandable(const char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}
