/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:40:15 by jesse             #+#    #+#             */
/*   Updated: 2025/08/03 17:10:50 by jesse            ###   ########.fr       */
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

// static t_command *parse_one_command(char **tokens, int *idx)
// {
//     t_command    *cmd = ft_calloc(1, sizeof(t_command));
//     int           argc;
//     int           arg_i = 0;
//     t_redirection *last_redir = NULL;

//     if (!cmd)
//         return NULL;

//     argc = count_actual_args(tokens, *idx);
//     cmd->args = malloc(sizeof(char *) * (argc + 1));
//     if (!cmd->args)
//     {
//         free_command_partial(cmd);
//         return NULL;
//     }
//      while (tokens[*idx] && tokens[*idx][0] != '|')
//     {
//         if (is_redir_token(tokens[*idx]))
//         {
//             t_redirection *redir = ft_calloc(1, sizeof(t_redirection));
//             if (!redir)
//             {
//                 free_command_partial(cmd);
//                 return NULL;
//             }
//             if (ft_strcmp(tokens[*idx], "<") == 0)
//                 redir->type = TOKEN_REDIRECT_IN;
//             else if (ft_strcmp(tokens[*idx], "<<") == 0)
//                 redir->type = TOKEN_HEREDOC;
//             else if (ft_strcmp(tokens[*idx], ">") == 0)
//                 redir->type = TOKEN_REDIRECT_OUT;
//             else 
//                 redir->type = TOKEN_REDIRECT_APPEND;

//             (*idx)++;
//             if (!tokens[*idx]) {
//                 free(redir);
//                 free_command_partial(cmd);
//                 return NULL;
//             }

//            if (redir->type == TOKEN_HEREDOC)
//             {
//                 char *stripped = strip_quotes(tokens[*idx]);
//                 if (!stripped)
//                 {
//                     free(redir);
//                     free_command_partial(cmd);
//                     return NULL;
//                 }
//                 redir->filename = stripped;
//                 cmd->heredoc = 1;
//                 if (tokens[*idx][0] == '\'' || tokens[*idx][0] == '"')
//                     cmd->heredoc_quoted = 1;
//                 else
//                     cmd->heredoc_quoted = 0;
//             }

//             else
//             {
//                 char *filename = strip_quotes(tokens[*idx]);
//             if (!filename)
//             {
//                 free(redir);
//                 free_command_partial(cmd);
//                 return NULL;
//             }
//                 redir->filename = filename;
//                 if (redir->type == TOKEN_REDIRECT_IN)
//                     cmd->infile = ft_strdup(filename);
//                 else if (redir->type == TOKEN_REDIRECT_OUT)
//                 {
//                     cmd->outfile = ft_strdup(filename);
//                     cmd->append = 0;
//                 }
//                 else 
//                 {
//                     cmd->outfile = ft_strdup(filename);
//                     cmd->append = 1;
//                 }
//             }

//             if (!cmd->redirections)
//                 cmd->redirections = redir;
//             else
//                 last_redir->next = redir;
//             last_redir = redir;
//             (*idx)++;
//         }
//         else
//         {
//             char *arg = ft_strdup(tokens[*idx]);
//             if (!arg)
//             {
//                 free_command_partial(cmd);
//                 return NULL;
//             }
//             // cmd->args[arg_i++] = strip_quotes(tokens[*idx]);
//             cmd->args[arg_i++] = arg;
//             (*idx)++;
//         }
//     }
//     cmd->args[arg_i] = NULL;
//     if (cmd->args[0])
//         cmd->cmd = ft_strdup(cmd->args[0]);

//     return cmd;
// }