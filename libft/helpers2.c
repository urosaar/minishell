/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:08:11 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/12 21:08:13 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *src, int start, int end)
{
	int		len;
	char	*s;
	int		i;

	len = end - start;
	s = malloc(len + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (i < len)
	{
		s[i] = src[start + i];
		i++;
	}
	s[len] = '\0';
	return (s);
}

int	is_only_whitespace(const char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!is_space(input[i]))
			return (0);
		i++;
	}
	return (1);
}

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	size_t			total_size;
	size_t			i;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	total_size = count * size;
	i = 0;
	while (i < total_size)
	{
		((unsigned char *)ptr)[i] = 0;
		i++;
	}
	return (ptr);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dst && !src)
		return (NULL);
	while (i < n && dst != src)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}
