/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:39:54 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/16 05:50:49 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*apend_char(char *buf, char c)
{
	size_t	len;
	char	*new;

	if (buf)
		len = ft_strlen(buf);
	else
		len = 0;
	new = malloc(len + 2);
	if (!new)
	{
		free(buf);
		return (NULL);
	}
	if (buf)
	{
		ft_memcpy(new, buf, len);
		free(buf);
	}
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}

char	*append_str(char *buf, const char *s)
{
	size_t	lenb;
	size_t	lens;
	char	*new;

	if (!s || s[0] == '\0')
		return (buf);
	if (!buf)
		return (ft_strdup(s));
	lenb = ft_strlen(buf);
	lens = ft_strlen(s);
	new = malloc(lenb + lens + 1);
	if (!new)
	{
		free (buf);
		return (NULL);
	}
	ft_memcpy(new, buf, lenb);
	ft_memcpy(new + lenb, s, lens + 1);
	free (buf);
	return (new);
}

static char	*handle_special_dollar(char *out, size_t *i, t_expand_ctx *ctx)
{
	char	*tok;
	char	*exp;

	tok = ft_strdup("$?");
	if (!tok)
	{
		free(out);
		return (NULL);
	}
	exp = expand_variables(tok, ctx->last_status, ctx->env);
	free(tok);
	if (exp)
	{
		out = append_str(out, exp);
		free(exp);
	}
	*i += 2;
	return (out);
}

static char	*handle_variable_dollar(char *out, const char *line, size_t *i,
	t_expand_ctx *ctx)
{
	size_t	j;
	size_t	toklen;
	char	*tok;
	char	*exp;

	j = *i + 1;
	while (line[j] && ((line[j] >= 'A' && line[j] <= 'Z')
			|| (line[j] >= 'a' && line[j] <= 'z')
			|| (line[j] >= '0' && line[j] <= '9')
			|| line[j] == '_'))
		j++;
	toklen = j - *i;
	tok = ft_strndup(line + *i, toklen);
	if (!tok)
		return (free(out), NULL);
	exp = expand_variables(tok, ctx->last_status, ctx->env);
	free(tok);
	if (exp)
	{
		out = append_str(out, exp);
		free(exp);
	}
	*i = j;
	return (out);
}

char	*handle_dollar(char *out, const char *line, size_t *i,
	t_expand_ctx *ctx)
{
	if (line[*i + 1] == '?')
		return (handle_special_dollar(out, i, ctx));
	if ((line[*i + 1] >= 'A' && line[*i + 1] <= 'Z')
		|| (line[*i + 1] >= 'a' && line[*i + 1] <= 'z')
		|| (line[*i + 1] == '_'))
		return (handle_variable_dollar(out, line, i, ctx));
	out = apend_char(out, '$');
	(*i)++;
	return (out);
}
