/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:34:26 by yagame            #+#    #+#             */
/*   Updated: 2025/05/18 19:19:46 by otzarwal         ###   ########.fr       */
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
