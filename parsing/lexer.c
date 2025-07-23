/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 09:40:34 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/22 11:02:01 by oukhanfa         ###   ########.fr       */
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
		return (substr(input, start, *i));
	}
	return (substr(input, (*i)++, *i));
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

char	*extract_word(const char *input, int *i)
{
	int		start;
	bool	in_quote;
	char	quote_char;

	start = *i;
	quote_char = 0;
	in_quote = false;
	while (input[*i])
	{
		if (!in_quote && input[*i] == '\\' && input[*i + 1])
			*i += 2;
		else
		{
			in_quote = is_in_quote(in_quote, &quote_char, input[*i]);
			if (!in_quote && (is_space(input[*i]) || is_operator(input[*i])))
				break ;
			(*i)++;
		}
	}
	return (substr(input, start, *i));
}

static void	skip_quoted(const char *input, int *i)
{
	char	quote;
	bool	escaped;

	quote = input[(*i)++];
	while (input[*i])
	{
		escaped = (input[*i] == '\\'
				&& (input[*i + 1] == quote || input[*i + 1] == '\\'));
		if (escaped)
			*i += 2;
		else if (input[*i] == quote)
		{
			(*i)++;
			break;
		}
		else
			(*i)++;
	}
}

static void	process_token(const char *input, int *i)
{
	if (input[*i] == '\'' || input[*i] == '"')
		skip_quoted(input, i);
	else if (is_operator(input[*i]))
	{
		(*i)++;
		if ((input[*i - 1] == '<' && input[*i] == '<')
			|| (input[*i - 1] == '>' && input[*i] == '>'))
			(*i)++;
	}
	else
	{
		while (input[*i] && !is_space(input[*i])
			&& !is_operator(input[*i]))
			(*i)++;
	}
}

int	count_tokens(const char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		while (is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		process_token(input, &i);
		count++;
	}
	return (count);
}

char	**lexer(const char *input)
{
	int		i;
	int		j;
	char	**tokens;
	int		tok_count;

	i = 0;
	j = 0;
	tok_count = count_tokens(input);
	tokens = malloc(sizeof(char *) * (tok_count + 1));
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		while (is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_operator(input[i]))
			tokens[j++] = extract_operator(input, &i);
		else
			tokens[j++] = extract_word(input, &i);
	}
	tokens[j] = NULL;
	return (tokens);
}

