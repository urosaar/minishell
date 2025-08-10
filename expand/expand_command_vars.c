/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:16:58 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 05:19:59 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char **build_new_args(t_cmd_exp *exp, char **exp_args, t_locals *v)
{
    while (exp_args && exp_args[v->count])
        v->count++;
    if (v->count)
        v->tokens = exp->token_count + (v->count - 1);
    else
        v->tokens = exp->token_count;
    v->res = malloc(sizeof(char *) * (v->tokens + 1));
    if (!v->res)
        return (NULL);
    while (v->i < exp->token_count)
        v->res[v->t++] = ft_strdup(exp->tokens[v->i++]);
    v->start = 1;
    while (exp_args && v->start < v->count)
        v->res[v->t++] = ft_strdup(exp_args[v->start++]);
    v->res[v->t] = NULL;
    return (v->res);
}

void rebuild_with_tokens(t_command *cmd, t_cmd_exp *exp, char **exp_args, bool *no_split)
{
    t_locals v;

    ft_bzero(&v, sizeof(v));
    if (!exp || exp->token_count <= 0)
        return;
    if (!build_new_args(exp, exp_args, &v))
        return;
    free_strarray(cmd->args);
    cmd->cmd = ft_strdup(v.res[0]);
    cmd->args = v.res;
    v.i = 0;
    while (v.i < exp->token_count)
        free(exp->tokens[v.i++]);
    free(exp->tokens);
    exp->tokens = NULL;
    v.start = 0;
    while (exp_args && exp_args[v.start])
        free(exp_args[v.start++]);
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

static void finalize_expansion(t_command *cmd, t_cmd_exp *exp,
                               char **exp_args, bool *no_split)
{
	int	i;

    if (exp->token_count > 0)
        rebuild_with_tokens(cmd, exp, exp_args, no_split);
    else
        rebuild_without_tokens(cmd, exp_args, no_split, exp->expanded_str);

    if (exp->tokens)
    {
        i = 0;
        while (exp->tokens[i])
            free(exp->tokens[i++]);
        free(exp->tokens);
        exp->tokens = NULL;
    }
    if (exp->expanded_str)
    {
        free(exp->expanded_str);
        exp->expanded_str = NULL;
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
