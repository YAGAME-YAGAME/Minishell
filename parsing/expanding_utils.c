/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:48:03 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a dollar sign should trigger special expansion.
 * Determines if the character after a dollar sign requires special
 * handling: exit status ($?), process ID ($$), or digit (positional
 * parameters). Used to identify special variable expansions.
 *
 * @param token: Token containing the dollar sign
 * @param dollar_pos: Position of the dollar sign in the token value
 * @return: true if special expansion needed, false for normal variable
 */
bool	ft_condition(t_token **token, ssize_t dollar_pos)
{
	return ((*token)->value[dollar_pos + 1] == '$' || (*token)->value[dollar_pos
			+ 1] == '?' || ft_isdigit((*token)->value[dollar_pos + 1]));
}

/*
 * Expands a variable name to its value from the environment.
 * Extracts the variable name following a dollar sign and retrieves
 * its value from the environment list. Handles variable name length
 * calculation and memory management for the expansion process.
 *
 * @param token: Token containing the variable reference
 * @param minienv: Environment variables list for lookup
 * @param dollar_pos: Position of the dollar sign in the token
 * @return: Expanded variable value, empty string if not found
 * Side effects: Allocates memory for variable name and expanded value
 */
char	*ft_expand(t_token **token, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr((*token)->value, dollar_pos + 1,
			ft_get_varlen((*token)->value + dollar_pos + 1));
	expanded = ft_getvar(var, minienv);
	free(var);
	return (expanded);
}

/*
 * Calculates the remaining portion of string after variable expansion.
 * Determines the substring that comes after a variable reference
 * in the token value. Used to reconstruct the token after variable
 * expansion by preserving the trailing content.
 *
 * @param token: Token being processed for variable expansion
 * @param dollar_pos: Position of the dollar sign in the token
 * @return: Substring representing the remaining content after variable
 * Side effects: Allocates memory for the substring
 */
char	*ft_temp2(t_token **token, ssize_t dollar_pos)
{
	return (ft_substr((*token)->value, dollar_pos
			+ ft_get_varlen((*token)->value + dollar_pos + 1) + 1,
			ft_strlen((*token)->value)));
}

/*
 * Joins two strings and frees the first string.
 * Concatenates two strings into a new string and automatically
 * frees the first string parameter. This is commonly used during
 * variable expansion to build up the final expanded string.
 *
 * @param s1: First string to join (will be freed)
 * @param s2: Second string to append
 * @return: New concatenated string, NULL on failure
 * Side effects: Frees s1, allocates memory for result
 */
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
