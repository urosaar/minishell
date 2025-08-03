/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buffer_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:12:52 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 15:12:13 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	grow_buffer(t_state *st, int extra)
{
	char	*new_buf;

	new_buf = malloc(st->rlen + extra + 1);
	if (!new_buf)
	{
		free(st->res);
		return (0);
	}
	if (st->res)
	{
		ft_memcpy(new_buf, st->res, st->rlen);
		free(st->res);
	}
	st->res = new_buf;
	st->res[st->rlen + extra] = '\0';
	return (1);
}

int	str_append_char(t_state *st, char c)
{
	if (!grow_buffer(st, 1))
		return (0);
	st->res[st->rlen++] = c;
	return (1);
}

static int	insert_string(t_state *st, const char *s, int len)
{
	if (!len)
		return (1);
	if (!grow_buffer(st, len))
		return (0);
	memcpy(st->res + st->rlen, s, len);
	st->rlen += len;
	return (1);
}

int	handle_exit_status(t_state *st)
{
	char	*num;
	int		len;
	int		success;

	num = ft_itoa(st->last_status);
	if (!num)
		return (0);
	len = ft_strlen(num);
	success = insert_string(st, num, len);
	free(num);
	st->idx++;
	return (success);
}

int	handle_env_var(t_state *st)
{
	int		start;
	int		varlen;
	char	*var_name;
	char	*val;

	start = st->idx;
	varlen = 0;
	while (ft_isalnum((unsigned char)st->in[st->idx])
		|| st->in[st->idx] == '_')
	{
		st->idx++;
		varlen++;
	}
	if (varlen == 0)
		return (str_append_char(st, '$'));
	var_name = ft_strndup(st->in + start, varlen);
	if (!var_name)
		return (0);
	val = ft_getenv(var_name, *st->env);
	free(var_name);
	if (val)
		return (insert_string(st, val, ft_strlen(val)));
	return (1);
}
