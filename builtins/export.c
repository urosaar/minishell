/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:05 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/01 18:49:23 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void sort_and_print(t_env **env)
{	
	t_env	*copy;
	t_env	*tmp;

	copy = copy_for_expo(*env);
	swap_line(&copy);
	tmp = copy;
	while (tmp)
	{
		printf("declare -x %s", tmp->key);
		if (tmp->value)
				printf("=\"%s\"", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
	free_env(copy);
}

int	ft_export(t_env **env, char **arg)
{
	int		valid;
	int		i;
	int		flag;

	i = 1;
	flag = 1;
	if (!arg[1])
		return (sort_and_print(env), 0);
	while (arg[i])
	{
		valid = is_valid_identifier(arg[i]);
		if (!valid)
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg[i]);
			flag = 0;
		}
		else if (is_valid_identifier(arg[i]) == 1)
			without_plus(env, arg[i]);
		else
			with_plus(env, arg[i]);
		i++;
	}
	if (!flag)
		return (1);
	return (0);
}
