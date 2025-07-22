/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:02:33 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/22 11:02:36 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
# include<ctype.h>

char *get_input(void)
{
    char *line = readline("minishell> ");
    if (line && *line)
        add_history(line);
    return (line);
}

char *strip_quotes(const char *str)
{
    int len = (int)ft_strlen(str);
    char *out = malloc(len + 1);
    int  i = 0, j = 0;
    char current_quote = 0;

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && current_quote == 0)
        {
            current_quote = str[i++];
        }
        else if (str[i] == current_quote)
        {
            current_quote = 0;
            i++;
        }
        else
        {
            out[j++] = str[i++];
        }
    }
    out[j] = '\0';
    return (out);
}
char *append_char(char *result, int *rlen, char c)
 {
    char *tmp = malloc(*rlen + 2);
    if (!tmp) {
        free(result);
        return NULL;
    }
    memcpy(tmp, result, *rlen);
    tmp[*rlen] = c;
    (*rlen)++;
    tmp[*rlen] = '\0';
    free(result);
    return tmp;
}
static int	ft_numlen(int n)
{
	int	len = 0;

	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	num;

	num = n;
	len = ft_numlen(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (num == 0)
		str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	while (num)
	{
		str[len--] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}
char *ft_getenv(const char *name, t_env *env)
{
    int i = 0;
    size_t len;

    if (!name || !env)
        return NULL;
    while (env != NULL)
    {
        if (!ft_strcmp(env->key, name))
            return (env->value);
        env = env->next;
    }
    return NULL;
}

char **splice_tokens(char **tokens, int pos, char **pieces)
{
    int old_n = 0, new_n = 0, out_i = 0;
    char **out;

    while (tokens[old_n])
        old_n++;
    while (pieces[new_n])
        new_n++;

    out = malloc(sizeof(char *) * (old_n - 1 + new_n + 1));
    if (!out)
        return NULL;

    int i = 0;
    while (i < pos)
    {
        out[out_i++] = tokens[i];
        i++;
    }

    int j = 0;
    while (j < new_n)
    {
        out[out_i++] = pieces[j];
        j++;
    }

    i = pos + 1;
    while (i < old_n)
    {
        out[out_i++] = tokens[i];
        i++;
    }

    out[out_i] = NULL;
    free(tokens);
    return out;
}


char **split_all_args(char **args)
{
    int i = 0;
    while (args[i])
    {
        char **pieces = ft_split(args[i], ' ');
        if (!pieces)
        {
            i++;
            continue;
        }
        if (!pieces[1])
        {
            free(args[i]);
            args[i] = pieces[0];
            free(pieces);
            i++;
        }
        else
        {
            free(args[i]);
            args = splice_tokens(args, i, pieces);
            free(pieces);

            int cnt = 0;
            while (args[i + cnt])
                cnt++;
            i += (cnt - 1);
        }
    }
    return args;
}