/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_card.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:52:23 by jesse             #+#    #+#             */
/*   Updated: 2025/08/09 21:29:41 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <dirent.h>

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr != NULL)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
static bool	wildcard_match(const char *pattern, const char *str)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern)
				return (true);
			while (*str)
			{
				if (wildcard_match(pattern, str))
					return (true);
				str++;
			}
			return (false);
		}
		if (*pattern != *str)
			return (false);
		pattern++;
		str++;
	}
	return (*str == '\0');
}

static int	cmp_str(const void *a, const void *b)
{
	return (ft_strcmp(*(char *const *)a, *(char *const *)b));
}

static char	**collect_matches(const char *pattern, int *count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				match_count;

	dir = opendir(".");
	matches = NULL;
	match_count = 0;
	if (dir == NULL)
		return (NULL);
	while ((entry = readdir(dir)))
	{
		if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
			continue ;
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (wildcard_match(pattern, entry->d_name))
		{
			matches = ft_realloc(matches, sizeof(char *) * match_count,
					sizeof(char *) * (match_count + 1));
			matches[match_count] = ft_strdup(entry->d_name);
			match_count++;
		}
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
		while (j < mcount)
		{
			*new_args = ft_realloc(*new_args, sizeof(char *) * (*new_count),
					sizeof(char *) * (*new_count + 1));
			(*new_args)[*new_count] = matches[j];
			(*new_count)++;
			j++;
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

void	expand_wildcards(t_command *cmd, bool *no_expand)
{
	char	**new_args;
	int		new_count;
	int		i;

	new_args = NULL;
	new_count = 0;
	i = 0;
	if (cmd->args[0] != NULL)
	{
		new_args = ft_realloc(new_args, 0, sizeof(char *));
		new_args[new_count++] = ft_strdup(cmd->args[0]);
	}
	while (cmd->args[++i])
	{
		if (no_expand[i] || !ft_strchr(cmd->args[i], '*'))
		{
			new_args = ft_realloc(new_args, sizeof(char *) * new_count,
					sizeof(char *) * (new_count + 1));
			new_args[new_count++] = ft_strdup(cmd->args[i]);
		}
		else
			process_wildcard(cmd->args[i], &new_count, &new_args);
	}
	new_args = ft_realloc(new_args, sizeof(char *) * new_count,
			sizeof(char *) * (new_count + 1));
	new_args[new_count] = NULL;
	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	cmd->args = new_args;
}
