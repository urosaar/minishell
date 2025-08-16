/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers8.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:29:54 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/16 17:54:30 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_pipe(char **token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (ft_strcmp(token[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_errno(char **args, char **envp)
{
	char	*arg[3];

	arg[0] = "/bin/sh";
	arg[1] = args[0];
	arg[2] = NULL;
	if (execve("/bin/sh", arg, envp) == -1)
	{
		free_envp(envp);
		perror("minishell: execve");
		exit(1);
	}
}

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	if (d == s || n == 0)
		return (dst);
	if (d < s)
	{
		while (n--)
			*d++ = *s++;
	}
	else
	{
		d += n;
		s += n;
		while (n--)
			*--d = *--s;
	}
	return (dst);
}

void	there_is_redir(t_command *curr)
{
	if (curr->redirections != NULL)
	{
		if (!curr->cmd || curr->cmd[0] == '\0')
		{
			ft_putstr_fd("minishell: : command not found\n", 2);
			exit(127);
		}
	}
}
