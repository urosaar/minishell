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

/* Extracts a quoted token *including* its delimiters.
   On input "... 'foo bar' baz", calling this when i points at the single quote
   will return the string "'foo bar'" (with both quotes), advancing *i past it. */
char *extract_quoted(const char *input, int *i)
{
    char quote = input[*i];    // either ' or "
    int start = *i;            // include this in the result
    (*i)++;                    // skip opening delimiter

    // find the matching closing quote
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
    {
        printf("minishell: syntax error: unterminated quoted string\n");
        return NULL;
    }
    (*i)++;  // include the closing delimiter

    // return substring [start, *i)
    return substr(input, start, *i);
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
int count_tokens(const char *input)
{
	int count = 0;
    int i = 0;
	while (input[i])
    {
		while (is_space(input[i])) i++;
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

char    *ft_strdup(char *src)
{
    char    *dest;
    int        i;

    i = 0;
    while (src[i])
        i++;
    dest = (char *)malloc(sizeof(char) * (i + 1));
    i = 0;
    while (src[i])
    {
       dest[i] = src[i];
       i++;
    }
    dest[i] = '\0';
    return (dest);
}
int ft_strcmp(const char *s1, const char *s2)
{

    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

size_t ft_strlen(const char *str)
{
    size_t i = 0;
    while(str[i])
    {
        i++;
    }
    return(i);
}
char *expand_variables(const char *input)
{
    char *result = malloc(1);
    if (!result) return NULL;
    int res_len = 0;
    int i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    result[0] = '\0';

    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            char *new_result = malloc(res_len + 2);
            if (!new_result)
            { 
                free(result);
                return NULL;
            }
            int j = 0;
            while (j < res_len)
            {
                new_result[j] = result[j];
                j++;
            }
            new_result[res_len++] = input[i++];
            new_result[res_len] = '\0';
            free(result);
            result = new_result;
            continue;
        }
        else if (input[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            char *new_result = malloc(res_len + 2);
            if (!new_result)
            {
                free(result);
                return NULL;
            }
            int j = 0;
            while (j < res_len)
            {
                new_result[j] = result[j];
                j++;
            }
            new_result[res_len++] = input[i++];
            new_result[res_len] = '\0';
            free(result);
            result = new_result;
            continue;
        }
        else if (input[i] == '$' && !in_single_quote)
        {
            i++;
            int var_start = i;
            while ((input[i] >= 'A' && input[i] <= 'Z') ||
                   (input[i] >= 'a' && input[i] <= 'z') ||
                   (input[i] >= '0' && input[i] <= '9') ||
                   input[i] == '_')
                i++;
            int var_len = i - var_start;
            char *var_name = substr(input, var_start, var_start + var_len);
            char *env_val = getenv(var_name);
            free(var_name);
            if (!env_val)
                env_val = "";
            size_t env_len = ft_strlen(env_val);
            char *new_result = malloc(res_len + env_len + 1);
            if (!new_result)
            {
                free(result);
                return NULL;
            }
            int j = 0;
            while (j < res_len)
            {
                new_result[j] = result[j];
                j++;
            }
            size_t k = 0;
            while (k < env_len)
            {
                new_result[res_len + k] = env_val[k];
                k++;
            }
            res_len += env_len;
            new_result[res_len] = '\0';
            free(result);
            result = new_result;
        }
        else
        {
            char *new_result = malloc(res_len + 2);
            if (!new_result)
            {
                free(result);
                return NULL;
            }
            int j = 0;
            while (j < res_len)
            {
                new_result[j] = result[j];
                j++;
            }
            new_result[res_len++] = input[i++];
            new_result[res_len] = '\0';
            free(result);
            result = new_result;
        }
    }

    return result;
}
/*  Counts the number of arguments in the `tokens` array starting from the `start` index
 stopping at operators ('|', '<', '>') or the end of the array */
int count_args(char **tokens, int start)
{
    int count = 0;
    while (tokens[start] && tokens[start][0] != '|'
        && tokens[start][0] != '<' && tokens[start][0] != '>')
    {
        count++;
        start++;
    }
    return count;
}
/*Converts the `tokens` array into a linked list of `t_command` structs 
 where each command holds arguments, input/output redirections, and possible pipe connections*/

/* Parses tokens into t_command structs, expanding variables,
   then stripping outer quotes so execution sees unquoted text. */
t_command *parse_tokens(char **tokens)
{
    int i = 0;
    t_command *head = NULL, *current = NULL;

    while (tokens[i])
    {
        t_command *node = calloc(1, sizeof(t_command));
        int argc = count_args(tokens, i);
        node->args = malloc(sizeof(char*) * (argc + 1));
        int j = 0;
        while (tokens[i] &&
               tokens[i][0] != '<' &&
               tokens[i][0] != '>' &&
               tokens[i][0] != '|')
        {
            char *expanded = expand_variables(tokens[i]);
            int len = ft_strlen(expanded);
            if (len >= 2 && 
               ((expanded[0] == '\'' && expanded[len-1] == '\'') ||
                (expanded[0] == '"'  && expanded[len-1] == '"')))
            {
                char *stripped = substr(expanded, 1, len-1);
                free(expanded);
                expanded = stripped;
            }

            node->args[j++] = expanded;
            i++;
        }
        node->args[j] = NULL;
        node->cmd = node->args[0];
        while (tokens[i] && tokens[i][0] != '|')
        {
            if (!ft_strcmp(tokens[i], "<"))
                node->infile = ft_strdup(tokens[++i]);
            else if (!ft_strcmp(tokens[i], "<<"))
            {
                node->infile = ft_strdup(tokens[++i]);
                node->heredoc = 1;
            }
            else if (!ft_strcmp(tokens[i], ">"))
                node->outfile = ft_strdup(tokens[++i]);
            else if (!ft_strcmp(tokens[i], ">>"))
            {
                node->outfile = ft_strdup(tokens[++i]);
                node->append = 1;
            }
            i++;
        }
        if (tokens[i] && !ft_strcmp(tokens[i], "|"))
            i++;
        if (!head)
            head = node;
        else
            current->next = node;
        current = node;
    }

    return head;
}

/*this just for test
 Iterates through a linked list of `t_command` structs and prints their information, 
including the command, its arguments, input/output redirections, and whether the input 
 redirection is a heredoc or normal file */

void print_commands(t_command *cmd)
{
    int cmd_num = 0;
    while (cmd)
    {
        printf("------------- Command #%d -------------\n", cmd_num++);
        printf("cmd: %s\n", cmd->cmd);
        for (int i = 0; cmd->args[i]; i++)
            printf("  arg[%d]: %s\n", i, cmd->args[i]);

        if (cmd->infile)
        {
            if (cmd->heredoc)
                printf("  infile: %s (heredoc)\n", cmd->infile);
            else
                printf("  infile: %s (normal)\n", cmd->infile);
        }

        if (cmd->outfile)
        {
            if (cmd->append)
                printf("  outfile: %s (append)\n", cmd->outfile);
            else
                printf("  outfile: %s (truncate)\n", cmd->outfile);
        }

        cmd = cmd->next;
    }
}
// Frees the memory allocated for the array of `tokens` and each of its elements 

void free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i])
        free(tokens[i++]);
    free(tokens);
}
/* Frees the memory allocated for the linked list of `t_command` structs, including 
 their arguments, input/output redirection strings, and the command struct itself */

void free_commands(t_command *cmd)
{
    t_command *next;
    int i;

    while (cmd)
    {
        next = cmd->next;
        i = 0;
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        free(cmd);

        cmd = next;
    }
}
   /*                
     --> int main for TESTING input && lexing && syntax error && parsing  <-- 
	                                                              */
int main(void)
{
    char *input;
    char **tokens;
    t_command *cmds;

    while (1)
    {
        input = get_input();
        if (!input)
            break;
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

        if (check_syntax_errors(tokens))
        {
            free_tokens(tokens);
            free(input);
            continue;
        }

        cmds = parse_tokens(tokens);
        if (cmds)
        {
            print_commands(cmds);
            free_commands(cmds);
        }

        free_tokens(tokens); 
        free(input);
    }

    return 0;
}

