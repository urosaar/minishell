/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:00:18 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 18:08:43 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unclosed_quotes(const char *line)
{
	int		i;
	char	q;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\\' && line[i + 1] != '\0')
		{
			i += 2;
			continue ;
		}
		if (line[i] == '\'' || line[i] == '"')
		{
			q = line[i++];
			if (skip_quotedd(line, &i, q))
				return (1);
		}
		else
			i++;
	}
	return (0);
}
int	skip_quotedd(const char *line, int *i, char q)
{
	while (line[*i] && line[*i] != q)
	{
		if (q == '"' && line[*i] == '\\'
			&& (line[*i + 1] == '"' || line[*i + 1] == '\\'))
			*i += 2;
		else
			(*i)++;
	}
	if (line[*i] != q)
	{
		printf("minishell: syntax error: unexpected EOF "
			"while looking for matching `%c'\n", q);
		return (1);
	}
	(*i)++;
	return (0);
}

int	check_syntax_errors(char *line, char **tokens)
{
	if (check_unclosed_quotes(line))
		return (1);
	if (check_consecutive_operators(tokens))
		return (1);
	if (check_redirection_without_filename(tokens))
		return (1);
	if (check_invalid_pipe_placement(tokens))
		return (1);
	return (0);
}
