/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers8.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:29:54 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/15 16:37:53 by skhallou         ###   ########.fr       */
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
