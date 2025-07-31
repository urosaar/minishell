/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesse <jesse@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:40:15 by jesse             #+#    #+#             */
/*   Updated: 2025/07/31 18:06:11 by jesse            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_redir_token(const char *tok)
{
	return (ft_strcmp(tok, "<") == 0
		|| ft_strcmp(tok, "<<") == 0
		|| ft_strcmp(tok, ">") == 0
		|| ft_strcmp(tok, ">>") == 0);
}

static int	count_actual_args(char **tokens, int start)
{
	int	count;
	int	j;

	count = 0;
	j = start;
	while (tokens[j] && tokens[j][0] != '|')
	{
		if (is_redir_token(tokens[j]))
			j += 2;
		else
		{
			count++;
			j++;
		}
	}
	return (count);
}

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_redirections(t_redirection *redir)
{
	t_redirection	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

static void	free_command_partial(t_command *cmd)
{
	if (!cmd)
		return ;
	free_args(cmd->args);
	free_redirections(cmd->redirections);
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd);
}

static t_command	*init_command(int argc)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

static int	handle_heredoc(t_redirection *redir, char *token, t_command *cmd)
{
	char	*stripped;

	stripped = strip_quotes(token);
	if (!stripped)
		return (1);
	redir->filename = stripped;
	cmd->heredoc = 1;
	if (strpbrk(token, "\"'") != NULL)
		cmd->heredoc_quoted = 1;
	else
		cmd->heredoc_quoted = 0;
	return (0);
}

static int	handle_redir(t_redirection *redir, char *token, t_command *cmd)
{
	char	*filename;

	filename = strip_quotes(token);
	if (!filename)
		return (1);
	redir->filename = filename;
	if (redir->type == TOKEN_REDIRECT_IN)
		cmd->infile = ft_strdup(filename);
	else if (redir->type == TOKEN_REDIRECT_OUT)
	{
		cmd->outfile = ft_strdup(filename);
		cmd->append = 0;
	}
	else
	{
		cmd->outfile = ft_strdup(filename);
		cmd->append = 1;
	}
	return (0);
}

static int	setup_redir(t_redirection **redir, char *token)
{
	*redir = ft_calloc(1, sizeof(t_redirection));
	if (!*redir)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		(*redir)->type = TOKEN_REDIRECT_IN;
	else if (ft_strcmp(token, "<<") == 0)
		(*redir)->type = TOKEN_HEREDOC;
	else if (ft_strcmp(token, ">") == 0)
		(*redir)->type = TOKEN_REDIRECT_OUT;
	else
		(*redir)->type = TOKEN_REDIRECT_APPEND;
	return (0);
}

static int	process_redir(char **tokens, int *idx,
	t_command *cmd, t_redirection **last)
{
	t_redirection	*redir;
	int				err;

	if (setup_redir(&redir, tokens[*idx]))
		return (1);
	(*idx)++;
	if (!tokens[*idx])
		return (free(redir), 1);
	if (redir->type == TOKEN_HEREDOC)
		err = handle_heredoc(redir, tokens[*idx], cmd);
	else
		err = handle_redir(redir, tokens[*idx], cmd);
	if (err)
		return (free(redir), 1);
	if (!cmd->redirections)
		cmd->redirections = redir;
	else
		(*last)->next = redir;
	*last = redir;
	(*idx)++;
	return (0);
}

static int	process_arg(char **tokens, int *idx, t_command *cmd, int *arg_i)
{
	char	*arg;

	arg = ft_strdup(tokens[*idx]);
	if (!arg)
		return (1);
	cmd->args[(*arg_i)++] = arg;
	(*idx)++;
	return (0);
}

static void	finalize_command(t_command *cmd, int arg_i)
{
	cmd->args[arg_i] = NULL;
	if (cmd->args[0])
		cmd->cmd = ft_strdup(cmd->args[0]);
}

t_command	*parse_one_command(char **tokens, int *idx)
{
	t_command		*cmd;
	int				argc;
	int				arg_i;
	t_redirection	*last_redir;

	argc = count_actual_args(tokens, *idx);
	cmd = init_command(argc);
	if (!cmd)
		return (NULL);
	arg_i = 0;
	last_redir = NULL;
	while (tokens[*idx] && tokens[*idx][0] != '|')
	{
		if (is_redir_token(tokens[*idx]))
		{
			if (process_redir(tokens, idx, cmd, &last_redir))
				return (free_command_partial(cmd), NULL);
		}
		else if (process_arg(tokens, idx, cmd, &arg_i))
			return (free_command_partial(cmd), NULL);
	}
	finalize_command(cmd, arg_i);
	return (cmd);
}

static int	count_total_heredocs(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "<<") == 0)
			count++;
		i++;
	}
	return (count);
}

static int	process_and_link(char **tokens, int *i,
	t_command **head, t_command **current)
{
	t_command	*cmd;

	cmd = parse_one_command(tokens, i);
	if (!cmd)
	{
		free_commands(*head);
		return (1);
	}
	if (!*head)
		*head = cmd;
	else
		(*current)->next = cmd;
	*current = cmd;
	if (tokens[*i] && tokens[*i][0] == '|')
		(*i)++;
	return (0);
}

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