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

// char *expand_variables(const char *input, int last_status, t_env **env)
// {
//     char *result = malloc(1);
//     if (!result)
//         return NULL;
//     result[0] = '\0';

//     int in_single = 0, in_double = 0;
//     int i = 0, rlen = 0;
//     while (input[i]) {
//         if (input[i] == '\'' && !in_double)
//         {
//             in_single = !in_single;
//             i++;
//         }
//         else if (input[i] == '"' && !in_single)
//         {
//             in_double = !in_double;
//             i++;
//         }
//         else if (input[i] == '$' && !in_single)
//         {
//             i++; 

//             if (input[i] == '?')
//             {
//                 char *num = ft_itoa(last_status);
//                 if (!num) {
//                     free(result);
//                     return NULL;
//                 }

//                 int addlen = strlen(num);
//                 char *tmp = malloc(rlen + addlen + 1);
//                 if (!tmp)
//                 {
//                     free(result);
//                     free(num);
//                     return NULL;
//                 }

//                 ft_memcpy(tmp, result, rlen);
//                 ft_memcpy(tmp + rlen, num, addlen);
//                 rlen += addlen;
//                 tmp[rlen] = '\0';

//                 free(result);
//                 free(num);
//                 result = tmp;
//                 i++;  // Skip the '?'
//                 continue;
//             }
//             else
//             {
//                 int start = i;
//                 int varlen = 0;
    
//                 result = append_char(result, &rlen, '$');
//                 if (!result)
//                     return NULL;

//                 while (isalnum((unsigned char)input[i]) || input[i] == '_') {
//                     i++;
//                     varlen++;
//                 }

//                 if (varlen > 0) 
//                 {
//                     char *tmp = malloc(rlen + varlen + 1);
//                     if (!tmp)
//                     {
//                         free(result);
//                         return NULL;
//                     }

//                     memcpy(tmp, result, rlen);
//                     memcpy(tmp + rlen, input + start, varlen);
//                     rlen += varlen;
//                     tmp[rlen] = '\0';

//                     free(result);
//                     result = tmp;

//                     if (!in_single) 
//                     { 
//                         char *var = strndup(input + start, varlen);
//                         char *val = ft_getenv(var, *env);
//                         free(var);

//                         if (val)
//                         {
//                             free(result);
//                             result = malloc(rlen - varlen - 1 + strlen(val) + 1);
//                             if (!result)
//                                 return NULL;
                                
//                             memcpy(result, tmp, rlen - varlen - 1);
//                             strcpy(result + rlen - varlen - 1, val);
//                             rlen = rlen - varlen - 1 + strlen(val);
//                         }
//                     }
//                 }
//             }
//         }
//         else 
//             result = append_char(result, &rlen, input[i++]);
//         if (!result)
//             return NULL;
//     }
//     return result;
// }

//************************************************************************************************* */

typedef struct s_state {
    char        *res;
    int         rlen;
    const char  *in;
    int         idx;
    int         last_status;
    t_env       **env;
    int         in_single;
    int         in_double;
}               t_state;

static int grow_buffer(t_state *st, int extra)
{
    char *new_buf = malloc(st->rlen + extra + 1);
    if (!new_buf) {
        free(st->res);
        return 0;
    }
    if (st->res) {
        memcpy(new_buf, st->res, st->rlen);
        free(st->res);
    }
    st->res = new_buf;
    st->res[st->rlen + extra] = '\0';
    return 1;
}

static int str_append_char(t_state *st, char c)
{
    if (!grow_buffer(st, 1)) return 0;
    st->res[st->rlen++] = c;
    return 1;
}

static int insert_string(t_state *st, const char *s, int len)
 {
    if (!len) return 1;
    if (!grow_buffer(st, len)) return 0;
    memcpy(st->res + st->rlen, s, len);
    st->rlen += len;
    return 1;
}

static int handle_exit_status(t_state *st)
{
    char *num = ft_itoa(st->last_status);
    if (!num) return 0;
    int len = strlen(num);
    int success = insert_string(st, num, len);
    free(num);
    st->idx++;
    return success;
}

static int handle_env_var(t_state *st)
{
    int start = st->idx;
    int varlen = 0;
    
    while (isalnum((unsigned char)st->in[st->idx]) || 
           st->in[st->idx] == '_'){
        st->idx++;
        varlen++;
    }

    if (varlen == 0) {
        return str_append_char(st, '$');
    }

    char *var_name = strndup(st->in + start, varlen);
    if (!var_name) return 0;
    
    char *val = ft_getenv(var_name, *st->env);
    free(var_name);
    
    if (val) {
        return insert_string(st, val, strlen(val));
    } else {
        if (!str_append_char(st, '$')) return 0;
        return insert_string(st, st->in + start, varlen);
    }
}

static int handle_var_exp(t_state *st)
{
    st->idx++;
    if (st->in[st->idx] == '?') {
        return handle_exit_status(st);
    } else {
        return handle_env_var(st);
    }
}

char *expand_variables(const char *input, int last_status, t_env **env)
{
    t_state st = {0};
    st.in = input;
    st.last_status = last_status;
    st.env = env;
    st.res = malloc(1);
    if (!st.res) return NULL;
    st.res[0] = '\0';

    while (st.in[st.idx])
    {
        char c = st.in[st.idx];
        if (c == '\'' && !st.in_double)
        {
            st.in_single = !st.in_single;
            st.idx++;
        } else if (c == '"' && !st.in_single)
        {
            st.in_double = !st.in_double;
            st.idx++;
        } else if (c == '$' && !st.in_single)
        {
            if (!handle_var_exp(&st))
            {
                free(st.res);
                return NULL;
            }
        } else {
            if (!str_append_char(&st, c))
            {
                free(st.res);
                return NULL;
            }
            st.idx++;
        }
    }
    return st.res;
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
            // while (i < arg_count)
            // {
            //     expanded_args[i] = expand_variables(cmd->args[i], last_status, env);
            //     i++;
            // }
              while (i < arg_count)
            {
                char *raw = cmd->args[i];
                char *tmp = expand_variables(raw, last_status, env);
                // free(raw);
                expanded_args[i] = strip_quotes(tmp);
                free(tmp);
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
