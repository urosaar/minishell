/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:14 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/20 15:48:19 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtins(t_env **env, char **args, char *prev_pwd)
{
	int last_status;

	last_status = 0;
	if (*args && !ft_strcmp(*args, "echo"))
		last_status = ft_echo(args);
	else if (*args && !ft_strcmp(*args, "pwd"))
		last_status = ft_pwd(*env, prev_pwd);
	else if (*args && !ft_strcmp(*args, "cd"))
		last_status = ft_cd(env, args, prev_pwd);
	else if (*args && !ft_strcmp(*args, "exit"))
		ft_exit(*env, args);
	else if (*args && !ft_strcmp(*args, "env"))
		last_status = ft_env(env, args);
	else if (*args && !ft_strcmp(*args, "unset"))
		last_status = ft_unset(env, args);
	else if (*args && !ft_strcmp(*args, "export"))
		last_status = ft_export(env, args);
	return (last_status);
}

int	is_builtins(char **args)
{
	if (*args && !ft_strcmp(*args, "echo"))
		return (1);
	else if (*args && !ft_strcmp(*args, "pwd"))
		return (1);
	else if (*args && !ft_strcmp(*args, "cd"))
		return (1);            
	else if (*args && !ft_strcmp(*args, "exit"))
		return (1);            
	else if (*args && !ft_strcmp(*args, "env"))
		return (1);        
	else if (*args && !ft_strcmp(*args, "unset"))
		return (1);              
	else if (*args && !ft_strcmp(*args, "export"))
		return (1);
	return (0);             
}
