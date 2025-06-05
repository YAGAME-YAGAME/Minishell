/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:43:18 by otzarwal          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Counts the number of words/tokens separated by a given character.
 * Special handling for braced variables (e.g., ${VAR}) to treat them
 * as single tokens. Skips over separator characters and counts distinct
 * word boundaries in the input string.
 *
 * @param s: Input string to analyze
 * @param p: Character used as separator
 * @return: Number of distinct words/tokens found
 */
int	count(char *s, char p)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && *s == p)
			s++;
		if (!s)
			break ;
		if (*s && *s == '{')
		{
			while (*s && *s != '}')
				s++;
		}
		while (*s && !(*s == p || *s == '{'))
			s++;
		while (*s && *s == p)
			s++;
		if (!s)
			break ;
		i++;
	}
	return (i);
}

/*
 * Skips over separators and extracts the next word boundaries.
 * Handles special case of braced variables by treating content within
 * braces as a single unit. Updates the string pointer, length, and
 * start position for the next token to be extracted.
 *
 * @param s: Pointer to current position in string (modified)
 * @param len: Pointer to store length of found token
 * @param p: Separator character to skip
 * @param start: Pointer to store start of found token
 * Side effects: Modifies s pointer position and sets len and start values
 */
void	skip(char **s, int *len, char p, char **start)
{
	char	*end;

	while (**s && **s == p)
		(*s)++;
	if (!*s)
		return ;
	*start = *s;
	if (**s && **s == '{')
	{
		*start = *s;
		while (**s && **s != '}')
			(*s)++;
		(*s)++;
		end = *s;
	}
	else
	{
		while (**s && !(**s == p))
			(*s)++;
		end = *s;
	}
	*len = end - *start;
}

/*
 * Copies characters from source to destination buffer.
 * Extracts a token of specified length from the start position
 * into the allocated buffer array at the given index. Null-terminates
 * the copied string for proper C string handling.
 *
 * @param len: Number of characters to copy
 * @param j: Index in the buffer array to store the token
 * @param start: Source position to copy from
 * @param buff: Destination buffer array
 * Side effects: Modifies buffer array at index j
 */
void	edit(int len, int j, char *start, char **buff)
{
	int	i;

	i = 0;
	while (i < len)
	{
		buff[j][i++] = *start++;
	}
	buff[j][i] = '\0';
}

/*
 * Splits a string into an array of tokens based on a separator character.
 * Main parsing function that allocates memory for token array, counts
 * words, and extracts each token while handling special braced variable
 * syntax. Returns a null-terminated array of string tokens.
 *
 * @param s: Input string to split
 * @param p: Character to use as separator
 * @return: Null-terminated array of string tokens, NULL on failure
 * Side effects: Allocates memory for token array and individual strings
 */
char	**parsing_split(char *s, char p)
{
	int		wc;
	int		len;
	int		j;
	char	**buff;
	char	*start;

	if (!s)
		return (NULL);
	wc = count(s, p);
	len = 0;
	buff = malloc((wc + 1) * sizeof(char *));
	if (!buff)
		return (NULL);
	j = 0;
	while (j < wc)
	{
		skip(&s, &len, p, &start);
		buff[j] = malloc(len + 1);
		if (!buff[j])
			return (free_all(buff, j - 1), NULL);
		edit(len, j, start, buff);
		j++;
	}
	return (buff[j] = NULL, buff);
}
