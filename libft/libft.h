/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oukhanfa <oukhanfa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:08:52 by oukhanfa          #+#    #+#             */
/*   Updated: 2025/08/16 06:11:36 by oukhanfa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include "../minishell.h"

typedef struct s_env	t_env;

size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *src);
int		ft_strcmp(const char *s1, const char *s2);
int		is_space(char c);
int		is_operator(char c);
char	*ft_substr(const char *src, int start, int end);
int		is_only_whitespace(const char *input);
void	*ft_calloc(size_t count, size_t size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_itoa(int n);
char	**ft_split(char *s, char c);
t_env	*ft_newnode(char *line, char c);
t_env	*ft_lstlast(t_env *env);
void	ft_lstadd_back(t_env **env, t_env *new);
char	*ft_join(char *s1, char *s2);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(char *s, int c);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
char	*ft_strndup(const char *s1, size_t n);
void	*ft_memset(void *s, int c, size_t n);
char	*ft_strpbrk(const char *s, const char *accept);
void	ft_bzero(void *s, size_t n);
void	free_envp(char **envp);
int		check_pipe(char **token);
void	ft_errno(char **args, char **envp);
void	*ft_memmove(void *dst, const void *src, size_t n);

#endif