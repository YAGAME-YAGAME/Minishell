/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:11:33 by yagame            #+#    #+#             */
/*   Updated: 2025/05/07 17:45:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
