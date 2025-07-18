/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:22 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/27 19:11:34 by skhallou         ###   ########.fr       */
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
	t_env *node;

	node = env;
	while (node)
	{
		if (!ft_strcmp(node->key, "HOME"))
		{
			path = ft_strdup(node->value);
			break;
		}
		node = node->next;
	}
	if (!path)
	{
		printf("bash: cd: HOME not set\n");
		return (-2);
	}
	i = chdir(path);
	free(path);
	return (i);
}

int	ft_cd(t_env **env, char **arg, char *prev_pwd)
{
	char	*oldpath;
	char	*path;
	int		i;

	path = NULL;
	oldpath = getcwd(NULL, 0);
	if (!oldpath)
		oldpath = ft_strdup(prev_pwd);
	if (!arg[1] || !ft_strcmp(arg[1], "~"))
		i = change_to_home(*env, path, i);
	else
		i = chdir(arg[1]);
	printf("I = %d\n", i);
	if (i == -1 || i == -2)
	{
		if (arg[1] && i == -1)
			printf("minishell: cd: %s: No such file or directory\n", arg[1]);
		return (1);
	}
	path = getcwd(NULL, 0);
	printf("PATH = %s\n", path);
	if (!path)
		return (1);
	replace(*env, path, oldpath);
	free(oldpath);
	return (0);
}

// void	replace(t_env *env, char *path, char *oldpath, char *prev_pwd)
// {
// 	t_env	*node;

// 	node = env;
// 	while (node)
// 	{
// 		if (node->line && !(ft_strcmp(node->key, "PWD")))
// 		{
// 			free(node->line);
// 			if (node->value)
// 				free(node->value);
// 			node->value = ft_strdup(path);
// 			node->line = ft_join(ft_strdup("PWD="), node->value);
// 		}
// 		else if (node->line && !(ft_strcmp(node->key, "OLDPWD")))
// 		{
// 			free(node->line);
// 			if (node->value)
// 				free(node->value);
// 			node->value = ft_strdup(oldpath);
// 			node->line = ft_join(ft_strdup("OLDPWD="), node->value);
// 		}
// 		node = node->next;
// 	}
// }

// int	change_to_home(t_env *env, char *path, int i)
// {
// 	t_env *node;

// 	node = env;
// 	while (node)
// 	{
// 		if (!ft_strcmp(node->key, "HOME"))
// 		{
// 			path = ft_strdup(node->value);
// 			break;
// 		}
// 		node = node->next;
// 	}
// 	if (!path)
// 	{
// 		printf("bash: cd: HOME not set\n");
// 		return (-2);
// 	}
// 	i = chdir(path);
// 	free(path);
// 	return (i);
// }

// int	ft_cd(t_env **env, char **arg, char *prev_pwd)
// {
// 	char	*oldpath;
// 	char	*test;
// 	char	*path;
// 	int		i;
// 	int		flag = 1;

// 	path = NULL;
// 	if (!arg[1] || !ft_strcmp(arg[1], "~"))
// 		i = change_to_home(*env, path, i);
// 	else
// 		i = chdir(arg[1]);
// 	printf("I = %d\n", i);
// 	if (i == -1 || i == -2)
// 	{
// 		if (arg[1] && i == -1)
// 			printf("minishell: cd: %s: No such file or directory\n", arg[1]);
// 		return (1);
// 	}
// 	test = getcwd(prev_pwd, 0);
// 	oldpath = getcwd(NULL, 0);
// 	path = getcwd(NULL, 0);
// 	if (!test)
// 	{
// 		if (!path && !test)
// 		{
// 			printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
// 			// oldpath = ft_strdup(prev_pwd);
// 		}
// 		// else if (path)
// 		// 	oldpath = ft_join(prev_pwd, "/..");
// 		oldpath = ft_strdup(prev_pwd);
// 		flag = 0;
// 		// else
// 		printf(">>>>>>>OLDPATH = %s\n", oldpath);
// 	}
// 	printf("PATH = %s\n", path);
// 	if (!path)
// 	{
// 		path = ft_join(prev_pwd, "/..");
		
// 	}
// 		// return (1);
// 	replace(*env, path, oldpath, prev_pwd);
// 	free(path);
// 	free(oldpath);
// 	return (0);
// }