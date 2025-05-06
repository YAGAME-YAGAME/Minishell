/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 20:32:30 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:07:41 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Checks if the character after a dollar sign is a special expansion character.
 * @param value The string to check.
 * @param dollar_pos The position of the dollar sign in the string.
 * @return true if the next character is '$', '?', or a digit; false otherwise.
 */
bool	ft_condition_inchar(char *value, ssize_t dollar_pos)
{
	return (value[dollar_pos + 1] == '$'
		|| value[dollar_pos + 1] == '?'
		|| ft_isdigit(value[dollar_pos + 1]));
}

/**
 * @brief Expands a variable found after a dollar sign in a string.
 * @param value The string containing the variable.
 * @param minienv The environment variable list.
 * @param dollar_pos The position of the dollar sign in the string.
 * @return The expanded variable value as a new string.
 */
char	*ft_expand_inchar(char *value, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr(value, dollar_pos + 1,
			ft_get_varlen(value + dollar_pos + 1));
	expanded = ft_getvar(var, minienv);
	free(var);
	return (expanded);
}

/**
 * @brief Returns the substring after the expanded variable in the string.
 * @param value The string containing the variable.
 * @param dollar_pos The position of the dollar sign in the string.
 * @return The substring after the variable.
 */
char	*ft_temp2_inchar(char *value, ssize_t dollar_pos)
{
	return (ft_substr(value,
			dollar_pos + ft_get_varlen(value + dollar_pos + 1) + 1,
			ft_strlen(value)));
}

/**
 * @brief Expands the exit status variable ($?) in a string.
 * @param value Pointer to the string to expand. The string is updated in place.
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
	temp2 = ft_substr(*value, dollar_pos + 2,
			ft_strlen(*value));
	*value = ft_strjoin_free(temp, temp2);
	free(temp2);
}
