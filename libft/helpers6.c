/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:26:14 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 19:11:25 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || (c >= '0' && c <= '9'));
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (s1[i] && i < n)
		i++;
	dup = (char *)malloc(i + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n--)
		*p++ = (unsigned char)c;
	return (s);
}

char	*ft_strpbrk(const char *s, const char *accept)
{
	const char	*a;

	while (*s)
	{
		a = accept;
		while (*a)
		{
			if (*s == *a)
				return ((char *)s);
			a++;
		}
		s++;
	}
	return (NULL);
}
