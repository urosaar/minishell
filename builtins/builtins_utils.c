/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:05 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/18 11:35:32 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	if_envp_empty(t_env **env)
{
	t_env	*node;
	char	*node1;
	char	*node2;
	char	*node3;
	char	*node4;

	printf("HEREEE\n");
	node1 = "PWD=/mnt/homes/skhallou/Desktop/minishell";
	node2 = "SHLVL=1";
	node3 = "_=/usr/bin/env";
	node4 = "PATH=/mnt/homes/skhallou/goinfre/homebrew/bin:/mnt/homes/skhallou/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands";
	node = ft_newnode(strdup(node1), '=');
	node->next = ft_newnode(strdup(node2), '=');
	node->next->next = ft_newnode(strdup(node3), '=');
	node->next->next->next = ft_newnode(strdup(node4), '=');
	ft_lstadd_back(env, node);
}

void	copy_env(char **envp, t_env **env) 
{
	t_env	*new_node;
	int		i;

	i = 0;
	while (envp[i])
	{
		new_node = ft_newnode(strdup(envp[i]), '=');
		ft_lstadd_back(env, new_node);
		i++;
	}
	if (!*envp)
	{
		if_envp_empty(env);
	}
}

t_env	*copy_for_expo(t_env *env)
{
	t_env	*copy;
	t_env	*tmp;

	copy = NULL;
	while (env)
	{
		tmp = ft_newnode((env->line), '=');
		ft_lstadd_back(&copy, tmp);
		env = env->next;
	}
	return copy;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->line)
		free(env->line);
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	check_for_pwd(char **prev_pwd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	int i = chdir(pwd);
	if (!i)
		*prev_pwd = strdup(pwd);
	if (i == -1)
		return;
}
