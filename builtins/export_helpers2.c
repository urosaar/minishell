/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 18:45:19 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/10 19:15:09 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*find_in_env(t_env *env, char *key)
{
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	update_found_node(t_env *found, char *arg)
{
	char	*value;

	value = ft_strchr(arg, '=');
	free(found->line);
	found->line = ft_strdup(arg);
	if (found->value)
		free(found->value);
	found->value = ft_strdup(value + 1);
}

void	without_plus(t_env **env, char *arg)
{
	t_env	*new;
	t_env	*found;
	char	*key;

	key = key_full(arg, '=');
	found = find_in_env(*env, key);
	if (found)
	{
		if (!ft_strcmp(found->key, "_"))
		{
			remove_if(*env);
			free(key);
			return ;
		}
		if (ft_strchr(arg, '='))
			update_found_node(found, arg);
	}
	else if (!found && ft_strcmp(key, "_"))
	{
		new = ft_newnode(ft_strdup(arg), '=');
		ft_lstadd_back(env, new);
	}
	free(key);
}

void	append_to_found(t_env *found, char *key, char *value)
{
	if (found->value)
		found->value = ft_join(found->value, value);
	else
		found->value = ft_strdup(value);
	free(found->line);
	found->line = ft_join(key, "=");
	found->line = ft_join(found->line, found->value);
}

void	with_plus(t_env **env, char *arg)
{
	t_env	*new;
	t_env	*found;
	char	*key;
	char	*value;

	key = key_full(arg, '+');
	found = find_in_env(*env, key);
	if (found)
	{
		if (!ft_strcmp(found->key, "_"))
			return (remove_if(*env), free(key));
		value = ft_strchr(arg, '=');
		if (value)
			append_to_found(found, key, value + 1);
	}
	else if (!found && ft_strcmp(key, "_"))
	{
		new = ft_newnode(ft_strdup(arg), '+');
		ft_lstadd_back(env, new);
	}
	free(key);
}
