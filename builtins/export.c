/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:05 by skhallou          #+#    #+#             */
/*   Updated: 2025/07/21 17:55:13 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	swap_nodes(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;
	char	*tmp_line;

	tmp_key = a->key;
	a->key = b->key;
	b->key = tmp_key;
	tmp_value = a->line;
	a->line = b->line;
	b->line = tmp_value;
	tmp_line = a->value;
	a->value = b->value;
	b->value = tmp_line;
}

void	swap_line(t_env **head)
{
	t_env *node;

	if (!head || !*head)
		return;
	node = *head;
	while (node && node->next)
	{
		if (node->key && ft_strcmp(node->key, node->next->key) > 0)
		{
			swap_nodes(node, node->next);
			node = *head;
		}
		else
			node = node->next;
	}
}

char	*key_full(char *line, char c)
{
	char	*key;
	int		i;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && line[i] != c)
		i++;
	key = ft_malloc(sizeof(char) * (i + 1), MALLOC);
	if (!key)
		return (NULL);
	i = 0;
	while (line[i] && line[i] != c)
	{
		key[i] = line[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

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

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (!(str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (2);
		else if (!(str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

void	remove_if(t_env *env)
{
	t_env	*tmp;
	t_env	*node;
	t_env	*prev;

	node = env;
	prev = NULL;
	while (node)
	{
		if (node->key && ft_strcmp(node->key, "_") == 0)
		{
			tmp = node->next;
			if (prev)
				prev->next = tmp;
			else
				env = tmp;
			free(node->line);
			free(node->key);
			free(node->value);
			return;
		}
		prev = node;		
		node = node->next;
	}
}

void	update_found_node(t_env *found, char *arg)
{
	char *value;

	value = strchr(arg, '=');
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
		if (strchr(arg, '='))
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
			return(remove_if(*env), free(key));
		value = strchr(arg, '=');
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
