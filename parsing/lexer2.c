/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:14:01 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 18:15:33 by jesse            ###   ########.fr       */
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
