#include "minishell.h"


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
char *expand_variables(const char *input)
{
    char *result = malloc(1);
    int   in_single = 0, in_double = 0;
    int   i = 0, rlen = 0;

    if (!result)
        return (NULL);
    result[0] = '\0';

    while (input[i])
    {
        if (input[i] == '\'' && !in_double)
        {
            in_single = !in_single;
            char *tmp = malloc(rlen + 2);
            if (!tmp) { free(result); return (NULL); }

            int j = 0;
            while (j < rlen)
            {
                tmp[j] = result[j];
                j++;
            }

            tmp[rlen] = input[i];
            rlen++;
            i++;
            tmp[rlen] = '\0';
            free(result);
            result = tmp;
        }
        else if (input[i] == '"' && !in_single)
        {
            in_double = !in_double;
            char *tmp = malloc(rlen + 2);
            if (!tmp) { free(result); return (NULL); }

            int j = 0;
            while (j < rlen)
            {
                tmp[j] = result[j];
                j++;
            }

            tmp[rlen] = input[i];
            rlen++;
            i++;
            tmp[rlen] = '\0';
            free(result);
            result = tmp;
        }
        else if (input[i] == '$' && !in_single)
        {
            i++;
            int var_start = i;

            while ((input[i] >= 'A' && input[i] <= 'Z')
                || (input[i] >= 'a' && input[i] <= 'z')
                || (input[i] >= '0' && input[i] <= '9')
                || input[i] == '_')
            {
                i++;
            }

            int var_len = i - var_start;
            char *var_name = substr(input, var_start, var_start + var_len);
            char *env = getenv(var_name);

            int elen;
            if (env)
                elen = (int)ft_strlen(env);
            else
                elen = 0;

            free(var_name);

            char *tmp = malloc(rlen + elen + 1);
            if (!tmp) { free(result); return (NULL); }

            int j = 0;
            while (j < rlen)
            {
                tmp[j] = result[j];
                j++;
            }

            if (env)
            {
                int k = 0;
                while (k < elen)
                {
                    tmp[rlen + k] = env[k];
                    k++;
                }
            }

            rlen += elen;
            tmp[rlen] = '\0';
            free(result);
            result = tmp;
        }
        else
        {
            char *tmp = malloc(rlen + 2);
            if (!tmp) { free(result); return (NULL); }

            int j = 0;
            while (j < rlen)
            {
                tmp[j] = result[j];
                j++;
            }

            tmp[rlen] = input[i];
            rlen++;
            i++;
            tmp[rlen] = '\0';
            free(result);
            result = tmp;
        }
    }
    return (result);
}

