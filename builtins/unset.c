/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:18 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/06 17:55:02 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset_form_env(t_env **env, char *arg)
{
	t_env	*node;
	t_env	*prev;
	t_env	*tmp;

	node = *env;
	prev = NULL;
	while (node)
	{
		if (node->key && ft_strcmp(node->key, arg) == 0)
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
			break ;
		}
		prev = node;
		node = node->next;
	}
}

int	ft_unset(t_env **env, char **arg)
{
	int		i;
	int		flag;

	i = 1;
	flag = 1;
	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n", arg[i]);
			flag = 0;
			i++;
			continue ;
		}
		unset_form_env(env, arg[i]);
		i++;
	}
	if (!flag)
		return (1);
	return (0);
}
