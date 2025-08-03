/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:09:34 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 19:36:32 by skhallou         ###   ########.fr       */
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
//for command "top"
void save_shell_term(t_exec *exec)
{
    tcgetattr(STDIN_FILENO, &exec->shell_termios);
}

void restore_shell_term(t_exec *exec)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &exec->shell_termios);
}