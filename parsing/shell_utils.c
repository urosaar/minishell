/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:02:33 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/03 00:44:31 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_input(void)
{
	char	*line;

	line = readline("minishell> ");
	if (line && *line)
		add_history(line);
	return (line);
}

static void	copy_without_quotes(const char *str, char *out)
{
	int		i;
	int		j;
	char	current_quote;

	i = 0;
	j = 0;
	current_quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && current_quote == 0)
			current_quote = str[i++];
		else if (str[i] == current_quote)
		{
			current_quote = 0;
			i++;
		}
		else
			out[j++] = str[i++];
	}
	out[j] = '\0';
}

char	*strip_quotes(const char *str)
{
	int		len;
	char	*out;

	len = (int)ft_strlen(str);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	copy_without_quotes(str, out);
	return (out);
}

char	*append_char(char *result, int *rlen, char c)
{
	char	*tmp;

	tmp = malloc(*rlen + 2);
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	memcpy(tmp, result, *rlen);
	tmp[*rlen] = c;
	(*rlen)++;
	tmp[*rlen] = '\0';
	free(result);
	return (tmp);
}

char	*ft_getenv(const char *name, t_env *env)
{
	if (!name || !env)
		return (NULL);
	while (env != NULL)
	{
		if (!ft_strcmp(env->key, name))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	copy_spliced_tokens(char **tokens, char **pieces,
				char **out, int pos)
{
	int	i;
	int	j;
	int	out_i;

	i = 0;
	j = 0;
	out_i = 0;
	while (i < pos)
		out[out_i++] = tokens[i++];
	while (pieces[j])
		out[out_i++] = pieces[j++];
	i = pos + 1;
	while (tokens[i])
		out[out_i++] = tokens[i++];
	out[out_i] = NULL;
}

char	**splice_tokens(char **tokens, int pos, char **pieces)
{
	int		old_n;
	int		new_n;
	char	**out;

	old_n = 0;
	while (tokens[old_n])
		old_n++;
	new_n = 0;
	while (pieces[new_n])
		new_n++;
	out = malloc(sizeof(char *) * (old_n - 1 + new_n + 1));
	if (!out)
		return (NULL);
	copy_spliced_tokens(tokens, pieces, out, pos);
	free(tokens);
	return (out);
}
