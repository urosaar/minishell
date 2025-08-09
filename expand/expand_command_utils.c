/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:16:27 by jesse             #+#    #+#             */
/*   Updated: 2025/08/09 21:27:08 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static int count_unquoted_tokens(const char *s)
{
    int i = 0;
    int count = 0;
    int in_single = 0;
    int in_double = 0;

    while (s[i])
    {
        while (s[i] && s[i] == ' ')
            i++;
        if (!s[i])
            break;
        count++;
        while (s[i] && ((in_single || in_double) || s[i] != ' '))
        {
            if (s[i] == '\'' && !in_double)
                in_single = !in_single;
            else if (s[i] == '"' && !in_single)
                in_double = !in_double;
            i++;
        }
    }
    return (count);
}

static char **split_unquoted_tokens(const char *s)
{
    int i = 0;
    int t = 0;
    int start;
    int tokens;
    char **res;
    int in_single;
    int in_double;

    if (!s)
        return (NULL);
    tokens = count_unquoted_tokens(s);
    res = malloc(sizeof(char *) * (tokens + 1));
    if (!res)
        return (NULL);
    while (s[i] && t < tokens)
    {
        while (s[i] && s[i] == ' ')
            i++;
        start = i;
        in_single = 0;
        in_double = 0;
        while (s[i] && ((in_single || in_double) || s[i] != ' '))
        {
            if (s[i] == '\'' && !in_double)
                in_single = !in_single;
            else if (s[i] == '\"' && !in_single)
                in_double = !in_double;
            i++;
        }
        res[t++] = ft_strndup(s + start, (size_t)(i - start));
    }
    res[t] = NULL;
    return (res);
}

t_cmd_exp expand_command_string(char *cmd_str, int last_status, t_env **env)
{
    t_cmd_exp exp;
    char **raw_tokens;
    int i;

    exp.expanded_str = NULL;
    exp.tokens = NULL;
    exp.token_count = 0;
    if (!cmd_str || !*cmd_str)
        return (exp);

    exp.expanded_str = expand_variables(cmd_str, last_status, env);
    if (!exp.expanded_str)
        return (exp);

    raw_tokens = split_unquoted_tokens(cmd_str);
    if (!raw_tokens)
        return (exp);

    while (raw_tokens[exp.token_count])
        exp.token_count++;
    exp.tokens = malloc(sizeof(char *) * (exp.token_count + 1));
    if (!exp.tokens)
    {
        free_strarray(raw_tokens);
        return (exp);
    }

    i = 0;
    while (raw_tokens[i])
    {
        exp.tokens[i] = expand_variables(raw_tokens[i], last_status, env);
        if (!exp.tokens[i])
            exp.tokens[i] = ft_strdup("");
        i++;
    }
    exp.tokens[i] = NULL;

    free_strarray(raw_tokens);
    return (exp);
}

char **expand_arguments(char **args, int arg_count,
        int last_status, t_env **env)
{
    char    **expanded;
    int     i;
    char    *raw;

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
            expanded[i] = ft_strdup(raw);
            free(raw);
        }
        i++;
    }
    expanded[arg_count] = NULL;
    return (expanded);
}

void	expand_redirections(t_command *cmd, int last_status, t_env **env)
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

void rebuild_with_tokens(t_command *cmd, t_cmd_exp *exp, char **exp_args, bool *no_split)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int exp_args_count = 0;
    char **new_args;
    int new_count;

    if (!exp || exp->token_count <= 0)
        return ;
    while (exp_args && exp_args[exp_args_count])
        exp_args_count++;


    if (exp_args_count)
        new_count = exp->token_count + (exp_args_count - 1);
    else
        new_count = exp->token_count;

    new_args = malloc(sizeof(char *) * (new_count + 1));
    if (!new_args)
        return ;

    while (i < exp->token_count)
        new_args[j++] = ft_strdup(exp->tokens[i++]);

    k = 1;
    while (exp_args && k < exp_args_count)
        new_args[j++] = ft_strdup(exp_args[k++]);
    new_args[j] = NULL;

    free_strarray(cmd->args);
    cmd->cmd = ft_strdup(new_args[0]);
    cmd->args = new_args;

    i = 0;
    while (i < exp->token_count)
        free(exp->tokens[i++]);
    free(exp->tokens);
    exp->tokens = NULL;

    k = 0;
    while (exp_args && exp_args[k])
        free(exp_args[k++]);
    if (exp_args)
        free(exp_args);

    if (cmd->args)
        cmd->args = split_selected_args(cmd->args, no_split);
}

void	rebuild_without_tokens(t_command *cmd, char **exp_args,
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
