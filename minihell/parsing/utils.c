/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:10:47 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/07 20:11:38 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\f' || c == '\v');
}

int	ft_isspecial(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')'
		|| c == '<' || c == '>' || c == ';');
}

bool	ft_is_duplicated(t_lexer *lexer, char op[3])
{
	return ((op[0] == '>' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '>')
		|| (op[0] == '<' && lexer->pos + 1 < lexer->len
			&& lexer->input[lexer->pos + 1] == '<'));
}

void	free_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}
