#include <stdlib.h>
#include <unistd.h>

/*i tryed to work with lixing because it will make things easy 
It splits a raw string input into pieces like commands, arguments, pipes, redirects, etc 
it is like a improved version of ft_split */

int is_space(char c) 
{
	return (c == ' ' || c == '\t' || c == '\n');// this for skiping space tabs o newlines
}

int is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>'); // this for found operators for spliting
}

char *substr(const char *src, int start, int end) // this is used to extract a token from input
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

char *extract_quoted(const char *input, int *i) // this for treated words inside quotes (' or ") as one argument 
{
	int start = ++(*i);
	char quote = input[start - 1];
	while (input[*i] && input[*i] != quote)
		(*i)++;
	char *token = substr(input, start, *i);
	if (input[*i] == quote)
        (*i)++;
	return (token);
}

char *extract_operator(const char *input, int *i) // this for Extracts one or two-character operator tokens like |, >, <, >>, <<
{
	if ((input[*i] == '<' && input[*i + 1] == '<') || (input[*i] == '>' && input[*i + 1] == '>')) 
    {
		*i += 2;
		return (substr(input, *i - 2, *i));
	}
	(*i)++;
	return (substr(input, *i - 1, *i));
}

char *extract_word(const char *input, int *i) /*  this used for Extracts a regular word token (not quoted, not an operator)
                                                      Stops at spaces or operators. Used for command names, arguments, filenames*/
{
	int start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
		(*i)++;
	return (substr(input, start, *i));
}

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
/* this is The main lexing function :

Uses the helpers to loop through the input string
Skips spaces
Extracts each token (quoted string, operator, or word)
Stores them in a char **tokens array
Returns that array, which ends in NULL

*/
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
			tokens[j++] = extract_quoted(input, &i);
		else if (is_operator(input[i]))
			tokens[j++] = extract_operator(input, &i);
		else
			tokens[j++] = extract_word(input, &i);
	}
	tokens[j] = NULL;
	return (tokens);
}
// testing lexer try it to see how things works :)
#include <stdio.h>
int main(void)
{
	char *input = "echo \"hello world\" > file.txt | grep '$USER'";
	char **tokens = lexer(input);
	int i = 0;
	while (tokens[i])
	{
		printf("token[%d] = [%s]\n", i, tokens[i]);
		i++;
	}
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
