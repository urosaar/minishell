/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:10 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/27 19:13:59 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_env *env, char *prev_pwd)
{
	t_env	*node;
	char	*pwd;
	int		i;

	i = 0;
	pwd = NULL;
	node = env;
	pwd = getcwd(NULL,0);
	if (!pwd)
		pwd = ft_strdup(prev_pwd);
	while (pwd[i])
		i++;
	write(1, pwd, i);
	write(1, "\n", 1);
	return (0);
}
// int	ft_pwd(t_env *env, char *prev_pwd)
// {
// 	t_env	*node;
// 	char	*pwd;
// 	int		i;

// 	i = 0;
// 	pwd = NULL;
// 	node = env;
// 	pwd = getcwd(NULL,0);
// 	if (!pwd)
// 	{
// 		while (node)
// 		{
// 			if (node->line && !(ft_strcmp(node->key, "PWD")))
// 			{
// 				pwd = ft_strdup(node->value);
// 				break;
// 			}
// 			node = node->next;
// 		}
// 	}
// 	while (pwd[i])
// 		i++;
// 	write(1, pwd, i);
// 	write(1, "\n", 1);
// 	return (0);
// }
