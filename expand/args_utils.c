/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:07:24 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/16 06:25:08 by oukhanfa         ###   ########.fr       */
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

static bool	check_no_split_condition(char *arg)
{
	int		len;
	char	*stripped;
	bool	result;

	len = ft_strlen(arg);
	result = (len >= 2 && ((arg[0] == '"' && arg[len - 1] == '"')
				|| (arg[0] == '\'' && arg[len - 1] == '\'')));
	if (!result && (ft_isalpha((unsigned char)arg[0]) || arg[0] == '_'))
		result = is_assignment(arg);
	if (result)
	{
		stripped = strip_empty_quotes(arg);
		if (stripped)
		{
			if (is_simple_dollar_var_str(stripped) || stripped[0] == '\0')
				result = false;
			free(stripped);
		}
	}
	return (result);
}

bool	*create_no_split_map(char **args)
{
	bool	*no_split;
	int		idx;
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
		no_split[idx] = check_no_split_condition(args[idx]);
		idx++;
	}
	return (no_split);
}
