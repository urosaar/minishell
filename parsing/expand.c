/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:00:30 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/07/31 21:01:06 by jesse            ###   ########.fr       */
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

static int	str_append_char(t_state *st, char c)
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

static int	handle_exit_status(t_state *st)
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

static int	handle_env_var(t_state *st)
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

static int	handle_var_exp(t_state *st)
{
	st->idx++;
	if (st->in[st->idx] == '?')
		return (handle_exit_status(st));
	else
		return (handle_env_var(st));
}

static int	process_character(t_state *st)
{
	char	c;

	c = st->in[st->idx];
	if (c == '\'' && !st->in_double)
	{
		st->in_single = !st->in_single;
		st->idx++;
		return (1);
	}
	else if (c == '"' && !st->in_single)
	{
		st->in_double = !st->in_double;
		st->idx++;
		return (1);
	}
	else if (c == '$' && !st->in_single)
		return (handle_var_exp(st));
	if (!str_append_char(st, c))
		return (0);
	st->idx++;
	return (1);
}

char	*expand_variables(const char *input, int last_status, t_env **env)
{
	t_state	st;

	ft_bzero(&st, sizeof(t_state));
	st.in = input;
	st.last_status = last_status;
	st.env = env;
	st.res = malloc(1);
	if (!st.res)
		return (NULL);
	st.res[0] = '\0';
	while (st.in[st.idx])
	{
		if (!process_character(&st))
		{
			free(st.res);
			return (NULL);
		}
	}
	return (st.res);
}

char	**split_selected_args(char **args, bool *no_split)
{
	int		i;
	int		k;
	char	**pieces;

	i = 0;
	while (args[i])
	{
		if (!no_split[i] && ft_strchr(args[i], ' '))
		{
			pieces = ft_split(args[i], ' ');
			free(args[i]);
			args = splice_tokens(args, i, pieces);
			free(pieces);
			k = 0;
			while (args[i + k])
				k++;
			i += k;
		}
		else
			i++;
	}
	return (args);
}

static int	count(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

static bool	is_assignment(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			break ;
		i++;
	}
	return (false);
}

