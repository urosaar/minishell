/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:16:27 by jesse             #+#    #+#             */
/*   Updated: 2025/08/08 00:45:32 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd_exp	expand_command_string(char *cmd_str
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

void	rebuild_with_tokens(t_command *cmd, t_cmd_exp *exp
		, char **exp_args, bool *no_split)
{
	char	**new_args;
	int		new_count;
	int		i[2];

	// if (handle_quoted_command(cmd, exp, no_split))
	// 		return;
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
