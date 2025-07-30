/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:38 by skhallou          #+#    #+#             */
/*   Updated: 2025/07/29 21:14:37 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_env **env, char **arg)
{
	t_env	*tmp;

	if (arg[1])
	{
		printf("env: %s: No such file or directory\n", arg[1]);
		return (127);
	}
	tmp = *env;
	while (tmp)
	{
		if (ft_strchr(tmp->line, '='))
			printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
	return (0);
}
