/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:03:10 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/31 19:36:00 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redir_token(const char *tok)
{
	return (!ft_strcmp(tok, "<")
		|| !ft_strcmp(tok, "<<")
		|| !ft_strcmp(tok, ">")
		|| !ft_strcmp(tok, ">>"));
}

static int	is_pipe_token(const char *tok)
{
	return (tok[0] == '|' && tok[1] == '\0');
}

static int	skip_quoted(const char *line, int *i, char q)
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
			if (skip_quoted(line, &i, q))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	check_consecutive_operators(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] && tokens[i + 1])
	{
		if ((is_pipe_token(tokens[i]) && is_pipe_token(tokens[i + 1]))
			|| (is_redir_token(tokens[i]) && is_redir_token(tokens[i + 1])))
		{
			printf("minishell: syntax error near unexpected token `%s'\n",
				tokens[i + 1]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_redirection_without_filename(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_redir_token(tokens[i]))
		{
			if (!tokens[i + 1] || is_pipe_token(tokens[i + 1])
				|| is_redir_token(tokens[i + 1]))
			{
				if (!tokens[i + 1])
					printf("minishell: syntax error near unexpected token "
						"`newline'\n");
				else
					printf(
						"minishell: syntax error near unexpected token `%s'\n",
						tokens[i + 1]);
				return (1);
			}
			i += 2;
		}
		else
			i++;
	}
	return (0);
}

int	check_invalid_pipe_placement(char **tokens)
{
	int	last;

	last = 0;
	if (!tokens[0])
		return (0);
	if (is_pipe_token(tokens[0]))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	while (tokens[last + 1])
		last++;
	if (is_pipe_token(tokens[last]))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
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
