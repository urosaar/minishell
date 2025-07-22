/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:00:30 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/22 11:00:58 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expand_variables(const char *input, int last_status, t_env **env)
{
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
    char    *expanded_cmd = NULL;
    char    **cmd_tokens = NULL;
    int     cmd_token_count = 0;

    if (cmd->cmd)
    {
        expanded_cmd = expand_variables(cmd->cmd, last_status, env);
        free(cmd->cmd);
        cmd->cmd = NULL;
        if (expanded_cmd)
        {
            cmd_tokens = ft_split(expanded_cmd, ' ');
            if (cmd_tokens)
            {
                while (cmd_tokens[cmd_token_count])
                    cmd_token_count++;
            }
        }
    }

    char    **expanded_args = NULL;
    int     arg_count = 0;
    int     i;

    if (cmd->args)
    {
        while (cmd->args[arg_count])
            arg_count++;
        expanded_args = malloc(sizeof(char *) * (arg_count + 1));
        if (expanded_args)
        {
            i = 0;
            while (i < arg_count)
            {
                expanded_args[i] = expand_variables(cmd->args[i], last_status, env);
                i++;
            }
            expanded_args[arg_count] = NULL;
        }
    }

        if (cmd_token_count > 0)
    {
        int new_count;
        if (arg_count > 0)
            new_count = cmd_token_count + (arg_count - 1);
        else
            new_count = cmd_token_count + 0;

        char **new_args = malloc(sizeof(char *) * (new_count + 1));
        int j = 0;

        i = 0;
        while (i < cmd_token_count)
        {
            new_args[j++] = cmd_tokens[i];
            i++;
        }

        if (arg_count > 0)
        {
            i = 1;
            while (i < arg_count)
            {
                new_args[j++] = expanded_args[i];
                i++;
            }
            if (expanded_args[0])
                free(expanded_args[0]);
        }

        new_args[j] = NULL;
        cmd->cmd = ft_strdup(cmd_tokens[0]);
        free(cmd_tokens);

        if (cmd->args)
        {
            i = 0;
            while (i < arg_count)
            {
                if (cmd->args[i])
                    free(cmd->args[i]);
                i++;
            }
            free(cmd->args);
        }

        cmd->args = new_args;
        if (cmd->args)
            cmd->args = split_all_args(cmd->args);

        free(expanded_args);
    }

    else
    {
        cmd->cmd = expanded_cmd;
        if (cmd->args && expanded_args)
        {
            i = 0;
            while (i < arg_count)
            {
                free(cmd->args[i]);
                cmd->args[i] = expanded_args[i];
                i++;
            }
            free(expanded_args);
        }
        if (cmd->args)
            cmd->args = split_all_args(cmd->args);
    }

    if (cmd_token_count == 0 && expanded_cmd)
        free(expanded_cmd);

    if (cmd->infile)
    {
        char *tmp = expand_variables(cmd->infile, last_status, env);
        free(cmd->infile);
        cmd->infile = tmp;
    }

    if (cmd->outfile)
    {
        char *tmp = expand_variables(cmd->outfile, last_status, env);
        free(cmd->outfile);
        cmd->outfile = tmp;
    }

    if (cmd->heredoc_delimiter)
    {
        char *tmp = expand_variables(cmd->heredoc_delimiter, last_status, env);
        free(cmd->heredoc_delimiter);
        cmd->heredoc_delimiter = tmp;
    }
}
void expand_tokens(char **tokens, int last_status, t_env **env)
{
    int skip_next = 0;
    
    for (int i = 0; tokens[i]; i++)
    {
        if (skip_next)
        {
            skip_next = 0;
            continue;
        }

        if (ft_strcmp(tokens[i], "<<") == 0)
        {
            skip_next = 1;
        }
        else
        {
            char *expanded = expand_variables(tokens[i], last_status, env);
            if (expanded)
            {
                free(tokens[i]);
                tokens[i] = expanded;
            }
        }
    }
}
