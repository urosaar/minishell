/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:08:23 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/14 17:39:56 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (NULL);
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
	return (NULL);
}

static int	count_word(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*count_word_len(char *s, char c)
{
	char	*new;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (s[len] && s[len] != c)
		len++;
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	while (i < len)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

static char	**full_word(char *s, char **array, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			array[j] = count_word_len(&s[i], c);
			if (!array[j])
				free_array(array);
			j++;
		}
		while (s[i] && s[i] != c)
			i++;
	}
	array[j] = NULL;
	return (array);
}

char	**ft_split(char *s, char c)
{
	int		n;
	char	**array;

	if (!s)
		return (NULL);
	n = count_word(s, c);
	array = malloc(sizeof(char *) * (n + 1));
	if (!array)
		return (NULL);
	return (full_word(s, array, c));
}
