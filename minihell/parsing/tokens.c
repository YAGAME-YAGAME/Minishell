/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:02:14 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/22 18:57:38 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_tokadd_back(t_token **token_list, t_token *token)
{
	t_token	*tmp;

	if (!token_list || !token)
		return ;
	if (*token_list == NULL)
	{
		*token_list = token;
		return ;
	}
	tmp = *token_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	token->next = NULL;
}

t_token	*ft_newtok(t_token *token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = token->type;
	new_token->value = ft_strdup(token->value);
	new_token->quote_type = token->quote_type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

t_token	*ft_get_next_token(t_lexer *lexer)
{
	char	current_char;
	t_token	*token;

	while (lexer->pos < lexer->len
		&& ft_isspace(lexer->input[lexer->pos]))
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (NULL);
	current_char = lexer->input[lexer->pos];
	if (current_char == '\'' || current_char == '"')
	{
		token = ft_handle_quotes(lexer, current_char);
		return (token);
	}
	else if (ft_isspecial(current_char))
		return (ft_handle_operator(lexer));
	else
		return (ft_handle_word(lexer));
}
