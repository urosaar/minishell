/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:22 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/15 16:04:20 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	replace(t_env *env, char *path, char *oldpath)
{
	t_env	*node;

	node = env;
	while (node)
	{
		if (node->line && !(ft_strcmp(node->key, "PWD")))
		{
			free(node->line);
			if (node->value)
				free(node->value);
			node->value = ft_strdup(path);
			node->line = ft_join(ft_strdup("PWD="), node->value);
		}
		else if (node->line && !(ft_strcmp(node->key, "OLDPWD")))
		{
			free(node->line);
			if (node->value)
				free(node->value);
			node->value = ft_strdup(oldpath);
			node->line = ft_join(ft_strdup("OLDPWD="), node->value);
		}
		node = node->next;
	}
}

int	change_to_home(t_env *env, char *path, int i)
{
	t_env	*node;

	node = env;
	while (node)
	{
		if (!ft_strcmp(node->key, "HOME"))
		{
			path = ft_strdup(node->value);
			break ;
		}
		node = node->next;
	}
	if (!path)
	{
		printf("minishell: cd: HOME not set\n");
		return (-2);
	}
	i = chdir(path);
	free(path);
	return (i);
}

void	check_prev_pwd(t_env **env, char *path, char **oldpath)
{
	t_env	*node;

	node = *env;
	if (!*oldpath)
	{
		if (!path && !*oldpath)
		{
			ft_putstr_fd("cd: error retrieving current directory: getcwd:", 2);
			ft_putstr_fd(" cannot access parent directories:", 2);
			ft_putstr_fd(" No such file or directory\n", 2);
		}
		while (node)
		{
			if (node->line && !(ft_strcmp(node->key, "PWD")))
			{
				*oldpath = ft_strdup(node->value);
				break ;
			}
			node = node->next;
		}
	}
}

void	cd_error(char *arg)
{
	if (errno == ENOTDIR)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": Not a directory\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

int	ft_cd(t_env **env, char **arg, char *prev_pwd)
{
	char	*oldpath;
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	if (!arg[1] || !ft_strcmp(arg[1], "~"))
		i = change_to_home(*env, path, i);
	else
		i = chdir(arg[1]);
	if (i == -1 || i == -2)
	{
		if (arg[1] && i == -1)
			cd_error(arg[1]);
		return (1);
	}
	oldpath = getcwd(prev_pwd, 0);
	path = getcwd(NULL, 0);
	check_prev_pwd(env, path, &oldpath);
	if (!path)
		path = ft_join(prev_pwd, "/..");
	replace(*env, path, oldpath);
	free(path);
	free(oldpath);
	return (0);
}
