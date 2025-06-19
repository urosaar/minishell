/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:52 by skhallou          #+#    #+#             */
/*   Updated: 2025/06/19 15:19:50 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_nbr(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	check_error(t_env *env, char *arg)
{
	printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", arg);
	// if(env)
		free_env(env);
	exit(2);
}
long	calcul(t_env *env, char *arg, int sign)
{
	long long	oldnbr;
	long long	r;
	int	i;

	i = 0;
	r = 0;
	while (arg[i] && arg[i] >= '0' && arg[i] <= '9')
	{
		oldnbr = r;
		r = r * 10 + (arg[i++] - '0');
		if ((r < oldnbr && sign > 0) || (r < oldnbr && sign < 0))
			check_error(env, arg);
	}
	return (r);
}
long	ft_atoi(t_env *env, char *arg)
{
	long long	r;
	int			i;
	int			sign;

	i = 0;
	r = 0;
	sign = 1;
	if (!strcmp(arg, "-9223372036854775808"))
		return (0);
	while (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i++] == '-')
			sign *= -1;
	}
	if (is_nbr(&arg[i]))
		r = calcul(env, &arg[i], sign);
	else
		check_error(env, arg);
	return (r * sign);
}

void	ft_check_arg(t_env *env, char *arg)
{
	long	r;

	r = ft_atoi(env, arg);
	printf("exit\n");
	free_env(env);
	exit(r);
}

void	ft_exit(t_env *env, char **arg)
{
	int	i;

	i = 0;
	if (!arg[1])
	{
		free_env(env);
		exit(0);
	}
	while ((arg[1][i] == ' ' || (arg[1][i] >= 9 && arg[1][i] <= 13)) && arg[1][i + 1] != '\0')
		i++;
	if (arg[1] && arg[2] && (ft_atoi(env, arg[1]) || !ft_atoi(env, arg[1])))
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return;
	}
	else if (arg[1] && arg[2] && !is_nbr(arg[1]))
		check_error(env, arg[1]);
	ft_check_arg(env, arg[1]);
}
