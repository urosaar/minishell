/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhallou <skhallou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:10:22 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/15 19:51:55 by skhallou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <stdint.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include "./libft/libft.h"

# define HEREDOC_MAX_LINES 1000

# define CTRLD "\033[A\033[2C"

int	g_status;

typedef struct s_malloc
{
	void			*ptr;
	struct s_malloc	*next;
}	t_malloc;

typedef enum e_type
{
	FREE,
	MALLOC
}	t_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	struct s_redirection	*next;
	bool					quoted;
	char					quote_char;
	int						heredoc_fd;
}	t_redirection;

typedef struct s_env
{
	char			*line;
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_heredoc_child
{
	int		write_fd;
	char	*delimiter;
	int		quoted;
	int		last_status;
	t_env	**env;
}	t_heredoc_child;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	char				*infile;
	char				*outfile;
	t_redirection		*redirections;
	int					append;
	int					heredoc;
	struct s_command	*next;
	int					heredoc_quoted;
}	t_command;

typedef struct s_exec
{
	pid_t			pid;
	pid_t			last_pid;
	int				status2;
	int				prev_fd;
	int				last_status;
	int				pipe_fd[2];
	int				f;
	char			*prev_pwd;
	struct termios	shell_termios;
}	t_exec;

typedef struct s_state
{
	char		*res;
	int			rlen;
	const char	*in;
	int			idx;
	int			last_status;
	t_env		**env;
	int			in_single;
	int			in_double;
}	t_state;

typedef struct s_cmd_exp
{
	char	*expanded_str;
	char	**tokens;
	int		token_count;
}	t_cmd_exp;

typedef struct s_empty
{
	char			*value;
	struct s_empty	*next;
}	t_empty;

typedef struct s_locals
{
	int		i;
	int		count;
	int		in_single;
	int		in_double;
	int		t;
	int		start;
	int		tokens;
	char	**res;
	char	**raw_tokens;
}	t_locals;

char		**lexer(const char *input);
int			count_tokens(const char *input);
char		*extract_operator(const char *input, int *i);
char		*extract_word(const char *input, int *i);
t_command	*parse_tokens(char **tokens);
int			check_syntax_errors(char *line, char **tokens);
int			check_consecutive_operators(char **tokens);
int			check_redirection_without_filename(char **tokens);
int			check_invalid_pipe_placement(char **tokens);
int			check_unclosed_quotes(const char *line);
char		**splice_tokens(char **tokens, int pos, char **pieces);
char		*strip_quotes(const char *str);
char		*get_input(void);
void		free_tokens(char **tokens);
void		free_commands(t_command *cmd);
void		execution(t_command *cmds, t_env **env, t_exec *exec);
void		handler(int signal);
int			ft_exit(t_env *env, char **arg, int flag);
int			ft_echo(char **arg);
int			ft_pwd(t_env *env);
int			ft_cd(t_env **env, char **arg, char *prev_pwd);
int			ft_env(t_env **env, char **arg);
int			ft_unset(t_env **env, char **arg);
int			ft_export(t_env **env, char **arg);
int			builtins(t_env **env, char **args, char *prev_pwd, int flag);
int			is_builtins(char **args);
void		copy_env(char **envp, t_env **env);
t_env		*copy_for_expo(t_env *env);
void		free_env(t_env *env);
int			is_valid_identifier(char *str);
char		*key_full(char *line, char c);
void		check_for_pwd(char **prev_pwd);
char		*append_char(char *result, int *rlen, char c);
char		**free_array(char **array);
char		*ft_getenv(const char *name, t_env *env);
void		*ft_malloc(size_t size, t_type type);
t_malloc	*new_node(void	*ptr);
t_malloc	*last_node(t_malloc **head);
void		add_back(t_malloc	**head, t_malloc *new);
void		free_all(t_malloc **head);
char		*expand_variables(const char *input, int last_status, t_env **env);
void		expand_command_vars(t_command *cmd, int last_status, t_env **env);
void		expand_tokens(char **tokens, int last_status, t_env **env);
void		save_shell_term(t_exec *exec);
void		restore_shell_term(t_exec *exec);
int			is_nbr(char *arg);
void		swap_line(t_env **head);
char		*key_full(char *line, char c);
void		remove_if(t_env *env);
void		without_plus(t_env **env, char *arg);
void		with_plus(t_env **env, char *arg);
char		**build_env_array(t_env **env);
int			handle_empty_cmd(t_command *curr, t_env **env, t_exec *ctx);
char		*check_if_exist(t_env *env, t_command *cmds, t_exec *ctx);
int			creat_a_child(t_command *curr, t_env **env, t_exec *ctx);
void		ft_wait(t_exec *ctx);
void		restore_std_fds(int saved_in, int saved_out);
void		close_fd(int fd);
void		ft_execve(t_command *curr, t_env **env, t_exec *ctx, char *d);
void		dup_if_there_is_pipe(t_command *curr, int *pipe_fd, int prev_fd);
int			process_all_heredocs(t_command *cmds, int last_status, t_env **env);
void		close_heredoc_fds(t_redirection *r);
void		handler_heredoc(int sig);
int			heredoc_iteration(t_heredoc_child *data, int *line_count);
int			apply_redirection(t_command *curr, t_env **env);
void		expand_wildcards(t_command *cmd, bool *no_expand);
bool		*create_no_split_map(char **args);
char		*expand_variables(const char *input, int last_status, t_env **env);
void		expand_tokens(char **tokens, int last_status, t_env **env);
char		**split_selected_args(char **args, bool *no_split);
bool		*create_no_split_map(char **args);
t_cmd_exp	expand_command_string(char *cmd_str, int last_status, t_env **env);
char		**expand_arguments(char **args, int arg_count,
				int last_status, t_env **env);
