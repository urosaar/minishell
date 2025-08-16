/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:53:07 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/17 00:30:31 by jesse            ###   ########.fr       */
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
	int	flag;

	if (curr->redirections)
	{
		flag = apply_redirection(curr, env);
		if (flag)
			ctx->last_status = 0;
		else
			ctx->last_status = 1;
		if ((curr->cmd && curr->cmd[0] == '\0') && flag)
		{
			ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
			ctx->last_status = 127;
		}
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

char	*ft_check1(char *cmd, t_exec *ctx)
{
	int	dirfd;

	if (!cmd)
		return (NULL);
	dirfd = open(cmd, O_DIRECTORY);
	if (dirfd >= 0)
	{
		close(dirfd);
		ctx->last_status = 126;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (NULL);
	}
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
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

char	*check_if_exist(t_env *env, t_command *cmds, t_exec *ctx)
{
	char	**path;
	char	*d;
	t_env	*tmp;

	1 && (path = NULL, tmp = env, d = NULL);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PATH"))
		{
			path = path_check(tmp);
			break ;
		}
		tmp = tmp->next;
	}
	if (!path || ft_strchr(cmds->cmd, '/'))
		d = ft_check1(cmds->cmd, ctx);
	if (d)
		return (d);
	else if (!d && ft_strchr(cmds->cmd, '/'))
		return (NULL);
	if (path)
		d = ft_check2(path, cmds->cmd);
	if (d)
		return (d);
	return (free_array(path), NULL);
}
