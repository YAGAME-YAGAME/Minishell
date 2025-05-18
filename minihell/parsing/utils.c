/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:10:47 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/04 19:53:31 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

int	ft_isspecial(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	ft_is_duplicated(t_lexer *lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '>') || (op[0] == '<'
			&& lexer->pos + 1 < lexer->len && lexer->input[lexer->pos
			+ 1] == '<'));
}

ssize_t	ft_dollar_pos(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '$' || str[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_get_varlen(char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "?") == 0)
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}
