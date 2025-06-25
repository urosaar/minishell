/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:05 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/25 17:53:00 by skhallou         ###   ########.fr       */
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
	node4 = "PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.";
	node = ft_newnode((node1), '=');
	node->next = ft_newnode((node2), '=');
	node->next->next = ft_newnode((node3), '=');
	node->next->next->next = ft_newnode((node4), '=');
	ft_lstadd_back(env, node);
}

void	copy_env(char **envp, t_env **env) 
{
	t_env	*new_node;
	int		i;

	i = 0;
	while (envp[i])
	{
		new_node = ft_newnode(ft_strdup(envp[i]), '=');
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
		*prev_pwd = ft_strdup(pwd);
	if (i == -1)
		return;
}
