#include "minishell.h"

int check_consecutive_operators(char **tokens)
{
    int i = 0;
    while (tokens[i] && tokens[i + 1])
    {
        if (is_operator(tokens[i][0]) && is_operator(tokens[i + 1][0]))
        {
            printf("minishell: syntax error near unexpected token `%s'\n",
                   tokens[i + 1]);
            return (1);
        }
        i++;
    }
    return (0);
}

int check_redirection_without_filename(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        if ((tokens[i][0] == '>' || tokens[i][0] == '<')
         && (!tokens[i + 1] || is_operator(tokens[i + 1][0])))
        {
            if (!tokens[i + 1])
                printf("minishell: syntax error near unexpected token `newline'\n");
            else
                printf("minishell: syntax error near unexpected token `%s'\n",
                       tokens[i + 1]);
            return (1);
        }
        i++;
    }
    return (0);
}

int check_invalid_pipe_placement(char **tokens)
{
    int last = 0;

    if (!tokens[0])
        return (0);
    if (tokens[0][0] == '|')
    {
        printf("minishell: syntax error near unexpected token `|'\n");
        return (1);
    }

    while (tokens[last + 1])
        last++;
    if (tokens[last][0] == '|')
    {
        printf("minishell: syntax error near unexpected token `|'\n");
        return (1);
    }
    return (0);
}

int check_syntax_errors(char **tokens)
{
    return (check_consecutive_operators(tokens)
         || check_redirection_without_filename(tokens)
         || check_invalid_pipe_placement(tokens));
}
