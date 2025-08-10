/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:14:26 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 19:08:23 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_var_exp(t_state *st)
{
	st->idx++;
	if (st->in[st->idx] == '?')
		return (handle_exit_status(st));
	else
		return (handle_env_var(st));
}

static int	process_character(t_state *st)
{
	char	c;

	if (st->in[0] == '\0')
		return (0);
	c = st->in[st->idx];
	if (c == '\'' && !st->in_double)
	{
		st->in_single = !st->in_single;
		st->idx++;
		return (1);
	}
	else if (c == '"' && !st->in_single)
	{
		st->in_double = !st->in_double;
		st->idx++;
		return (1);
	}
	else if (c == '$' && !st->in_single)
		return (handle_var_exp(st));
	if (!str_append_char(st, c))
		return (0);
	st->idx++;
	return (1);
}

char	*expand_variables(const char *input, int last_status, t_env **env)
{
	t_state	st;
	char	*ret;

	ft_bzero(&st, sizeof(t_state));
	st.in = input;
	st.last_status = last_status;
	st.env = env;
	st.res = malloc(2);
	if (!st.res)
		return (NULL);
	st.res[0] = (char)127;
	st.res[1] = '\0';
	while (st.in[st.idx])
	{
		if (!process_character(&st))
			return (free(st.res), NULL);
	}
	if (!has_expandable(st.in) && empties_inside(st.res))
	{
		ret = ft_substr(st.res, 1, ft_strlen(st.res));
		return (free(st.res), ret);
	}
	return (st.res);
}

static void	expand_token_if_needed(char **token, int last_status, t_env **env)
{
	char	*expanded;

	expanded = expand_variables(*token, last_status, env);
	if (expanded)
	{
		free(*token);
		*token = expanded;
	}
}

void	expand_tokens(char **tokens, int last_status, t_env **env)
{
	int	i;
	int	skip_next;

	i = 0;
	skip_next = 0;
	while (tokens && tokens[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (ft_strcmp(tokens[i], "<<") == 0)
			skip_next = 1;
		else
			expand_token_if_needed(&tokens[i], last_status, env);
		i++;
	}
}
