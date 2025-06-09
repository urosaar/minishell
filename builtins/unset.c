/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:18 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/09 17:15:19 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_unset(t_env **env, char **arg)
{
	t_env	*node;
	t_env	*prev;
	t_env	*tmp;
	int		i;

	i = 1;
	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
		{
			printf("unset: `%s': not a valid identifier\n", arg[i]);
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
				printf("HERE\n");
				break;
			}
			prev = node;
			node = node->next;
		}
		i++;
	}
}