void		expand_redirections(t_command *cmd, int last_status, t_env **env);
void		expand_command_vars(t_command *cmd, int last_status, t_env **env);
void		free_strarray(char **arr);
int			count(char **args);
int			handle_exit_status(t_state *st);
int			handle_env_var(t_state *st);
void		rebuild_without_tokens(t_command *cmd, char **exp_args,
				bool *no_split, char *exp_cmd);
void		rebuild_with_tokens(t_command *cmd, t_cmd_exp *exp,
				char **exp_args, bool *no_split);
int			str_append_char(t_state *st, char c);
void		free_strarray(char **arr);
void		free_tokens(char **tokens);
void		free_commands(t_command *cmds);
void		handler_eof(t_command *cmds, t_env *env, int last_status);
void		handler(int signal);
void		signals(void);
t_exec		*init_exec_env(char **envp, t_env **env);
int			handle_args_error(int ac, char **av, t_exec *exec);
char		*read_and_preprocess(t_command *cmds, t_env *env, t_exec *exec);
char		**lex_and_validate(char *raw, t_exec *exec);
void		free_args(char **args);
void		free_redirections(t_redirection *redir);
void		free_command_partial(t_command *cmd);
t_command	*init_command(int argc);
int			setup_redir(t_redirection **redir, char *token);
int			process_redir(char **tokens, int *idx,
				t_command *cmd, t_redirection **last);
int			process_arg(char **tokens, int *idx, t_command *cmd, int *arg_i);
void		finalize_command(t_command *cmd, int arg_i);
t_command	*parse_one_command(char **tokens, int *idx);
int			process_and_link(char **tokens, int *i,
				t_command **head, t_command **current);
int			count_actual_args(char **tokens, int start);
bool		is_redir_token(const char *tok);
int			process_redir(char **tokens, int *idx,
				t_command *cmd, t_redirection **last);
int			count_total_heredocs(char **tokens);
int			skip_quotedd(const char *line, int *i, char q);
char		*extract_operator(const char *input, int *i);
bool		is_in_quote(bool in_quote, char *quote_char, char current_char);
int			count_words_isspace(const char *s);
int			handle_single_redirection(t_redirection *tmp, t_env **env);
int			apply_redirect(t_redirection *tmp, char *clean);
void		free_empty_list(t_empty *empty);
int			add_empty_back(t_empty **head, char *str);
char		**empty_list_to_argv(t_empty *head);
char		**empty_system(char **argv, t_exec *ctx);
bool		empties_inside(char *str);
bool		one_empty(char *str);
int			has_expandable(const char *str);
int			count_unquoted_tokens(const char *s);
char		**split_unquoted_tokens(const char *s);
void		check_args_and_exit(char **args, t_exec *ctx);

#endif