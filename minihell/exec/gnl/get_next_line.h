/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:12:34 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/20 04:38:32 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
int		my_strchr(char *s, char c);
char	*my_strdup(const char *s1);
size_t	my_strlen(const char *s);
char	*my_strjoin(char *s1, char *s2);
char	*my_substr(char *s, unsigned int start, size_t len);

#endif
