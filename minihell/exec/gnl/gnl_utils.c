/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:01:16 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/12 16:17:25 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*my_strdup(const char *s1)
{
	char	*buff;
	size_t	size;
	int		i;

	i = 0;
	if (!s1)
		return (NULL);
	size = my_strlen(s1);
	buff = malloc(size + 1);
	if (!buff)
		return (NULL);
	while (s1[i])
	{
		buff[i] = s1[i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

char	*my_strjoin(char *s1, char *s2)
{
	char	*new_str;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1)
		return (my_strdup(s2));
	if (!s2)
		return (my_strdup(s1));
	total_len = my_strlen(s1) + my_strlen(s2) + 1;
	new_str = (char *)malloc(total_len * sizeof(char));
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	i = -1;
	while (s1[++i])
		new_str[i] = s1[i];
	j = -1;
	while (s2[++j])
		new_str[i++] = s2[j];
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}

int	my_strchr(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

char	*my_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*buff;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= my_strlen(s))
	{
		len = 0;
		start = 0;
		return (NULL);
	}
	if (start + len > my_strlen(s))
		len = my_strlen(s) - start;
	buff = malloc(len + 1);
	if (!buff)
		return (NULL);
	while (s[start + i] && i < len)
	{
		buff[i] = s[start + i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

size_t	my_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
