/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:02:33 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/30 19:02:19 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

