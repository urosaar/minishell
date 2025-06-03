#include "../minishell.h"

void	replace(t_env *env, char *path, char *oldpath)
{
	t_env	*node;

	node = env;
	while (node)
	{
		
		if (node->line && !(strcmp(node->key, "PWD")))
		{
			free(node->line);
			if (node->value)
			free(node->value);
			node->value = strdup(path);
			node->line = ft_join(ft_strdup("PWD="), node->value);
		}
		else if (node->line && !(strcmp(node->key, "OLDPWD")))
		{
			free(node->line);
			if (node->value)
				free(node->value);
			node->value = strdup(oldpath);
			node->line = ft_join(ft_strdup("OLDPWD="), node->value);
		}
		node = node->next;
	}
}

void	ft_cd(t_env **env, char **arg, char *prev_pwd)
{
	t_env	*node;
	char	*oldpath;
	char	*path;
	int		i;

	path = NULL;
	node = *env;
	oldpath = getcwd(NULL, 0);    
	if (!oldpath)
		oldpath = strdup(prev_pwd);
	if (!arg[1] || (!strcmp(arg[1], "~")  && !arg[2]))
	{
		while (node)
		{
			if (!strcmp(node->key, "HOME"))
			{
				path = strdup(node->value);
				break;
			}
			node = node->next;
		}
		if (!path)
		{
			printf("bash: cd: HOME not set\n");
			return;
		}
		i = chdir(path);
		free(path);
	}
	else
		i = chdir(arg[1]);
	if (i == -1)
	{
		if (arg[1])
			printf("bash: cd: %s: No such file or directory\n", arg[1]);
		return;
	}
	path = getcwd(NULL, 0);
	if (!path)
		return;
	replace(*env, path, oldpath);
	free(oldpath);
}
