/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 01:52:00 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/18 16:45:10 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*check_full_path(char *p)
{
	if (!p)
		return (NULL);
	if (*p == '/' || *p == '.')
	{
		if (access(p, X_OK) == 0)
			return (ft_strdup(p));
		else
			return (NULL);
	}
	return (NULL);
}

void	handle_exec_error(char *cmd_path, char *cmd_name)
{
	free(cmd_path);
	free(cmd_name);
	ft_cmd_error(NULL, "malloc failure\n", 1);
}
