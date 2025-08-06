/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 18:45:08 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/06 17:41:43 by skhallou         ###   ########.fr       */
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
	t_env	*node;

	if (!head || !*head)
		return ;
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
			return ;
		}
		prev = node;
		node = node->next;
	}
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (!(str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= 'a' && str[i] <= 'z')))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (2);
		else if (!(str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= '0' && str[i] <= '9')))
		{
			return (0);
		}
		i++;
	}
	return (1);
}
