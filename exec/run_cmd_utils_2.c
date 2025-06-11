/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 01:52:00 by otzarwal          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a path is a full path and verifies execution permissions.
 * Determines if the given path is an absolute path (starts with /) or
 * relative path (starts with .), then checks if the file exists and
 * has execute permissions. Returns a duplicate of the path if valid.
 *
 * @param p: Path string to check
 * @return: Duplicate of path if valid and executable, NULL otherwise
 * Side effects: Allocates memory for returned string if successful
 */
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

/*
 * Handles memory cleanup and error reporting for execution failures.
 * Frees allocated memory for command path and name, then reports
 * a malloc failure error. Used when memory allocation fails during
 * command execution setup.
 *
 * @param cmd_path: Command path string to free
 * @param cmd_name: Command name string to free
 * Side effects: Frees memory, prints error message, exits with status 1
 */
void	handle_exec_error(char *cmd_path, char *cmd_name)
{
	free(cmd_path);
	free(cmd_name);
	ft_cmd_error(NULL, "malloc failure\n", 1);
}
