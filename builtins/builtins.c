/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:14 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/09 17:14:16 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    builtins(t_env **env, char **args, char *prev_pwd)
{
    if (*args && !ft_strcmp(*args, "echo"))
        ft_echo(args);
    else if (*args && !ft_strcmp(*args, "pwd"))
        ft_pwd(*env, prev_pwd);
    else if (*args && !ft_strcmp(*args, "cd"))
        ft_cd(env, args, prev_pwd);
    else if (*args && !ft_strcmp(*args, "exit"))
        ft_exit(*env, args);
    else if (*args && !ft_strcmp(*args, "env"))
        ft_env(env);
    else if (*args && !ft_strcmp(*args, "unset"))
        ft_unset(env, args);
    else if (*args && !ft_strcmp(*args, "export"))
        ft_export(env, args);
}

int is_builtins(char **args)
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
