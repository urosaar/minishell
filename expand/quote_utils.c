/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 06:16:02 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/16 06:26:52 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_assignment(char *str)
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

bool	is_simple_dollar_var_str(const char *s)
{
	int	i;

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

static bool	remove_start_quotes(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len >= 2 && str[0] == '"' && str[1] == '"')
	{
		ft_memmove(str, str + 2, len - 1);
		return (true);
	}
	if (len >= 2 && str[0] == '\'' && str[1] == '\'')
	{
		ft_memmove(str, str + 2, len - 1);
		return (true);
	}
	return (false);
}

static bool	remove_end_quotes(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len >= 2 && str[len - 2] == '"' && str[len - 1] == '"')
	{
		str[len - 2] = '\0';
		return (true);
	}
	if (len >= 2 && str[len - 2] == '\'' && str[len - 1] == '\'')
	{
		str[len - 2] = '\0';
		return (true);
	}
	return (false);
}

char	*strip_empty_quotes(const char *s)
{
	char	*str;
	bool	changed;

	if (!s)
		return (NULL);
	str = ft_strdup(s);
	if (!str)
		return (NULL);
	changed = true;
	while (changed)
	{
		changed = false;
		if (remove_start_quotes(str) || remove_end_quotes(str))
			changed = true;
	}
	return (str);
}