static void	free_strarray(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static bool	*create_no_split_map(char **args)
{
	bool	*no_split;
	int		idx;
	int		len;
	int		orig_argc;

	orig_argc = 0;
	while (args[orig_argc])
		orig_argc++;
	no_split = ft_calloc(orig_argc + 1, sizeof(bool));
	if (!no_split)
		return (NULL);
	idx = 0;
	while (idx < orig_argc)
	{
		len = ft_strlen(args[idx]);
		no_split[idx] = (len >= 2 && ((args[idx][0] == '"'
						&& args[idx][len - 1] == '"') || (args[idx][0] == '\''
						&& args[idx][len - 1] == '\'')));
		if (!no_split[idx] && (ft_isalpha(args[idx][0]) || args[idx][0] == '_'))
			no_split[idx] = is_assignment(args[idx]);
		idx++;
	}
	return (no_split);
}

static t_cmd_exp	expand_command_string(char *cmd_str
		, int last_status, t_env **env)
{
	t_cmd_exp	exp;

	exp.expanded_str = NULL;
	exp.tokens = NULL;
	exp.token_count = 0;
	if (!cmd_str || !*cmd_str)
		return (exp);
	exp.expanded_str = expand_variables(cmd_str, last_status, env);
	if (!exp.expanded_str)
		return (exp);
	exp.tokens = ft_split(exp.expanded_str, ' ');
	if (exp.tokens)
		while (exp.tokens[exp.token_count])
			exp.token_count++;
	return (exp);
}

static char	**expand_arguments(char **args, int arg_count,
		int last_status, t_env **env)
{
	char	**expanded;
	int		i;
	char	*raw;

	if (!args || arg_count == 0)
		return (NULL);
	expanded = malloc(sizeof(char *) * (arg_count + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < arg_count)
	{
		raw = expand_variables(args[i], last_status, env);
		if (!raw)
			expanded[i] = NULL;
		else
		{
			expanded[i] = strip_quotes(raw);
			free(raw);
		}
		i++;
	}
	expanded[arg_count] = NULL;
	return (expanded);
}

static void	expand_redirections(t_command *cmd, int last_status, t_env **env)
{
	char	*tmp;

	if (cmd->infile)
	{
		tmp = expand_variables(cmd->infile, last_status, env);
		free(cmd->infile);
		cmd->infile = tmp;
	}
	if (cmd->outfile)
	{
		tmp = expand_variables(cmd->outfile, last_status, env);
		free(cmd->outfile);
		cmd->outfile = tmp;
	}
}

static void	rebuild_with_tokens(t_command *cmd, t_cmd_exp *exp
		, char **exp_args, bool *no_split)
{
	char	**new_args;
	int		new_count;
	int		i[2];

	new_count = exp->token_count + (count(exp_args) - 1);
	new_args = malloc(sizeof(char *) * (new_count + 1));
	ft_memset(i, 0, sizeof(i));
	while (i[0] < exp->token_count)
		new_args[i[1]++] = exp->tokens[i[0]++];
	i[0] = 1;
	while (exp_args && exp_args[i[0]])
		new_args[i[1]++] = exp_args[i[0]++];
	new_args[i[1]] = NULL;
	free(exp->tokens);
	free(exp->expanded_str);
	cmd->cmd = ft_strdup(new_args[0]);
	free_strarray(cmd->args);
	cmd->args = new_args;
	if (exp_args && exp_args[0])
		free(exp_args[0]);
	free(exp_args);
	if (cmd->args)
		cmd->args = split_selected_args(cmd->args, no_split);
}

static void	rebuild_without_tokens(t_command *cmd, char **exp_args,
	bool *no_split, char *exp_cmd)
{
	int	i;
	int	arg_count;

	cmd->cmd = exp_cmd;
	if (!cmd->args || !exp_args)
		return ;
	arg_count = count(cmd->args);
	i = 0;
	while (i < arg_count)
	{
		free(cmd->args[i]);
		cmd->args[i] = exp_args[i];
		i++;
	}
	free(exp_args);
	if (cmd->args)
		cmd->args = split_selected_args(cmd->args, no_split);
}

static void	finalize_expansion(t_command *cmd, t_cmd_exp *exp,
				char **exp_args, bool *no_split)
{
	if (exp->token_count > 0)
	{
		rebuild_with_tokens(cmd, exp, exp_args, no_split);
		exp->tokens = NULL;
	}
	else
	{
		rebuild_without_tokens(cmd, exp_args, no_split,
			exp->expanded_str);
		exp->expanded_str = NULL;
	}
	if (exp->tokens)
	{
		free(exp->tokens);
		exp->tokens = NULL;
	}
	free(no_split);
}

void	expand_command_vars(t_command *cmd, int last_status, t_env **env)
{
	t_cmd_exp	exp;
	bool		*no_split;
	char		**exp_args;
	int			arg_count;

	if (cmd->args)
		no_split = create_no_split_map(cmd->args);
	else
		no_split = NULL;
	if (cmd->args)
		arg_count = count(cmd->args);
	else
		arg_count = 0;
	exp = expand_command_string(cmd->cmd, last_status, env);
	free(cmd->cmd);
	cmd->cmd = NULL;
	exp_args = expand_arguments(cmd->args, arg_count,
			last_status, env);
	finalize_expansion(cmd, &exp, exp_args, no_split);
	expand_redirections(cmd, last_status, env);
}

static void	expand_token_if_needed(char **token, int last_status, t_env **env)
{
	char	*expanded;

	expanded = expand_variables(*token, last_status, env);
	if (expanded)
	{
		free(*token);
		*token = expanded;
	}
}

void	expand_tokens(char **tokens, int last_status, t_env **env)
{
	int	i;
	int	skip_next;

	i = 0;
	skip_next = 0;
	while (tokens && tokens[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (ft_strcmp(tokens[i], "<<") == 0)
			skip_next = 1;
		else
			expand_token_if_needed(&tokens[i], last_status, env);
		i++;
	}
}

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
//                             result = malloc(rlen - varlen - 1 
//+ strlen(val) + 1);
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
//*************************************/
// void expand_command_vars(t_command *cmd, int last_status, t_env **env)
// {
//     char    *expanded_cmd = NULL;
//     char    **cmd_tokens = NULL;
//     int     cmd_token_count = 0;
//     if (cmd->cmd)
//     {
//         expanded_cmd = expand_variables(cmd->cmd, last_status, env);
//         free(cmd->cmd);
//         cmd->cmd = NULL;
//         if (expanded_cmd)
//         {
//             cmd_tokens = ft_split(expanded_cmd, ' ');
//             if (cmd_tokens)
//             {
//                 while (cmd_tokens[cmd_token_count])
//                     cmd_token_count++;
//             }
//         }
//     }
//     char    **expanded_args = NULL;
//     int     arg_count = 0;
//     int     i;
//     if (cmd->args)
//     {
//         while (cmd->args[arg_count])
//             arg_count++;
//         expanded_args = malloc(sizeof(char *) * (arg_count + 1));
//         if (expanded_args)
//         {
//             i = 0;
//             // while (i < arg_count)
//             // {
//             //     expanded_args[i] = expand_variables(cmd->args[i]
//, last_status, env);
//             //     i++;
//             // }
//               while (i < arg_count)
//             {
//                 char *raw = cmd->args[i];
//                 char *tmp = expand_variables(raw, last_status, env);
//                 // free(raw);
//                 expanded_args[i] = strip_quotes(tmp);
//                 free(tmp);
//                 i++;
//             }
//             expanded_args[arg_count] = NULL;
//         }
//     }
//         if (cmd_token_count > 0)
//     {
//         int new_count;
//         if (arg_count > 0)
//             new_count = cmd_token_count + (arg_count - 1);
//         else
//             new_count = cmd_token_count + 0;
//         char **new_args = malloc(sizeof(char *) * (new_count + 1));
//         int j = 0;
//         i = 0;
//         while (i < cmd_token_count)
//         {
//             new_args[j++] = cmd_tokens[i];
//             i++;
//         }
//         if (arg_count > 0)
//         {
//             i = 1;
//             while (i < arg_count)
//             {
//                 new_args[j++] = expanded_args[i];
//                 i++;
//             }
//             if (expanded_args[0])
//                 free(expanded_args[0]);
//         }
//         new_args[j] = NULL;
//         cmd->cmd = ft_strdup(cmd_tokens[0]);
//         free(cmd_tokens);
//         if (cmd->args)
//         {
//             i = 0;
//             while (i < arg_count)
//             {
//                 if (cmd->args[i])
//                     free(cmd->args[i]);
//                 i++;
//             }
//             free(cmd->args);
//         }
//         cmd->args = new_args;
//         if (cmd->args)
//             cmd->args = split_all_args(cmd->args);
//         free(expanded_args);
//     }
//     else
//     {
//         cmd->cmd = expanded_cmd;
//         if (cmd->args && expanded_args)
//         {
//             i = 0;
//             while (i < arg_count)
//             {
//                 free(cmd->args[i]);
//                 cmd->args[i] = expanded_args[i];
//                 i++;
//             }
//             free(expanded_args);
//         }
//         if (cmd->args)
//             cmd->args = split_all_args(cmd->args);
//     }
//     if (cmd_token_count == 0 && expanded_cmd)
//         free(expanded_cmd);
//     if (cmd->infile)
//     {
//         char *tmp = expand_variables(cmd->infile, last_status, env);
//         free(cmd->infile);
//         cmd->infile = tmp;
//     }
//     if (cmd->outfile)
//     {
//         char *tmp = expand_variables(cmd->outfile, last_status, env);
//         free(cmd->outfile);
//         cmd->outfile = tmp;
//     }
//     if (cmd->heredoc_delimiter)
//     {
//         char *tmp = expand_variables(cmd->heredoc_delimiter
//, last_status, env);
//         free(cmd->heredoc_delimiter);
//         cmd->heredoc_delimiter = tmp;
//     }
// }
/****************************************/