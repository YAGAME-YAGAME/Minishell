/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:55:24 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/05 15:55:33 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_builtins(char *input, t_list *minienv)
{
	if (ft_strncmp(input, "exit", 4) == 0)
	{
		ft_lstclear(&minienv, free);
		free(input);
		exit(0);
	}
	if (ft_strncmp(input, "env", 3) == 0)
	{
		ft_print_env(minienv);
		return ;
	}
}
