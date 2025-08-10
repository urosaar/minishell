/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_card2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 05:00:13 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/10 05:02:24 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr != NULL)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}

bool	wildcard_match(const char *pattern, const char *str)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern)
				return (true);
			while (*str)
			{
				if (wildcard_match(pattern, str))
					return (true);
				str++;
			}
			return (false);
		}
		if (*pattern != *str)
			return (false);
		pattern++;
		str++;
	}
	return (*str == '\0');
}

int	cmp_str(const void *a, const void *b)
{
	return (ft_strcmp(*(char *const *)a, *(char *const *)b));
}
