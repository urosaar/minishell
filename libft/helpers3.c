/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:08:17 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/12 21:08:19 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	num;

	num = n;
	len = ft_numlen(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (num == 0)
		str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	while (num)
	{
		str[len--] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}

int	is_nbr(char *arg)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (!arg)
		return (0);
	while (arg[i])
	{
		if ((arg[i] < '0' || arg[i] > '9') && !(arg[i] == ' '
				|| (arg[i] >= 9 && arg[i] <= 13)))
			return (0);
		if (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13))
			flag = 1;
		if ((arg[i] >= '0' && arg[i] <= '9') && flag == 1)
			return (0);
		i++;
	}
	return (1);
}

void	save_shell_term(t_exec *exec)
{
	tcgetattr(STDIN_FILENO, &exec->shell_termios);
}

void	restore_shell_term(t_exec *exec)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &exec->shell_termios);
}
