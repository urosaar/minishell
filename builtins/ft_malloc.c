/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 20:13:23 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/25 17:11:34 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

void	ft_free_exit(void *ptr, t_malloc **head)
{
	printf("minishell : error malloc failed\n");
	free(ptr);
	free_all(head);
	exit(1);
}

void	*ft_malloc(size_t size, t_type type)
{
	static t_malloc	*head;
	t_malloc			*tmp;
	void			*ptr;

	if (type == MALLOC)
	{
		ptr = malloc(size);
		if (!ptr)
		{
			printf("minishell : error malloc failed\n");
			free_all(&head);
			exit(1);
		}
		tmp = new_node(ptr);
		if (!tmp)
			ft_free_exit(ptr, &head);
		add_back(&head, tmp);
		return (ptr);
	}
	else if (type == FREE)
		free_all(&head);
	return (NULL);
}
