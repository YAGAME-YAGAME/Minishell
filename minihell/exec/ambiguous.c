/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:34:26 by yagame            #+#    #+#             */
/*   Updated: 2025/06/04 23:14:35 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_ambiguous(char *file)
{
	char	*tmp;

	if (!file || file[0] == '\0')
		return (true);
	tmp = file;
	while (*tmp)
	{
		if (*tmp == ' ')
			break ;
		tmp++;
	}
	if (*tmp == ' ')
		return (true);
	return (false);
}

void	finish_exec(void)
{
	int	status;

	setup_parent_waiting_signals();
	ft_wait_children(&status);
	restore_signals();
}
