/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:07:39 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/12 21:07:42 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	build_expanded_tokens(t_cmd_exp *exp, t_locals *v,
		int last_status, t_env **env)
{
	while (v->raw_tokens[exp->token_count])
		exp->token_count++;
	exp->tokens = malloc(sizeof(char *) * (exp->token_count + 1));
	if (!exp->tokens)
		return (0);
	v->i = 0;
	while (v->raw_tokens[v->i])
	{
		exp->tokens[v->i] = expand_variables(v->raw_tokens[v->i],
				last_status, env);
		if (!exp->tokens[v->i])
			exp->tokens[v->i] = ft_strdup("");
		v->i++;
	}
	exp->tokens[v->i] = NULL;
	return (1);
}

t_cmd_exp	expand_command_string(char *cmd_str, int last_status, t_env **env)
{
	t_cmd_exp	exp;
	t_locals	v;

	exp.expanded_str = NULL;
	exp.tokens = NULL;
	exp.token_count = 0;
	if (!cmd_str || !*cmd_str)
		return (exp);
	exp.expanded_str = expand_variables(cmd_str, last_status, env);
	if (!exp.expanded_str)
		return (exp);
	ft_bzero(&v, sizeof(v));
	v.raw_tokens = split_unquoted_tokens(cmd_str);
	if (!v.raw_tokens)
	{
		free(exp.expanded_str);
		exp.expanded_str = NULL;
		return (exp);
	}
	if (!build_expanded_tokens(&exp, &v, last_status, env))
	{
		free_strarray(v.raw_tokens);
		return (exp);
	}
	return (free_strarray(v.raw_tokens), exp);
}

char	**expand_arguments(char **args, int arg_count,
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
