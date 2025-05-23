/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:00:14 by codespace         #+#    #+#             */
/*   Updated: 2025/05/22 21:04:18 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Parse an environment variable string into key and value
 * @param env_var The environment variable string (e.g., "KEY=value")
 * @param key Pointer to store the extracted key
 * @param value Pointer to store the extracted value
 * @return 1 on success, 0 if the string doesn't contain '='
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
