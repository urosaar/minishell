/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:10 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/03 19:27:53 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*if_pwd_null(t_env *env)
{
	t_env	*node;
	char	*pwd;
	
	pwd = NULL;
	node = env;
	while (node)
	{
		if (node->key && !(ft_strcmp(node->key, "PWD")))
		{
			pwd = ft_strdup(node->value);
			break;
		}
		node = node->next;
	}
	return (pwd);
}

int	ft_pwd(t_env *env)
{
	t_env	*node;
	char	*pwd;
	int		i;

	i = 0;
	pwd = NULL;
	node = env;
	pwd = getcwd(NULL,0);
	if (!pwd)
		pwd = if_pwd_null(node);
	if (!pwd)
	{
		printf("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
		return (1);	
	}
	while (pwd && pwd[i])
		i++;
	write(1, pwd, i);
	write(1, "\n", 1);
	free(pwd);  
	return (0);
}
