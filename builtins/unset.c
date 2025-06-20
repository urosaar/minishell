/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:18 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/20 15:19:49 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_unset(t_env **env, char **arg)
{
	t_env	*node;
	t_env	*prev;
	t_env	*tmp;
	int		i;
	int		flag;

	i = 1;
	flag = 1;
	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
		{
			printf("unset: `%s': not a valid identifier\n", arg[i]);
			flag = 0;
			i++;
			continue;
		}
		node = *env;
		prev = NULL;
		while (node)
		{
			if (node->key && strcmp(node->key, arg[i]) == 0)
			{
				tmp = node->next;
				if (prev)
					prev->next = tmp;
		 		else
					*env = tmp;
				free(node->line);
				free(node->key);
				free(node->value);
				free(node);
				break;
			}
			prev = node;
			node = node->next;
		}
		i++;
	}
	if (!flag)
		return (1);
	return (0);
}
