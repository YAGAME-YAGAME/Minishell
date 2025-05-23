/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 01:34:46 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/23 04:24:32 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_helper(char **key, char **value, char *cmd)
{
	*key = ft_strdup(cmd);
	*value = NULL;
}

void	ft_handle_append(t_list *dup_key, char **key, char **value)
{
	char	*tmp;

	tmp = dup_key->value;
	dup_key->value = ft_strjoin(dup_key->value, *value);
	free(tmp);
	free(*value);
	free(*key);
	*key = NULL;
	*value = NULL;
}

int	ft_handle_plus(char *cmd, char **key, char **value)
{
	int		is_append;
	char	*plus_pos;

	is_append = 0;
	plus_pos = ft_strchr(cmd, '+');
	if (plus_pos && plus_pos + 1 && *(plus_pos + 1) == '=')
	{
		is_append = 1;
		*key = ft_substr(cmd, 0, plus_pos - cmd);
		*value = ft_substr(cmd, (plus_pos - cmd) + 2, ft_strlen(cmd) - (plus_pos
					- cmd) - 2);
	}
	else
	{
		*key = ft_substr(cmd, 0, ft_strchr(cmd, '=') - cmd);
		*value = ft_substr(cmd, ft_strchr(cmd, '=') - cmd + 1, ft_strlen(cmd)
				- (ft_strchr(cmd, '=') - cmd));
	}
	if (!*key)
	{
		if (*value)
			free(*value);
		return (-1);
	}
	return (is_append);
}
