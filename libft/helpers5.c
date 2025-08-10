/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:20:27 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 19:12:00 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_env	*ft_newnode(char *line, char c)
{
	t_env	*newnode;
	char	*equal;

	if (!line)
		return (NULL);
	newnode = ft_malloc(sizeof(t_env), MALLOC);
	if (!newnode)
		return (NULL);
	newnode->key = key_full(line, c);
	equal = strchr(line, '=');
	if (equal)
	{
		newnode->value = ft_strdup(equal + 1);
		newnode->line = ft_join(newnode->key, "=");
		newnode->line = ft_join(newnode->line, newnode->value);
	}
	else
	{
		newnode->value = NULL;
		newnode->line = ft_strdup(line);
	}
	newnode->next = NULL;
	return (newnode);
}

t_env	*ft_lstlast(t_env *env)
{
	if (!env)
	{
		printf("HELLO\n");
		return (0);
	}
	while (env && env->next)
	{
		env = env->next;
	}
	return (env);
}

void	ft_lstadd_back(t_env **env, t_env *new)
{
	t_env	*last;

	if (!env && !new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	last = ft_lstlast(*env);
	last->next = new;
}

char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_join(char *s1, char *s2)
{
	char	*newstring;
	int		i;
	int		j;
	int		len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2);
	newstring = ft_malloc(len + 1, MALLOC);
	if (!newstring)
		return (free(s1), NULL);
	i = -1;
	while (s1[++i])
		newstring[i] = s1[i];
	j = 0;
	while (s2[j])
		newstring[i++] = s2[j++];
	newstring[i] = '\0';
	return (newstring);
}
