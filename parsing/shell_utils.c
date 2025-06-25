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
static char *append_char(char *result, int *rlen, char c)
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
        if (!strcmp(env->key, name))
            return (env->value);
        env = env->next;
    }
    return NULL;
}

char *expand_variables(const char *input, int last_status, t_env **env) {
    char *result = malloc(1);
    if (!result)
        return NULL;
    result[0] = '\0';

    int in_single = 0, in_double = 0;
    int i = 0, rlen = 0;
    while (input[i]) {
        if (input[i] == '\'' && !in_double)
        {
            in_single = !in_single;
            result = append_char(result, &rlen, input[i++]);
        }
        else if (input[i] == '"' && !in_single)
        {
            in_double = !in_double;
            result = append_char(result, &rlen, input[i++]);
        }
        else if (input[i] == '$' && !in_single)
        {
            i++; 

            if (input[i] == '?')
            {
                char *num = ft_itoa(last_status);
                if (!num) {
                    free(result);
                    return NULL;
                }

                int addlen = strlen(num);
                char *tmp = malloc(rlen + addlen + 1);
                if (!tmp)
                {
                    free(result);
                    free(num);
                    return NULL;
                }

                ft_memcpy(tmp, result, rlen);
                ft_memcpy(tmp + rlen, num, addlen);
                rlen += addlen;
                tmp[rlen] = '\0';

                free(result);
                free(num);
                result = tmp;
                i++;  // Skip the '?'
                continue;
            }
            else
            {
                int start = i;
   
                while (isalnum((unsigned char)input[i]) || input[i] == '_')
                    i++;
                int varlen = i - start;

                if (varlen > 0) 
                {
                    char *var = strndup(input + start, varlen);
                    char *val = ft_getenv(var, *env);
                    free(var);

                    if (val) {
                        int vlen = strlen(val);
                        char *tmp = malloc(rlen + vlen + 1);
                        if (!tmp)
                        {
                            free(result);
                            return NULL;
                        }

                        memcpy(tmp, result, rlen);
                        memcpy(tmp + rlen, val, vlen);
                        rlen += vlen;
                        tmp[rlen] = '\0';

                        free(result);
                        result = tmp;
                    }
                }
                else {

                    result = append_char(result, &rlen, '$');
                    if (!result)
                        return NULL;
                }
            }
        }
        else 
            result = append_char(result, &rlen, input[i++]);
        if (!result)
            return NULL;
    }
    return result;
}