// #include "../minishell.h"

// int check_consecutive_operators(char **tokens)
// {
//     int i = 0;
//     while (tokens[i] && tokens[i + 1])
//     {
//         if (is_operator(tokens[i][0]) && is_operator(tokens[i + 1][0]))
//         {
//             printf("minishell: syntax error near unexpected token `%s'\n",
//                    tokens[i + 1]);
//             return (1);
//         }
//         i++;
//     }
//     return (0);
// }

// int check_redirection_without_filename(char **tokens)
// {
//     int i = 0;
//     while (tokens[i])
//     {
//         if ((tokens[i][0] == '>' || tokens[i][0] == '<')
//          && (!tokens[i + 1] || is_operator(tokens[i + 1][0])))
//         {
//             if (!tokens[i + 1])
//                 printf("minishell: syntax error near unexpected token `newline'\n");
//             else
//                 printf("minishell: syntax error near unexpected token `%s'\n",
//                        tokens[i + 1]);
//             return (1);
//         }
//         i++;
//     }
//     return (0);
// }

// int check_invalid_pipe_placement(char **tokens)
// {
//     int last = 0;

//     if (!tokens[0])
//         return (0);
//     if (tokens[0][0] == '|')
//     {
//         printf("minishell: syntax error near unexpected token `|'\n");
//         return (1);
//     }

//     while (tokens[last + 1])
//         last++;
//     if (tokens[last][0] == '|')
//     {
//         printf("minishell: syntax error near unexpected token `|'\n");
//         return (1);
//     }
//     return (0);
// }

// int check_unclosed_quotes(const char *line)
// {
//     int  i = 0;
//     char q;

//     while (line[i])
//     {
//         if (line[i] == '\'' || line[i] == '"')
//         {
//             q = line[i++];
//             while (line[i] && line[i] != q)
//                 i++;
//             if (line[i] != q)
//             {
//                 printf("minishell: syntax error: unexpected EOF while looking for matching `%c'\n", q);
//                 return (1);
//             }
//             i++;
//         }
//         else
//             i++;
//     }
//     return (0);
// }


// int check_syntax_errors(char *line, char **tokens)
// {
//     if (check_unclosed_quotes(line))
//         return (1);
//     if (check_consecutive_operators(tokens))
//         return (1);
//     if (check_redirection_without_filename(tokens))
//         return (1);
//     if (check_invalid_pipe_placement(tokens))
//         return (1);
//     return (0);
// }


#include "../minishell.h"

static int is_redir_token(const char *tok)
{
    return (!ft_strcmp(tok, "<")
         || !ft_strcmp(tok, "<<")
         || !ft_strcmp(tok, ">")
         || !ft_strcmp(tok, ">>"));
}

static int is_pipe_token(const char *tok)
{
    return (tok[0] == '|' && tok[1] == '\0');
}

int check_unclosed_quotes(const char *line)
{
    int  i = 0;
    char q;

    while (line[i])
    {
        if (line[i] == '\'' || line[i] == '"')
        {
            q = line[i++];
            while (line[i] && line[i] != q)
                i++;
            if (line[i] != q)
            {
                printf(
                  "minishell: syntax error: unexpected EOF while looking for matching `%c'\n",
                  q);
                return (1);
            }
            i++;
        }
        else
            i++;
    }
    return (0);
}


int check_consecutive_operators(char **tokens)
{
    int i = 0;

    while (tokens[i] && tokens[i + 1])
    {
        if ((is_pipe_token(tokens[i]) && is_pipe_token(tokens[i + 1]))
         || (is_redir_token(tokens[i]) && is_redir_token(tokens[i + 1])))
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
        if (is_redir_token(tokens[i]))
        {
            if (!tokens[i + 1] || is_pipe_token(tokens[i + 1]) || is_redir_token(tokens[i + 1]))
            {
                if (!tokens[i + 1])
                    printf(
                      "minishell: syntax error near unexpected token `newline'\n");
                else
                    printf(
                      "minishell: syntax error near unexpected token `%s'\n",
                      tokens[i + 1]);
                return (1);
            }
            i += 2; 
        }
        else
            i++;
    }
    return (0);
}

int check_invalid_pipe_placement(char **tokens)
{
    int last = 0;

    if (!tokens[0])
        return (0);
    if (is_pipe_token(tokens[0]))
    {
        printf("minishell: syntax error near unexpected token `|'\n");
        return (1);
    }
    while (tokens[last + 1])
        last++;
    if (is_pipe_token(tokens[last]))
    {
        printf("minishell: syntax error near unexpected token `|'\n");
        return (1);
    }
    return (0);
}


int check_syntax_errors(char *line, char **tokens)
{
    if (check_unclosed_quotes(line))
        return (1);
    if (check_consecutive_operators(tokens))
        return (1);
    if (check_redirection_without_filename(tokens))
        return (1);
    if (check_invalid_pipe_placement(tokens))
        return (1);
    return (0);
}
