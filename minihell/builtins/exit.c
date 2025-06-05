/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:11:33 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 04:35:23 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Validates if a string contains only numeric characters.
 * Allows for leading '+' or '-' sign followed by digits.
 * Used to validate exit status arguments to ensure they are numeric.
 *
 * @param str: String to validate for numeric content
 * @return: 1 if string is numeric, 0 if contains non-numeric characters
 */
int	is_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
 * Implements the exit builtin command functionality.
 * Terminates the shell with specified exit status or current g_exit_status.
 * Handles argument validation, error reporting for invalid arguments,
 * and proper cleanup of environment resources before exiting.
 *
 * @param cmd: Array of command arguments
 * (cmd[0] is "exit", cmd[1] is optional status)
 * @param env: Environment variables linked list to cleanup
 * @return: Does not return (calls exit()), or returns error status on failure
 * Side effects: Terminates program, frees environment memory,
 * prints error messages
 */
int	ft_exit(char **cmd, t_list **env)
{
	int	status;

	if (cmd[1] && cmd[2])
	{
		g_exit_status = 1;
		return (write(2, "exit: too many arguments\n", 25), g_exit_status);
	}
	else if (cmd[1])
	{
		if (is_digit(cmd[1]) == 0)
		{
			g_exit_status = 255;
			return (write(2, EXIT_ERROR, 32), g_exit_status);
		}
		status = ft_atoi(cmd[1]);
		if (status < 0)
			status = 256 + status;
		if (status > 255)
			status = status % 256;
		g_exit_status = status;
	}
	else
		g_exit_status = 0;
	ft_lstclear(env, free);
	exit(g_exit_status);
}
