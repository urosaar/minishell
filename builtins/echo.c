#include "../minishell.h"

void	check_option(char **args, int *flag, int *t)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (args[i] && args[i][j] && args[i][j] == '-')
	{
		while (args[i][++j])
		{
			if (args[i][j] == 'n')
			{
				if (args[i][j + 1] == '\0')
				{
					(*t)++;
					*flag = 0;
				}
			}
			else
				return;
		} 
		if (!args[i][j] && args[i][j - 1] == '-')
			return;
		j = 0;
		i++;
	}
}

void	ft_echo(char **args)
{
	int i = 1;
	int flag = 1;

	check_option(args, &flag, &i);
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (flag)
		write(1, "\n", 1);
}
