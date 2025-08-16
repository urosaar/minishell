/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:09:23 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/16 08:01:32 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_operator(const char *input, int *i)
{
	int	start;

	if ((input[*i] == '<' && input[*i + 1] == '<')
		|| (input[*i] == '>' && input[*i + 1] == '>'))
	{
		start = *i;
		*i += 2;
		return (ft_substr(input, start, *i));
	}
	return (ft_substr(input, (*i)++, *i));
}

bool	is_in_quote(bool in_quote, char *quote_char, char current_char)
{
	if (!in_quote && (current_char == '\'' || current_char == '"'))
	{
		*quote_char = current_char;
		return (true);
	}
	else if (in_quote && current_char == *quote_char)
	{
		return (false);
	}
	return (in_quote);
}

bool	contains_whitespace(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (is_space(s[i]))
			return (true);
		i++;
	}
	return (false);
}

void	normalize_tabs_to_spaces(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\t')
			s[i] = ' ';
		i++;
	}
}
