/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:49:38 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/19 13:43:46 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

size_t	countwords(const char *s, char *c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (ft_strchr(c, s[i]) == NULL
			&& (i == 0 || ft_strchr(c, s[i - 1]) != NULL))
		{
			if (s[i++] == '\'')
			{
				while (s[i] && s[i] != '\'')
					i++;
			}
			count++;
		}
		i++;
	}
	return (count);
}

char	**freeall(char **p, size_t x)
{
	while (x > 0)
		free (p[--x]);
	free (p);
	return (NULL);
}

void	ft_quote(const char *s, size_t *i, size_t *start)
{
	if (s[*i] == '\'')
	{
		(*i)++;
		(*start)++;
		while (s[*i] && s[*i] != '\'')
			(*i)++;
	}
	else if (s[*i] == '\"')
	{
		(*i)++;
		(*start)++;
		while (s[*i] && s[*i] != '\"')
			(*i)++;
	}
}

static char	**copywords(char **p, const char *s, char *c)
{
	size_t	i;
	size_t	start;
	size_t	x;

	i = 0;
	x = 0;
	while (s[i] && x < countwords (s, c))
	{
		while (ft_strchr(c, s[i]) != NULL)
			i++;
		start = i;
		ft_quote(s, &i, &start);
		while (s[i] && ft_strchr(c, s[i]) == NULL
			&& s[i] != '\'' && s[i] != '\"')
			i++;
		p[x++] = ft_substr(s, start, i - start);
		if (!p[x - 1])
			return (freeall(p, x));
		if (s[i] == '\'' || s[i] == '\"')
			i++;
	}
	p[x] = NULL;
	return (p);
}

char	**ft_split(const char *s, char *c)
{
	size_t	count;
	char	**p;

	if (!s)
		return (NULL);
	count = countwords(s, c);
	p = (char **)malloc(sizeof(char *) * (count + 1));
	if (!p)
		return (NULL);
	return (copywords(p, s, c));
}
