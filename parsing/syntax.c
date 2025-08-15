/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:10:07 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/12 21:10:09 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	redir_token(const char *tok)
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

int	check_consecutive_operators(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] && tokens[i + 1])
	{
		if ((is_pipe_token(tokens[i]) && is_pipe_token(tokens[i + 1]))
			|| (redir_token(tokens[i]) && redir_token(tokens[i + 1])))
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
		if (redir_token(tokens[i]))
		{
			if (!tokens[i + 1] || is_pipe_token(tokens[i + 1])
				|| redir_token(tokens[i + 1]))
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
