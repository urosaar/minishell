/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:14:01 by jesse             #+#    #+#             */
/*   Updated: 2025/08/06 17:00:11 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_operator(const char *input, int *i)
{
	int	start;

	if ((input[*i] == '<' && input[*i + 1] == '<')
		|| (input[*i] == '>' && input[*i + 1] == '>'))
	{
		start = *i;
		*i += 2;
		return (ft_substr(input, start, *i));
	}
	return (ft_substr(input, (*i)++, *i));
}

bool	is_in_quote(bool in_quote, char *quote_char, char current_char)
{
	if (!in_quote && (current_char == '\'' || current_char == '"'))
	{
		*quote_char = current_char;
		return (true);
	}
	else if (in_quote && current_char == *quote_char)
	{
		return (false);
	}
	return (in_quote);
}

void remove_leading_empty_args(t_command *cmd)
{
    int i = 0, j = 0, count = 0;

    // 1) Count how many args we have
    while (cmd->args[count])
        count++;

    // 2) Only strip if >1 arg, and only unquoted empties
    if (count > 1)
    {
        // Skip leading "" *only* if it was unquoted
        while (cmd->args[i]
               && cmd->args[i][0] == '\0'
               && cmd->was_quoted[i] == false)
        {
            i++;
        }

        // Slide the remainder down, mirroring was_quoted[]
        while (cmd->args[i])
        {
            cmd->args[j]       = cmd->args[i];
            cmd->was_quoted[j] = cmd->was_quoted[i];
            j++;
            i++;
        }

        // Terminate both arrays safely
        cmd->args[j]       = NULL;
        cmd->was_quoted[j] = false;
    }

    // 3) Rebuild cmd->cmd field
    free(cmd->cmd);
    if (cmd->args[0])
        cmd->cmd = ft_strdup(cmd->args[0]);
    else
        cmd->cmd = NULL;
}

