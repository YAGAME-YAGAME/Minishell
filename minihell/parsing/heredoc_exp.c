/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 20:32:30 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a dollar sign should trigger special expansion in character strings.
 * Similar to ft_condition but operates on character strings rather than tokens.
 * Determines if the character after a dollar sign requires special handling
 * for exit status, process ID, or positional parameters.
 *
 * @param value: String containing the dollar sign
 * @param dollar_pos: Position of the dollar sign in the string
 * @return: true if special expansion needed, false for normal variable
 */
bool	ft_condition_inchar(char *value, ssize_t dollar_pos)
{
	return (value[dollar_pos + 1] == '$' || value[dollar_pos + 1] == '?'
		|| ft_isdigit(value[dollar_pos + 1]));
}

/*
 * Expands a variable name to its value from environment in character strings.
 * Extracts variable name following a dollar sign and retrieves its value
 * from the environment. Used for variable expansion in heredoc content
 * and other string contexts outside of token processing.
 *
 * @param value: String containing the variable reference
 * @param minienv: Environment variables list for lookup
 * @param dollar_pos: Position of the dollar sign in the string
 * @return: Expanded variable value, empty string if not found
 * Side effects: Allocates memory for variable name and expanded value
 */
char	*ft_expand_inchar(char *value, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr(value, dollar_pos + 1, ft_get_varlen(value + dollar_pos
				+ 1));
	expanded = ft_getvar(var, minienv);
	free(var);
	return (expanded);
}

/*
 * Calculates remaining portion of string after variable expansion.
 * Determines the substring that comes after a variable reference
 * in a character string. Used to reconstruct strings after variable
 * expansion by preserving trailing content.
 *
 * @param value: String being processed for variable expansion
 * @param dollar_pos: Position of the dollar sign in the string
 * @return: Substring representing content after the variable
 * Side effects: Allocates memory for the substring
 */
char	*ft_temp2_inchar(char *value, ssize_t dollar_pos)
{
	return (ft_substr(value, dollar_pos + ft_get_varlen(value + dollar_pos + 1)
			+ 1, ft_strlen(value)));
}

/*
 * Expands exit status or process ID variables in character strings.
 * Handles special variable expansion for $? (exit status) and $$
 * (process ID) in character strings. Replaces the variable with
 * the appropriate numeric value and reconstructs the string.
 *
 * @param value: Pointer to string containing special variable
 * Side effects: Modifies the string pointer, frees/allocates memory
 */
void	ft_expand_exit_status_inchar(char **value)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;

	dollar_pos = ft_dollar_pos(*value);
	if ((*value)[dollar_pos + 1] == '?')
		expanded = ft_itoa(g_exit_status);
	else
		expanded = ft_strdup("");
	temp = ft_strjoin_free(ft_substr(*value, 0, dollar_pos), expanded);
	temp2 = ft_substr(*value, dollar_pos + 2, ft_strlen(*value));
	*value = ft_strjoin_free(temp, temp2);
	free(temp2);
}
