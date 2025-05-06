/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:48:03 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:09:06 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Checks if the character after a dollar sign is a special expansion character in a token.
 * @param token The token pointer to check.
 * @param dollar_pos The position of the dollar sign in the token value.
 * @return true if the next character is '$', '?', or a digit; false otherwise.
 */
bool	ft_condition(t_token **token, ssize_t dollar_pos)
{
	return ((*token)->value[dollar_pos + 1] == '$'
		|| (*token)->value[dollar_pos + 1] == '?'
		|| ft_isdigit((*token)->value[dollar_pos + 1]));
}

/**
 * @brief Expands a variable found after a dollar sign in a token value.
 * @param token The token pointer containing the variable.
 * @param minienv The environment variable list.
 * @param dollar_pos The position of the dollar sign in the token value.
 * @return The expanded variable value as a new string.
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

/**
 * @brief Returns the substring after the expanded variable in the token value.
 * @param token The token pointer containing the variable.
 * @param dollar_pos The position of the dollar sign in the token value.
 * @return The substring after the variable.
 */
char	*ft_temp2(t_token **token, ssize_t dollar_pos)
{
	return (ft_substr((*token)->value,
			dollar_pos + ft_get_varlen((*token)->value + dollar_pos + 1) + 1,
			ft_strlen((*token)->value)));
}

/**
 * @brief Joins two strings and frees the first string.
 * @param s1 The first string (will be freed).
 * @param s2 The second string.
 * @return The joined string.
 */
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
