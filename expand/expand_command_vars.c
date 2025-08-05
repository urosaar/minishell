/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:16:58 by jesse             #+#    #+#             */
/*   Updated: 2025/08/05 01:44:21 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_quoted_command(t_command *cmd, t_cmd_exp *exp
		, bool *no_split)
{
	char	*clean;

    if (no_split && no_split[0])
    {
		clean = strip_quotes(exp->expanded_str);
        free_array(exp->tokens);
        free(exp->expanded_str);
        free_strarray(cmd->args);

        cmd->cmd = ft_strdup(clean);
        cmd->args = malloc(sizeof(char *) * 2);
        cmd->args[0] = clean;
        cmd->args[1] = NULL;
        return (1);
    }
    return (0);
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
