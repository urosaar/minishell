/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:40:15 by jesse             #+#    #+#             */
/*   Updated: 2025/08/10 16:09:57 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*parse_tokens(char **tokens)
{
	t_command	*head;
	t_command	*current;
	int			i;

	head = NULL;
	current = NULL;
	i = 0;
	if (count_total_heredocs(tokens) > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n",
			STDERR_FILENO);
		return (NULL);
	}
	while (tokens[i])
	{
		if (process_and_link(tokens, &i, &head, &current))
			return (NULL);
	}
	return (head);
}

static void	copy_spliced_tokens(char **tokens, char **pieces,
				char **out, int pos)
{
	int	i;
	int	j;
	int	out_i;

	i = 0;
	j = 0;
	out_i = 0;
	while (i < pos)
		out[out_i++] = tokens[i++];
	while (pieces[j])
		out[out_i++] = pieces[j++];
	i = pos + 1;
	while (tokens[i])
		out[out_i++] = tokens[i++];
	out[out_i] = NULL;
}

char	**splice_tokens(char **tokens, int pos, char **pieces)
{
	int		old_n;
	int		new_n;
	char	**out;

	old_n = 0;
	while (tokens[old_n])
		old_n++;
	new_n = 0;
	while (pieces[new_n])
		new_n++;
	out = malloc(sizeof(char *) * (old_n - 1 + new_n + 1));
	if (!out)
		return (NULL);
	copy_spliced_tokens(tokens, pieces, out, pos);
	free(tokens);
	return (out);
}
