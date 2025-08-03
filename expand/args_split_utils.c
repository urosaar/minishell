/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_split_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:15:17 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 14:59:38 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	count(char **args)
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

void	free_strarray(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

bool	*create_no_split_map(char **args)
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
