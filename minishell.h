#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_env
{
	char *line;
	char *key;
	char *value;
	struct s_env *next;
}	t_env;

typedef struct s_command
{
    char *cmd;              // command name
    char **args;            // arguments (including cmd)
    char *infile;           // input redirection: < or <<
    char *outfile;          // output redirection: > or >>
    int append;             // 1 if >>
    int heredoc;            // 1 if <<
    struct s_command *next; // next command in pipeline
} t_command;

//parssing
void    builtins(t_env **env, char **args, char *prev_pwd);
int     is_builtins(char **args);


void execution(t_env *env, t_command *cmds);

//builtins
char	**ft_split(char *s, char c);
void	ft_echo(char **arg);
void	ft_pwd(t_env *env, char *prev_pwd);
void    ft_cd(t_env **env, char **arg);
void	ft_exit(t_env *env, char **arg);
void	ft_env(t_env **env);
void	ft_unset(t_env **env,char **arg);
void	ft_export(t_env **env, char **arg);

//builtins utils
void	copy_env(char **envp, t_env **env);
t_env	*copy_for_expo(t_env *env) ;
void	free_env(t_env *env);
int		is_valid_identifier(char *str);
char	*key_full(char *line, char c);
void     check_for_pwd(char **prev_pwd);

//utils
t_env	*ft_newnode(char *line, char c);
t_env	*ft_lstlast(t_env *env);
void	ft_lstadd_back(t_env **env, t_env *new);
char	*ft_strdup(char *src);
char	*ft_join(char *s1, char *s2);
int		ft_strlen(char *s);
int		ft_strcmp(char *s1, char *s2);
char	**free_array(char **array);
void ft_putstr_fd(char *s, int fd);

#endif