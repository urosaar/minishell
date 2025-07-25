#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#define HEREDOC_MAX_LINES 1000

#define CTRLD "\033[A\033[2C"

extern int g_status;

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
}   t_token_type;

typedef struct s_token
{
	t_token_type      type;
	char             *value;
	struct s_token   *next;
}   t_token;

typedef struct s_redirection
{
	int                  type;
	char                *filename;
	struct s_redirection *next;
	int                   heredoc_fd;
}   t_redirection;

typedef struct s_env
{
	char *line;
	char *key;
	char *value;
	struct s_env *next;
}	t_env;

typedef struct s_command {
	char            *cmd;       // Command name (e.g. "ls")
	char            **args;     // NULL-terminated array of arguments
	char            *infile;    // Filename for '<' or heredoc delimiter for '<<'
	char            *outfile;   // Filename for '>' or '>>'
	t_redirection   *redirections;  //  Linked list of redirections (if you later want to support multiple redirections)
	int              append;    // 1 if “>>”, 0 if “>”
	int              heredoc;   // 1 if “<<”
	struct s_command *next;     // Next command in a pipeline
	int 			heredoc_quoted;
	// char *heredoc_delimiter;
}   t_command;

typedef struct s_exec
{   
	pid_t 	pid;
	pid_t 	last_pid;
	int		status2;
	int		prev_fd;
    int		last_status;
	int		pipe_fd[2];
    char	*prev_pwd;
} t_exec;

/* Function prototypes */
/* Lexer */
char    **lexer(const char *input);
int     count_tokens(const char *input);
// char    *extract_quoted(const char *input, int *i);
char    *extract_operator(const char *input, int *i);
char    *extract_word(const char *input, int *i);

/* Parser */
t_command   *parse_tokens(char **tokens);
int         count_args(char **tokens, int start);

/* Syntax Check */
int check_syntax_errors(char *line,char **tokens);
int check_consecutive_operators(char **tokens);
int check_redirection_without_filename(char **tokens);
int check_invalid_pipe_placement(char **tokens);
int check_unclosed_quotes(const char *line);

/* Utils */
size_t  ft_strlen(const char *str);
char	**splice_tokens(char **tokens, int pos, char **pieces);
char	**split_all_args(char **args);
// char    *ft_strdup(const char *src);
int     ft_strcmp(const char *s1, const char *s2);
int     is_space(char c);
int     is_operator(char c);
char	*substr(const char *src, int start, int end);
char    *strip_quotes(const char *str);
char    *get_input(void);
int     is_only_whitespace(const char *input);
void	*ft_calloc(size_t count, size_t size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
/* Memory */
void    free_tokens(char **tokens);
void    free_commands(t_command *cmd);

/* Debug */
void    print_commands(t_command *cmd);

/* Execution */
void execution(t_command *cmds, t_env **env, t_exec *exec);

/*Signals*/
void	handler(int signal);

/* Builtins */
char	**ft_split(char *s, char c);
void	ft_exit(t_env *env, char **arg);
int		ft_echo(char **arg);
int		ft_pwd(t_env *env);
int		ft_cd(t_env **env, char **arg, char *prev_pwd);
int		ft_env(t_env **env, char **arg);
int		ft_unset(t_env **env,char **arg);
int		ft_export(t_env **env, char **arg);
int		builtins(t_env **env, char **args, char *prev_pwd);
int		is_builtins(char **args);

/* Builtins utils */
void	copy_env(char **envp, t_env **env);
t_env	*copy_for_expo(t_env *env) ;
void	free_env(t_env *env);
int		is_valid_identifier(char *str);
char	*key_full(char *line, char c);
void     check_for_pwd(char **prev_pwd);

/* Utils */
t_env	*ft_newnode(char *line, char c);
t_env	*ft_lstlast(t_env *env);
void	ft_lstadd_back(t_env **env, t_env *new);
char	*ft_join(char *s1, char *s2);
char	*ft_strdup(char *src);
char *append_char(char *result, int *rlen, char c);
char	*ft_itoa(int n);
char	*ft_getenv(const char *name, t_env *env);
// int		ft_strlen(char *s);
// int		ft_strcmp(char *s1, char *s2);
char	**free_array(char **array);
void ft_putstr_fd(char *s, int fd);

/* Malloc */
void	*ft_malloc(size_t size, t_type type);
t_malloc	*new_node(void	*ptr);
t_malloc	*last_node(t_malloc **head);
void	add_back(t_malloc	**head, t_malloc *new);
void	free_all(t_malloc **head);

char    *expand_variables(const char *input, int last_status, t_env **env);
void	expand_command_vars(t_command *cmd, int last_status, t_env **env);
void expand_tokens(char **tokens, int last_status, t_env **env);
char **split_all_args(char **args);

#endif
