#include "../minishell.h"

char *extract_quoted(const char *input, int *i)
{
    char quote = input[*i];
    int start = *i;
    (*i)++;
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
    {
        printf("minishell: syntax error: unterminated quoted string\n");
        return (NULL);
    }
    (*i)++;
    return (substr(input, start, *i));
}

char *extract_operator(const char *input, int *i)
{
    if ((input[*i] == '<' && input[*i + 1] == '<')
     || (input[*i] == '>' && input[*i + 1] == '>'))
    {
        int start = *i;
        *i += 2;
        return (substr(input, start, *i));
    }
    return (substr(input, (*i)++, *i));
}

char *extract_word(const char *input, int *i)
{
    int start = *i;
    int in_quote = 0;
    char quote_char = 0;

    while (input[*i])
    {
        if (!in_quote && input[*i] == '\\' && input[*i + 1])
            *i += 2;
        else if (!in_quote && (input[*i] == '\'' || input[*i] == '"'))
        {
            in_quote = 1;
            quote_char = input[*i];
            (*i)++;
        }
        else if (in_quote && input[*i] == quote_char)
        {
            in_quote = 0;
            (*i)++;
        }
        else if (!in_quote && (is_space(input[*i]) || is_operator(input[*i])))
        {
            break;
        }
        else
        {
            (*i)++;
        }
    }
    return (substr(input, start, *i));
}

int count_tokens(const char *input)
{
    int count = 0;
    int i = 0;

    while (input[i])
    {
        while (is_space(input[i]))
            i++;
        if (!input[i])
            break;

        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (input[i])
                i++;
        }
        else if (is_operator(input[i]))
        {
            i++;
            if ((input[i - 1] == '<' && input[i] == '<')
             || (input[i - 1] == '>' && input[i] == '>'))
                i++;
        }
        else
        {
            while (input[i] && !is_space(input[i]) && !is_operator(input[i]))
                i++;
        }
        count++;
    }
    return (count);
}

char **lexer(const char *input)
{
    int     i = 0, j = 0;
    int     tok_count = count_tokens(input);
    char   **tokens = malloc(sizeof(char *) * (tok_count + 1));

    if (!tokens)
        return (NULL);

    while (input[i])
    {
        while (is_space(input[i]))
            i++;
        if (!input[i])
            break;

        if (input[i] == '\'' || input[i] == '"')
        {
            tokens[j] = extract_quoted(input, &i);
            if (!tokens[j])
            {
                while (j > 0)
                    free(tokens[--j]);
                free(tokens);
                return (NULL);
            }
            j++;
        }
        else if (is_operator(input[i]))
        {
            tokens[j++] = extract_operator(input, &i);
        }
        else
        {
            tokens[j++] = extract_word(input, &i);
        }
    }
    tokens[j] = NULL;
    return (tokens);
}
