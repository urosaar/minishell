/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:53:07 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/01 19:53:13 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**build_env_array(t_env **env)
{
	int		count;
	t_env	*tmp;
	char	**envp;
	int		i;

	count = 0;
	tmp = *env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = ft_malloc((count + 1) * sizeof(char *), MALLOC);
	tmp = *env;
	i = 0;
	while (tmp)
	{
		envp[i] = ft_join(tmp->key, "=");
		envp[i] = ft_join(envp[i], tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

char	*ft_check1(char *cmd)
{
	int	dirfd;

	if (!cmd)
		return (NULL);
	dirfd = open(cmd, O_DIRECTORY);
	if (dirfd >= 0)
	{
		close(dirfd);
		fprintf(stderr, "minishell: %s: is a directory\n", cmd);
		return (NULL);
	}
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (ft_strchr(cmd, '/'))
		fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
	return (NULL);
}

char	*ft_check2(char **path, char *cmd)
{
	char	*s;
	char	*d;
	int		i;

	i = 0;
	while (path[i] && cmd)
	{
		s = ft_join("/", cmd);
		d = ft_join(path[i], s);
		free(s);
		if (access(d, X_OK) == 0)
		{
			free_array(path);
			return (d);
		}
		free(d);
		i++;
	}
	return (NULL);
}

char	*check_if_exist(t_env *env, t_command *cmds)
{
	char	**path;
	char	*d;
	t_env	*tmp;

	path = NULL;
	tmp = env;
	d = ft_check1(cmds->cmd);
	if (d)
		return (d);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PATH"))
		{
			path = ft_split(tmp->value, ':');
			break ;
		}
		tmp = tmp->next;
	}
	if (!path)
		return (NULL);
	d = ft_check2(path, cmds->cmd);
	if (d)
		return (d);
	return (free_array(path), NULL);
}
