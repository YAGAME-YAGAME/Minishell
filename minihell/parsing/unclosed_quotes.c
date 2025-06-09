/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 22:05:33 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/09 14:48:32 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_skip_chars(char *input, int i)
{
	while (input[i] && input[i] != '"' && input[i] != '\'')
		i++;
	return (i);
}

bool	ft_check_quotes(char *input)
{
	int		i;
	char	q;

	i = 0;
	while (input[i])
	{
		i = ft_skip_chars(input, i);
		if (input[i] == '"' || input[i] == '\'')
		{
			q = input[i];
			i++;
			while (input[i] && input[i] != q)
				i++;
			if (!input[i])
				return (printf(RED UNCLOSED RESET), g_exit_status = 258, false);
			if (input[i])
				i++;
		}
		else if (input[i])
			i++;
	}
	return (true);
}
