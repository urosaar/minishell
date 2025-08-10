/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:42:07 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 19:56:04 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_words_isspace(const char *s)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (s[i] && !is_space(s[i]))
		{
			words++;
			while (s[i] && !is_space(s[i]))
				i++;
		}
	}
	return (words);
}

static int	heredoc(t_redirection *tmp)
{
	if (tmp->type == TOKEN_HEREDOC && tmp->heredoc_fd >= 0)
	{
		if (dup2(tmp->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 heredoc");
			return (0);
		}
		close(tmp->heredoc_fd);
		tmp->heredoc_fd = -1;
		return (1);
	}
	return (0);
}

int	check_clean(char *clean, char *filename)
{
	int	words;

	words = count_words_isspace(clean);
	if (words != 1 || (clean[0] == (char)127 && clean[1] == '\0'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		free(clean);
		g_status = 1;
		return (0);
	}
	return (1);
}

int	handle_single_redirection(t_redirection *tmp, t_env **env)
{
	char	*expanded;
	char	*clean;
	int		ok;

	if (heredoc(tmp))
		return (1);
	if (tmp->quoted && tmp->quote_char == '\'')
		expanded = ft_strdup(tmp->filename);
	else
		expanded = expand_variables(tmp->filename, g_status, env);
	if (expanded == NULL)
		return (0);
	clean = strip_quotes(expanded);
	free(expanded);
	if (clean == NULL)
		return (0);
	if (ft_strchr(tmp->filename, '$'))
	{
		if (!tmp->quoted && !check_clean(clean, tmp->filename))
			return (0);
	}
	ok = apply_redirect(tmp, clean);
	free(clean);
	return (ok);
}
