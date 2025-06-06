#include "../minishell.h"

size_t ft_strlen(const char *str)
{
    size_t i = 0;
    while (str[i])
        i++;
    return (i);
}

// char *ft_strdup(const char *src)
// {
//     size_t len = ft_strlen(src);
//     char *dup = malloc(len + 1);
//     if (!dup)
//         return (NULL);

//     size_t i = 0;
//     while (i <= len)
//     {
//         dup[i] = src[i];
//         i++;
//     }
//     return (dup);
// }


int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

char *substr(const char *src, int start, int end)
{
    int len = end - start;
    char *s = malloc(len + 1);
    int i = 0;
    if (!s)
        return (NULL);
    while (i < len)
    {
        s[i] = src[start + i];
        i++;
    }
    s[len] = '\0';
    return (s);
}
int is_only_whitespace(const char *input)
{
    int i = 0;
    while (input[i])
    {
        if (!is_space(input[i]))
            return (0);
        i++;
    }
    return (1);
}
void *ft_calloc(size_t count, size_t size)
{
    void *ptr = malloc(count * size);
    if (!ptr)
        return (NULL);
    size_t total_size = count * size;
    size_t i = 0;
    while (i < total_size)
    {
        ((unsigned char *)ptr)[i] = 0;
        i++;
    }
    return (ptr);
}
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dst && !src)
		return (NULL);
	while (i < n && dst != src)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}
