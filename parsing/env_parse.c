/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:00:14 by codespace         #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Parses an environment variable string into separate key and value components.
 * Splits an environment variable string at the '=' character to extract
 * the variable name (key) and its value. Allocates memory for both
 * components and handles error cases where no '=' is found.
 *
 * @param env_var: Environment variable string in "KEY=value" format
 * @param key: Pointer to store the extracted variable name
 * @param value: Pointer to store the extracted variable value
 * @return: 1 on successful parsing, 0 if no '=' found or allocation fails
 * Side effects: Allocates memory for key and value, frees on error
 */
int	ft_parse_env_var(char *env_var, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(env_var, '=');
	if (!equal_sign)
		return (0);
	key_len = equal_sign - env_var;
	*key = ft_substr(env_var, 0, key_len);
	*value = ft_substr(env_var, key_len + 1, ft_strlen(env_var) - key_len - 1);
	if (!(*key) || !(*value))
	{
		free(*key);
		free(*value);
		return (0);
	}
	return (1);
}
