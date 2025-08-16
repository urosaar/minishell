/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:05 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/16 16:53:35 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_and_print(t_env **env)
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
	int		i;
	int		flag;

	1 && (i = 1, flag = 1);
	if (!arg[1])
		return (sort_and_print(env), 0);
	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arg[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
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
