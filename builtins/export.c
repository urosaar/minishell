/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:15:05 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/09 17:15:06 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	swap_line(t_env **head)
{
	t_env	*node;
	char	*tmp_key;
	char	*tmp_line;
	char	*tmp_value;

	if (!head || !*head)
		return;
	node = *head;
	while (node && node->next) 
	{
		if (node->key && strcmp(node->key, node->next->key) > 0) 
		{
			tmp_key = node->key;
			tmp_line = node->line;
			tmp_value = node->value;
			node->key = node->next->key;
			node->line = node->next->line;
			node->value = node->next->value;
			node->next->key = tmp_key;
			node->next->line = tmp_line;
			node->next->value = tmp_value;
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
	key = malloc(sizeof(char) * (i + 1));
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

char	*check_for_quote(char *value)
{
	char	*d;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	if (!value)
		return (NULL);
	len = ft_strlen(value);
	d = malloc(len + 1);
	if (!d)
		return (NULL);
	while (value[i])
	{
		if (value[i] == '"')
			i++;
		if ( value[i] && value[i] != '"')
		{
			d[j] = value[i];
			j++;
		}
		i++;
	}
	d[j] = '\0';
	return (free(value), d);
}

t_env	*find_in_env(t_env *env, char *key, char c)
{
	while (env)
	{
		if (env->key && strcmp(env->key, key) == 0)
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
		if (node->key && strcmp(node->key, "_") == 0)
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

void	without_plus(t_env **env, char *arg)
{
	t_env	*new;
	t_env	*found;
	char	*key;
	char	*equal;

	key = key_full(arg, '=');
	found = find_in_env(*env, key, '=');
	if (found)
	{
		if (!strcmp(found->key, "_"))
		{
			remove_if(found);
			return;
		}
		char *value = strchr(arg, '=');
		equal = strchr(arg, '=');
		if (equal)
		{
			free(found->line);
			found->line = strdup(arg);
			if (found->value)
				free(found->value);
			found->value = strdup(value + 1);
			printf("HERE\n");
		}
		// else if (!equal && strchr(arg, '='))
		// {
		//     if (found->value)
		//     free(found->value);
		//     found->value = strdup(equal + 1);
		// }
	}
	else if (!found && strcmp(key, "_"))
	{
		new = ft_newnode(strdup(arg), '=');
		ft_lstadd_back(env, new);
	}
	free(key);
}

void	with_plus(t_env **env, char *arg)
{
	t_env	*new;
	t_env	*found;
	char	*key;
	char	*equal;

	key = key_full(arg, '+');
	found = find_in_env(*env, key, '+');
	if (found)
	{
		if (!strcmp(found->key, "_"))
		{            
			remove_if(found);
			return;
		}
		found->value = check_for_quote(found->value);
		equal = strchr(found->line, '=');
		char *value = strchr(arg, '=');
		if (value)
		{
			if (found->value)
				found->value = ft_join(found->value, value + 1);
			else
				found->value = strdup(value + 1);
			free(found->line);
			found->line = ft_join(key, "=");
			found->line = ft_join(found->line, found->value);
		}
	}
	else if (!found && strcmp(key, "_"))
	{
		new = ft_newnode(strdup(arg), '+');
		ft_lstadd_back(env, new);
	}
	free(key);
}


void	ft_export(t_env **env, char **arg)
{
	t_env	*copy;
	t_env	*tmp;
	int	i;

	i = 1;
	if (!arg[1])
	{
		copy = copy_for_expo(*env);
		swap_line(&copy);
		tmp = copy;
		while (tmp)
		{
			printf("declare -x %s", tmp->key);
			if (tmp->value)
			{
				tmp->value = check_for_quote(tmp->value);
					printf("=\"%s\"", tmp->value);
				// else
				//     printf("=%s", tmp->value);
			}
			printf("\n");
			tmp = tmp->next;
		}
		free_env(copy);
		return;
	}
	while (arg[i])
	{
		if (!is_valid_identifier(arg[i]))
			printf("export: `%s': not a valid identifier\n", arg[i]);
		else if (is_valid_identifier(arg[i]) == 1)
		{
			without_plus(env, arg[i]);
		}
		else
			with_plus(env, arg[i]);
		i++;
	}
}
