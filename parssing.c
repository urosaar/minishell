#include "minishell.h"

/*i tryed to work with lixing because it will make things easy 
It splits a raw string input into pieces like commands, arguments, pipes, redirects, etc 
it is like a improved version of ft_split */

/* Helper function to check if a character is whitespace */
int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

/* Helper function to check if a character is a shell operator */
int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

/* Creates a substring from src between start and end indices */
char *substr(const char *src, int start, int end)
{
    int len = end - start;
    char *s = malloc(len + 1);
    if (!s)
        return NULL;
    int i = 0;
    while (i < len)
    {
        s[i] = src[start + i];
        i++;
    }
    s[len] = '\0';
    return (s);
}

/* Extracts content between quotes (single or double) */
char *extract_quoted(const char *input, int *i)
{
    char quote = input[(*i)++];
    int start = *i;
    
    while (input[*i] && input[*i] != quote)
        (*i)++;
        
    if (input[*i] != quote) {
        printf("minishell: syntax error: unterminated quoted string\n");
        return NULL;
    }
    
    char *token = substr(input, start, *i);
    (*i)++;
    return token;
}

/* Extracts operators (can be 1 or 2 chars like >, >>, <, <<) */
char *extract_operator(const char *input, int *i)
{
    if ((input[*i] == '<' && input[*i + 1] == '<') ||
        (input[*i] == '>' && input[*i + 1] == '>'))
    {
        int start = *i;
        *i += 2;
        return substr(input, start, *i);
    }
    return substr(input, (*i)++, *i);
}

/* Extracts regular words (not quoted, not operators) */
char *extract_word(const char *input, int *i)
{
    int start = *i;
    while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
        (*i)++;
    return substr(input, start, *i);
}

/* Counts how many tokens exist in the input string */
int count_tokens(const char *input)  // this is used for count the number of tokens in an input string
{
	int count = 0, i = 0;
	while (input[i])
    {
		while (is_space(input[i])) i++;
		if (!input[i])
            break;
		if (input[i] == '\'' || input[i] == '"')
        {
			i++;
			while (input[i] && input[i] != input[i - 1])
                i++;
			if (input[i])
                i++;
		}
        else if (is_operator(input[i]))
        {
			i++;
			if ((input[i - 1] == '<' && input[i] == '<') || (input[i - 1] == '>' && input[i] == '>'))
				i++;
		}
        else
        {
			while (input[i] && !is_space(input[i]) && !is_operator(input[i]))
				i++;
		}
		count++;
	}
	return count;
}

/* Main lexer function - converts input string into tokens */
char **lexer(const char *input)
{
    int i = 0;
    int j = 0;
    int count = count_tokens(input);
    char **tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens)
        return NULL;

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
                return NULL;
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
    return tokens;
}

/* Checks for consecutive operators */
int check_consecutive_operators(char **tokens)
{
    int i = 0;
    while (tokens[i] && tokens[i + 1])
    {
        if (is_operator(tokens[i][0]) && is_operator(tokens[i + 1][0]))
        {
            printf("minishell: syntax error near unexpected token `%s'\n", tokens[i + 1]);
            return 1;
        }
        i++;
    }
    return 0;
}

/* Checks for redirections without filenames */
int check_redirection_without_filename(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        if ((tokens[i][0] == '>' || tokens[i][0] == '<') &&
            (!tokens[i + 1] || is_operator(tokens[i + 1][0])))
        {
            if (!tokens[i + 1])
                printf("minishell: syntax error near unexpected token `newline'\n");
            else
                printf("minishell: syntax error near unexpected token `%s'\n", tokens[i + 1]);
            return 1;
        }
        i++;
    }
    return 0;
}

/* Checks for pipes at start/end */
int check_invalid_pipe_placement(char **tokens)
{
    if (!tokens[0]) return 0;
    
    if (tokens[0][0] == '|') {
        printf("minishell: syntax error near unexpected token `|'\n");
        return 1;
    }
    
    int last = 0;
    while (tokens[last + 1]) last++;
    if (tokens[last][0] == '|') {
        printf("minishell: syntax error near unexpected token `|'\n");
        return 1;
    }
    
    return 0;
}

/* Combined syntax checking */
int check_syntax_errors(char **tokens)
{
    return check_consecutive_operators(tokens) ||
           check_redirection_without_filename(tokens) ||
           check_invalid_pipe_placement(tokens);
}

/* Checks if input contains only whitespace */
int is_only_whitespace(const char *input)
{
    int i = 0;
    while (input[i])
    {
        if (!is_space(input[i]))
            return 0;
        i++;
    }
    return 1;
}
// this for geting input from user and showing old commandes
char *get_input(void) 
{
    char *input = readline("minishell> ");
    if (input && *input)
        add_history(input);
    return input;
}

   /*                
     --> int main for TESTING input && lexing && syntax error <-- 
	                                                              */

int main(void)
{
    char *input;
    char **tokens;
    int i;

    while (1)
    {
        input = get_input();
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (is_only_whitespace(input))
        {
            free(input);
            continue;
        }
        tokens = lexer(input);
        if (!tokens)
        {
            free(input);
            continue;
        }
        if (!check_syntax_errors(tokens))
        {
            i = 0;
            while (tokens[i])
			{
                printf("token[%d]: %s\n", i, tokens[i]);
				i++;
			}
		}
        i = 0;
        while (tokens[i])
            free(tokens[i++]);
        free(tokens);
        free(input);
    }

    return 0;
}
