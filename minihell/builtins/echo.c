/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:08:06 by yagame            #+#    #+#             */
/*   Updated: 2025/04/29 14:46:45 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int only_n(char *str)
{
	int i;

	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char    *ft_join_arg(char **cmd, int i)
{
	char *tmp;
	char *hold;

	hold = ft_strdup("");
	while (cmd[i])
	{
		tmp = ft_strjoin(hold, cmd[i]);
		free(hold);
		hold = tmp;
		if (cmd[i + 1])
		{
			tmp = ft_strjoin(hold, " ");
			free(hold);
			hold = tmp;
		}
		i++;
	}
	return (hold);
}

void	ft_print_echo(char *tmp, int n_flag)
{
	if (!n_flag)
		ft_putendl_fd(tmp, 1);
	else
		ft_putstr_fd(tmp, 1);
}


int	ft_echo(char **cmd, t_cmdarg *shell)
{
	int i;
	int n_flag;
	char *tmp;
	(void)shell;

	i = 1;
	n_flag = 0;
	tmp = NULL;

	while (cmd[i] && ft_strncmp(cmd[i], "-n", 2) == 0)
	{
		if(!only_n(cmd[i]))
			break;
		n_flag = 1;
		i++;
	}
	tmp = ft_join_arg(cmd, i);
	ft_print_echo(tmp, n_flag);
	free(tmp);
	return (0);
}
