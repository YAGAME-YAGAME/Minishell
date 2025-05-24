/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:35:29 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/24 14:46:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
size_t				ft_strlen(const char *s);
void				*ft_memset(void *b, int c, size_t len);
int					ft_atoi(const char *str);
char				*ft_strdup(const char *s1);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strchr(const char *s, int c);
// Additional functions
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char *s1, char *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(const char *s, char *c);
size_t				countwords(const char *s, char *c);
char				**freeall(char **p, size_t x);
char				*ft_itoa(int n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

// Bonus part
typedef struct s_list
{
	char			*key;
	char			*value;
	int				check;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

t_list				*ft_lstnew(char *key, char *value);
int					ft_lstsize(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new_node);
void				ft_lstclear(t_list **lst, void (*del)(void *));

#endif // LIBFT_H
