/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:59:46 by skhallou          #+#    #+#             */
/*   Updated: 2025/08/15 19:49:36 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(1);
}

static char *apend_char(char *buf, char c)
{
	size_t len = buf ? ft_strlen(buf) : 0;
	char *new = malloc(len + 2);
	if (!new)
	{
		free(buf);
		return NULL;
	}
	if (buf)
	{
		ft_memcpy(new, buf, len);
		free(buf);
	}
	new[len] = c;
	new[len + 1] = '\0';
	return new;
}

static char *append_str(char *buf, const char *s)
{
	if (!s || s[0] == '\0')
		return buf;
	if (!buf)
		return ft_strdup(s);
	size_t lenb = ft_strlen(buf);
	size_t lens = ft_strlen(s);
	char *new = malloc(lenb + lens + 1);
	if (!new)
	{
		free(buf);
		return NULL;
	}
	ft_memcpy(new, buf, lenb);
	ft_memcpy(new + lenb, s, lens + 1);
	free(buf);
	return new;
}

static char *heredoc_expand_line(const char *line, int last_status, t_env **env)
{
    size_t i = 0;
    char *out = NULL;

    while (line[i])
    {
        if (line[i] == '$')
        {
            if (line[i + 1] == '?')
            {
                char *tok = ft_strdup("$?");
                if (!tok) { free(out); return NULL; }
                char *exp = expand_variables(tok, last_status, env);
                free(tok);
                if (exp)
                {
                    out = append_str(out, exp);
                    free(exp);
                }
                i += 2;
            }
            else if ((line[i + 1] >= 'A' && line[i + 1] <= 'Z') ||
                     (line[i + 1] >= 'a' && line[i + 1] <= 'z') ||
                     line[i + 1] == '_')
            {
                size_t j = i + 1;
                while (line[j] && ((line[j] >= 'A' && line[j] <= 'Z') ||
                                   (line[j] >= 'a' && line[j] <= 'z') ||
                                   (line[j] >= '0' && line[j] <= '9') ||
                                   line[j] == '_'))
                    j++;
                size_t toklen = j - i;
                char *tok = ft_strndup(line + i, toklen);
                if (!tok) { free(out); return NULL; }
                char *exp = expand_variables(tok, last_status, env);
                free(tok);
                if (exp)
                {
                    out = append_str(out, exp);
                    free(exp);
                }
                i = j;
            }
            else 
            {
                out = apend_char(out, '$');
                i++;
            }
        }
        else
        {
            out = apend_char(out, line[i]);
            i++;
        }
    }

    if (!out)
        out = ft_strdup("");

    return out;
}


static void process_heredoc_line(t_heredoc_child *data, char *line)
{
	char *expanded;

	if (!data->quoted)
	{
		expanded = heredoc_expand_line(line, data->last_status, data->env);
		if (expanded)
		{
			write(data->write_fd, expanded, ft_strlen(expanded));
			write(data->write_fd, "\n", 1);
			free(expanded);
		}
		else
		{
			write(data->write_fd, line, ft_strlen(line));
			write(data->write_fd, "\n", 1);
		}
	}
	else
	{
		write(data->write_fd, line, ft_strlen(line));
		write(data->write_fd, "\n", 1);
	}
}


int	heredoc_iteration(t_heredoc_child *data, int *line_count)
{
	char	*line;

	if (*line_count >= HEREDOC_MAX_LINES)
	{
		ft_putstr_fd("minishell: heredoc limit exceeded\n", STDERR_FILENO);
		close(data->write_fd);
		exit(1);
	}
	line = readline("> ");
	if (!line)
		return (0);
	if (ft_strcmp(line, data->delimiter) == 0)
	{
		free(line);
		return (0);
	}
	process_heredoc_line(data, line);
	free(line);
	(*line_count)++;
	return (1);
}

void	close_heredoc_fds(t_redirection *r)
{
	while (r)
	{
		if (r->type == TOKEN_HEREDOC && r->heredoc_fd != -1)
		{
			close(r->heredoc_fd);
			r->heredoc_fd = -1;
		}
		r = r->next;
	}
}
