/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:26:23 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 10:25:03 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include "get_next_line/get_next_line.h"

typedef struct s_lexing
{
	char	*value;
	char	*type;
	int		pos;
	int		layer;
	int		strength;
	int		env_variable;
	void	*next;
}				t_lexing;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
void		*ft_memset(void *s, int c, size_t size);
void		*ft_bzero(void *s, size_t size);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dest, const char *src, size_t n);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
long		ft_atoi(const char *str);
char		*ft_strdup(const char *s);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
int			free_final(char **final, int row);
int			quote_split(char const *str, int *i, int *start, int *quote_status);

t_lexing	*ft_lstnew(char *value, char *type, int strength);
void		ft_lstadd_front(t_lexing **lst, t_lexing *new);
int			ft_lstsize(t_lexing *lst);
t_lexing	*ft_lstlast(t_lexing *lst);
void		ft_lstadd_back(t_lexing **lst, t_lexing *new);
void		ft_lstdelone(t_lexing *lst);
void		ft_lstclear(t_lexing *lex_head);

#endif
