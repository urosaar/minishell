/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_card.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:52:23 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 05:02:05 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_entry(char ***matches, int match_count,
				const char *pattern, struct dirent *entry)
{
	if (ft_strcmp(entry->d_name, ".") == 0
		|| ft_strcmp(entry->d_name, "..") == 0)
		return (match_count);
	if (entry->d_name[0] == '.' && pattern[0] != '.')
		return (match_count);
	if (wildcard_match(pattern, entry->d_name))
	{
		*matches = ft_realloc(*matches, sizeof(char *) * match_count,
				sizeof(char *) * (match_count + 1));
		if (!(*matches))
			return (-1);
		(*matches)[match_count] = ft_strdup(entry->d_name);
		if (!(*matches)[match_count])
			return (-1);
		match_count++;
	}
	return (match_count);
}

static char	**collect_matches(const char *pattern, int *count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				match_count;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = NULL;
	match_count = 0;
	entry = readdir(dir);
	while (entry)
	{
		match_count = process_entry(&matches, match_count, pattern, entry);
		if (match_count == -1)
		{
			free_strarray(matches);
			closedir(dir);
			return (NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	*count = match_count;
	return (matches);
}

static void	process_wildcard(char *arg, int *new_count, char ***new_args)
{
	int		mcount;
	char	**matches;
	int		j;

	matches = collect_matches(arg, &mcount);
	if (mcount > 0)
	{
		qsort(matches, mcount, sizeof(char *), cmp_str);
		j = 0;
		j = 0;
		while (j < mcount)
		{
			*new_args = ft_realloc(*new_args, sizeof(char *) * (*new_count),
					sizeof(char *) * (*new_count + 1));
			(*new_args)[(*new_count)++] = matches[j++];
		}
		free(matches);
	}
	else
	{
		*new_args = ft_realloc(*new_args, sizeof(char *) * (*new_count),
				sizeof(char *) * (*new_count + 1));
		(*new_args)[*new_count] = ft_strdup(arg);
		(*new_count)++;
	}
}

static void	copy_or_expand_arg(t_command *cmd, bool *no_expand,
		char ***new_args, int *new_count)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (no_expand[i] || !ft_strchr(cmd->args[i], '*'))
		{
			*new_args = ft_realloc(*new_args,
					sizeof(char *) * (*new_count),
					sizeof(char *) * (*new_count + 1));
			(*new_args)[(*new_count)++] = ft_strdup(cmd->args[i]);
		}
		else
			process_wildcard(cmd->args[i], new_count, new_args);
		i++;
	}
}

void	expand_wildcards(t_command *cmd, bool *no_expand)
{
	char	**new_args;
	int		new_count;
	int		i;

	new_args = NULL;
	new_count = 0;
	if (cmd->args[0] != NULL)
	{
		new_args = ft_realloc(new_args, 0, sizeof(char *));
		new_args[new_count++] = ft_strdup(cmd->args[0]);
	}
	copy_or_expand_arg(cmd, no_expand, &new_args, &new_count);
	new_args = ft_realloc(new_args, sizeof(char *) * new_count,
			sizeof(char *) * (new_count + 1));
	new_args[new_count] = NULL;
	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	cmd->args = new_args;
}
