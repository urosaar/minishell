/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:38 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/19 15:19:47 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_env(t_env **env)
{
    t_env   *tmp;

    tmp = *env;
    while (tmp)
    {
        if (strchr(tmp->line, '='))
            printf("%s\n", tmp->line);
        tmp = tmp->next;
    }
}
