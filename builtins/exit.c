/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:14:52 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/11 18:39:32 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_error(t_env *env, char *arg, int flag)
{
	if (flag)
		printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", arg);
	free_env(env);
	exit(255);
}

long	calcul(t_env *env, char *arg, int sign, int falg)
{
	long long	oldnbr;
	long long	r;
	int			i;

	i = 0;
	r = 0;
	while (arg[i] && arg[i] >= '0' && arg[i] <= '9')
	{
		oldnbr = r;
		r = r * 10 + (arg[i++] - '0');
		if ((r < oldnbr && sign > 0) || (r < oldnbr && sign < 0))
			check_error(env, arg, falg);
	}
	return (r);
}

long	ft_atoi(t_env *env, char *arg, int flag)
{
	long long	r;
	int			sign;

	r = 0;
	sign = 1;
	if (!ft_strcmp(arg, "-9223372036854775808"))
		return (0);
	while (*arg == ' ' || (*arg >= 9 && *arg <= 13))
		arg++;
	if (*arg == '+' || *arg == '-')
	{
		if (*arg == '-')
			sign *= -1;
		arg++;
	}
	if (is_nbr(arg))
		r = calcul(env, arg, sign, flag);
	else
		check_error(env, arg, flag);
	return (r * sign);
}

void	ft_check_arg(t_env *env, char *arg, int flag)
{
	long	r;

	r = ft_atoi(env, arg, flag);
	if (arg[0] == '\0' || (arg[0] == ' ' && r == 0))
		check_error(env, arg, flag);
	else if (flag)
		printf("exit\n");
	free_env(env);
	exit(r);
}

int	ft_exit(t_env *env, char **arg, int flag)
{
	if (!arg[1])
	{
		if (flag)
			printf("exit\n");
		free_env(env);
		exit(0);
	}
	if (arg[1] && arg[2] && (ft_atoi(env, arg[1], flag)
			|| !ft_atoi(env, arg[1], flag)))
	{
		if (flag)
			printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else if (arg[1] && arg[2] && !is_nbr(arg[1]))
		check_error(env, arg[1], flag);
	ft_check_arg(env, arg[1], flag);
	return (0);
}
