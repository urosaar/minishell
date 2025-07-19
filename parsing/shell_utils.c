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
        if (!ft_strcmp(env->key, name))
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
            i++;  // Skip the quote character
        }
        else if (input[i] == '"' && !in_single)
        {
            in_double = !in_double;
            i++;  // Skip the quote character
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
                int varlen = 0;
    
                result = append_char(result, &rlen, '$');
                if (!result)
                    return NULL;

                while (isalnum((unsigned char)input[i]) || input[i] == '_') {
                    i++;
                    varlen++;
                }

                if (varlen > 0) 
                {
                    char *tmp = malloc(rlen + varlen + 1);
                    if (!tmp)
                    {
                        free(result);
                        return NULL;
                    }

                    memcpy(tmp, result, rlen);
                    memcpy(tmp + rlen, input + start, varlen);
                    rlen += varlen;
                    tmp[rlen] = '\0';

                    free(result);
                    result = tmp;

                    if (!in_single) 
                    { 
                        char *var = strndup(input + start, varlen);
                        char *val = ft_getenv(var, *env);
                        free(var);

                        if (val)
                        {
                            free(result);
                            result = malloc(rlen - varlen - 1 + strlen(val) + 1);
                            if (!result)
                                return NULL;
                                
                            memcpy(result, tmp, rlen - varlen - 1);
                            strcpy(result + rlen - varlen - 1, val);
                            rlen = rlen - varlen - 1 + strlen(val);
                        }
                    }
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

void expand_command_vars(t_command *cmd, int last_status, t_env **env)
{
    char *expanded_cmd = NULL;
    char **cmd_tokens = NULL;
    int cmd_token_count = 0;

    if (cmd->cmd)
    {
        expanded_cmd = expand_variables(cmd->cmd, last_status, env);
        free(cmd->cmd);
        cmd->cmd = NULL;
        if (expanded_cmd)
        {
            cmd_tokens = ft_split(expanded_cmd, ' ');
            if (cmd_tokens) {
                while (cmd_tokens[cmd_token_count]) 
                    cmd_token_count++;
            }
        }
    }

    char **expanded_args = NULL;
    int arg_count = 0;
    if (cmd->args)
    {
        while (cmd->args[arg_count]) arg_count++;
        expanded_args = malloc(sizeof(char*) * (arg_count + 1));
        if (expanded_args) {
            for (int i = 0; i < arg_count; i++) {
                expanded_args[i] = expand_variables(cmd->args[i], last_status, env);
            }
            expanded_args[arg_count] = NULL;
        }
    }
    if (cmd_token_count > 0)
    {
        int new_count = cmd_token_count + (arg_count > 0 ? arg_count - 1 : 0);
        char **new_args = malloc(sizeof(char*) * (new_count + 1));
        int j = 0;

        for (int i = 0; i < cmd_token_count; i++) {
            new_args[j++] = cmd_tokens[i]; 
        }
        if (arg_count > 0)
        {
            for (int i = 1; i < arg_count; i++)
            {
                new_args[j++] = expanded_args[i];
            }
            if (expanded_args[0]) free(expanded_args[0]);
        }
        new_args[j] = NULL;
        cmd->cmd = ft_strdup(cmd_tokens[0]);
        free(cmd_tokens);
        if (cmd->args)
        {
            for (int i = 0; i < arg_count; i++) 
                if (cmd->args[i]) free(cmd->args[i]);
            free(cmd->args);
        }
        cmd->args = new_args;

        free(expanded_args);
    }
    else 
    {
        cmd->cmd = expanded_cmd;
        if (cmd->args && expanded_args) {
            for (int i = 0; i < arg_count; i++) {
                free(cmd->args[i]);
                cmd->args[i] = expanded_args[i];
            }
            free(expanded_args);
        }
    }
    if (cmd_token_count == 0 && expanded_cmd) {
        free(expanded_cmd);
    }
    if (cmd->infile) {
        char *tmp = expand_variables(cmd->infile, last_status, env);
        free(cmd->infile);
        cmd->infile = tmp;
    }

    if (cmd->outfile) {
        char *tmp = expand_variables(cmd->outfile, last_status, env);
        free(cmd->outfile);
        cmd->outfile = tmp;
    }

    if (cmd->heredoc_delimiter) {
        char *tmp = expand_variables(cmd->heredoc_delimiter, last_status, env);
        free(cmd->heredoc_delimiter);
        cmd->heredoc_delimiter = tmp;
    }
}
