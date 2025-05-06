/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:10:47 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:12:41 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Checks if a character is a whitespace character.
 * @param c The character to check.
 * @return Non-zero if the character is whitespace, 0 otherwise.
 */
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\f' || c == '\v');
}

/**
 * @brief Checks if a character is a shell special character (|, <, >).
 * @param c The character to check.
 * @return Non-zero if the character is special, 0 otherwise.
 */
int	ft_isspecial(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * @brief Checks if the current operator is duplicated (<< or >>) in the lexer.
 * @param lexer The lexer structure.
 * @param op The operator string.
 * @return true if the operator is duplicated, false otherwise.
 */
bool	ft_is_duplicated(t_lexer *lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '>')
		|| (op[0] == '<' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '<'));
}

/**
 * @brief Finds the position of the next dollar sign ($) in a string that starts a variable expansion.
 * @param str The string to search.
 * @return The index of the dollar sign, or -1 if not found.
 */
ssize_t	ft_dollar_pos(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0'
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '$' || str[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Gets the length of a variable name in a string.
 * @param str The string starting with the variable name.
 * @return The length of the variable name.
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
