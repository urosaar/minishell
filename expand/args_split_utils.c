/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_split_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:07:24 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/15 19:50:52 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**split_selected_args(char **args, bool *no_split)
{
	int		i;
	int		k;
	char	**pieces;

	i = 0;
	while (args[i])
	{
		if (!no_split[i] && ft_strchr(args[i], ' '))
		{
			pieces = ft_split(args[i], ' ');
			free(args[i]);
			args = splice_tokens(args, i, pieces);
			free(pieces);
			k = 0;
			while (args[i + k])
				k++;
			i += k;
		}
		else
			i++;
	}
	return (args);
}

int	count(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

static bool	is_assignment(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			break ;
		i++;
	}
	return (false);
}

void	free_strarray(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static bool is_simple_dollar_var_str(const char *s)
{
    int i;

    if (!s)
        return (false);
    if (s[0] != '$')
        return (false);
    if (s[1] == '?')
        return (s[2] == '\0');
    i = 1;
    if (!(ft_isalpha((unsigned char)s[i]) || s[i] == '_'))
        return (false);
    i++;
    while (s[i])
    {
        if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
            return (false);
        i++;
    }
    return (true);
}

static char *strip_empty_quotes(const char *s)
{
    char *str;
    int len;
    int changed;

    if (!s)
        return (NULL);
    str = ft_strdup(s);
    if (!str)
        return (NULL);
    changed = 1;
    while (changed)
    {
        changed = 0;
        len = ft_strlen(str);
        if (len >= 2 && str[0] == '"' && str[1] == '"')
        {
            memmove(str, str + 2, len - 1);
            changed = 1;
            continue;
        }
        if (len >= 2 && str[0] == '\'' && str[1] == '\'')
        {
            memmove(str, str + 2, len - 1);
            changed = 1;
            continue;
        }
        len = ft_strlen(str);
        if (len >= 2 && str[len-2] == '"' && str[len-1] == '"')
        {
            str[len-2] = '\0';
            changed = 1;
            continue;
        }
        if (len >= 2 && str[len-2] == '\'' && str[len-1] == '\'')
        {
            str[len-2] = '\0';
            changed = 1;
            continue;
        }
    }
    return (str);
}
bool    *create_no_split_map(char **args)
{
    bool    *no_split;
    int     idx;
    int     len;
    int     orig_argc;

    orig_argc = 0;
    while (args[orig_argc])
        orig_argc++;
    no_split = ft_calloc(orig_argc + 1, sizeof(bool));
    if (!no_split)
        return (NULL);
    idx = 0;
    while (idx < orig_argc)
    {
        len = ft_strlen(args[idx]);
        no_split[idx] = (len >= 2 && ((args[idx][0] == '"'
                        && args[idx][len - 1] == '"') || (args[idx][0] == '\''
                        && args[idx][len - 1] == '\'')));
        if (!no_split[idx] && (ft_isalpha((unsigned char)args[idx][0]) || args[idx][0] == '_'))
            no_split[idx] = is_assignment(args[idx]);

        if (no_split[idx])
        {
            char *stripped = strip_empty_quotes(args[idx]);
            if (stripped)
            {
                if (is_simple_dollar_var_str(stripped) || stripped[0] == '\0')
                    no_split[idx] = false;
                free(stripped);
            }
        }
        idx++;
    }
    return (no_split);
}
