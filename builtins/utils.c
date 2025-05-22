#include "../minishell.h"

t_env	*ft_newnode(char *line, char c)
{
	t_env	*newnode;
	char	*equal;

	if (!line)
		return (NULL);
	newnode = malloc(sizeof(t_env));
	if (!newnode)
		return (NULL);
	newnode->key = key_full(line, c);
	equal = strchr(line, '=');
	if (equal)
	{
		newnode->value = strdup(equal + 1);
		newnode->line = ft_join(newnode->key, "=");
		newnode->line = ft_join(newnode->line, newnode->value);
	}
	else
	{
		newnode->value = NULL;
		newnode->line = strdup(line);
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
int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s)
		return;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;

	if (!s1 && !s2)
		return 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

char *ft_strchr(char *s, int c)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strdup(char *src)
{
	int		i;
	char	*dest;

	i = 0;
	if (!src)
		return (NULL);
	dest = malloc(ft_strlen(src) + 1);
	if (!dest)
		return (0);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

char	*ft_join(char *s1, char *s2)
{
	char	*newstring;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2);
	newstring = malloc(len + 1);
	if (!newstring)
		return (free(s1), NULL);
	while (s1[i])
	{
		newstring[i] = s1[i];
		i++;
	}
	while (s2[j])
		newstring[i++] = s2[j++];
	newstring[i] = 0;
	return (newstring);
}
