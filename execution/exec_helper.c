/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:53:07 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/10 19:52:27 by jesse            ###   ########.fr       */
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

int	handle_empty_cmd(t_command *curr, t_env **env, t_exec *ctx)
{
	if (curr->redirections)
	{
		if (apply_redirection(curr, env))
			ctx->last_status = 0;
		else
			ctx->last_status = 1;
		return (1);
	}
	if (!curr->next)
	{
		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
		ctx->last_status = 127;
		return (1);
	}
	return (0);
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
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (NULL);
	}
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (ft_strchr(cmd, '/'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
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
