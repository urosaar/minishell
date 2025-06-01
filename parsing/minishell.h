#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC
}   t_token_type;

typedef struct s_token {
    t_token_type      type;
    char             *value;
    struct s_token   *next;
}   t_token;

typedef struct s_redirection {
    int                  type;
    char                *filename;
    struct s_redirection *next;
}   t_redirection;

typedef struct s_command {
    char            *cmd;
    char            **args;
    char            *infile;
    char            *outfile;
    int              append;
    int              heredoc;
    struct s_command *next;
}   t_command;

/* Function prototypes */
/* Lexer */
char    **lexer(const char *input);
int     count_tokens(const char *input);
char    *extract_quoted(const char *input, int *i);
char    *extract_operator(const char *input, int *i);
char    *extract_word(const char *input, int *i);

/* Parser */
t_command   *parse_tokens(char **tokens);
int         count_args(char **tokens, int start);

/* Syntax Check */
int check_syntax_errors(char **tokens);
int check_consecutive_operators(char **tokens);
int check_redirection_without_filename(char **tokens);
int check_invalid_pipe_placement(char **tokens);

/* Utils */
size_t  ft_strlen(const char *str);
char    *ft_strdup(const char *src);
int     ft_strcmp(const char *s1, const char *s2);
int     is_space(char c);
int     is_operator(char c);
char    *substr(const char *src, int start, int end);
char    *expand_variables(const char *input);
char    *strip_quotes(const char *str);
char    *get_input(void);
int     is_only_whitespace(const char *input);

/* Memory */
void    free_tokens(char **tokens);
void    free_commands(t_command *cmd);

/* Debug */
void    print_commands(t_command *cmd);

#endif