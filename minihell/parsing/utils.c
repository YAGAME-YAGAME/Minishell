/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:10:47 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a character is a whitespace character.
 * Tests for standard whitespace characters including space, tab, newline,
 * carriage return, form feed, and vertical tab.
 *
 * @param c: Character to test
 * @return: Non-zero if character is whitespace, 0 otherwise
 */
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

/*
 * Checks if a character is a shell special operator character.
 * Identifies characters that have special meaning in shell syntax:
 * pipe (|), input redirection (<), and output redirection (>).
 *
 * @param c: Character to test
 * @return: Non-zero if character is a special operator, 0 otherwise
 */
int	ft_isspecial(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
 * Checks if the current operator should be treated as a double operator.
 * Determines if redirection operators should be parsed as append (>>) or
 * heredoc (<<) by checking if the same operator character appears twice
 * consecutively in the input stream.
 *
 * @param lexer: Lexer structure containing input and position information
 * @param op: Current operator character(s) being processed
 * @return: true if operator should be treated as doubled, false otherwise
 */
bool	ft_is_duplicated(t_lexer *lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '>') || (op[0] == '<'
			&& lexer->pos + 1 < lexer->len && lexer->input[lexer->pos
				+ 1] == '<'));
}

/*
 * Finds the position of the first expandable dollar sign in a string.
 * Searches for '$' characters that are followed by valid variable name
 * characters (alphanumeric, underscore), special variables ('$', '?'),
 * indicating they should be expanded as environment variables.
 *
 * @param str: String to search for expandable dollar signs
 * @return: Position of first expandable '$', -1 if none found
 */
ssize_t	ft_dollar_pos(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '$' || str[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

/*
 * Calculates the length of a variable name for expansion.
 * Determines how many characters form a valid variable name starting
 * from the current position. Handles special case of '?' variable
 * and standard variable names (alphanumeric and underscore characters).
 *
 * @param str: String containing the variable name
 * @return: Length of the variable name in characters
 */
int	ft_get_varlen(char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "?") == 0)
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}
