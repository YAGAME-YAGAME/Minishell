/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 01:34:46 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/24 14:06:52 by yagame           ###   ########.fr       */
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

	if (!dup_key || !key || !value || !*value)
		return;
		
	tmp = dup_key->value;
	if (!tmp)
		dup_key->value = ft_strdup(*value);
	else
	{
		dup_key->value = ft_strjoin(tmp, *value);
		free(tmp);
	}
	free(*key);
	free(*value);
	*key = NULL;
	*value = NULL;
	free(*value);
	free(*key);
	*key = NULL;
	*value = NULL;
}

static char	*trim_spaces(char *str)
{
	char	*start;
	char	*end;
	char	*result;

	if (!str)
		return (NULL);
	if (!*str)
	{
		result = ft_strdup("");
		free(str);
		return (result);
	}
	start = str;
	while (*start && (*start == ' ' || *start == '\t'))
		start++;
	end = str + ft_strlen(str) - 1;
	while (end >= start && (*end == ' ' || *end == '\t'))
		end--;
	result = ft_substr(start, 0, end - start + 1);
	free(str);
	if (!result)
		return (ft_strdup(""));
	return (result);
}

int	ft_handle_plus(char *cmd, char **key, char **value)
{
	int		is_append;
	char	*plus_pos;
	char	*equals_pos;

	is_append = 0;
	plus_pos = ft_strchr(cmd, '+');
	equals_pos = ft_strchr(cmd, '=');
	if (plus_pos && plus_pos + 1 && *(plus_pos + 1) == '=')
	{
		is_append = 1;
		*key = ft_substr(cmd, 0, plus_pos - cmd);
		*value = ft_substr(cmd, (plus_pos - cmd) + 2, ft_strlen(cmd) - (plus_pos
					- cmd) - 2);
	}
	else if (equals_pos)
	{
		*key = ft_substr(cmd, 0, equals_pos - cmd);
		*value = ft_substr(cmd, equals_pos - cmd + 1, ft_strlen(cmd)
				- (equals_pos - cmd));
	}
	if (*value)
		*value = trim_spaces(*value);
	if (!*key)
	{
		if (*value)
			free(*value);
		return (-1);
	}
	return (is_append);
}
