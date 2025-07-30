/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:07:34 by jesse             #+#    #+#             */
/*   Updated: 2025/07/29 21:45:12 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_substr(const char *src, int start, int end)
{
    int len = end - start;
    char *s = malloc(len + 1);
    int i = 0;
    if (!s)
        return (NULL);
    while (i < len)
    {
        s[i] = src[start + i];
        i++;
    }
    s[len] = '\0';
    return (s);
}
int is_only_whitespace(const char *input)
{
    int i = 0;
    while (input[i])
    {
        if (!is_space(input[i]))
            return (0);
        i++;
    }
    return (1);
}
void *ft_calloc(size_t count, size_t size)
{
    void *ptr = malloc(count * size);
    if (!ptr)
        return (NULL);
    size_t total_size = count * size;
    size_t i = 0;
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