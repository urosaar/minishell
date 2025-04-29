#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
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


#endif