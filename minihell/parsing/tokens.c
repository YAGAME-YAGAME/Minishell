/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/04 19:54:00 by abenajib         ###   ########.fr       */
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
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->addSpace = token->addSpace;
	new_token->variable = false;
	return (new_token);
}

t_token	*ft_jointok(t_token *token, t_lexer **lexer)
{
	t_token	*new_token;
	char	*value;

	new_token = ft_get_next_token(*lexer);
	if (!new_token)
		return (NULL);
	if (token->type != WORD)
		new_token->type = token->type;
	value = ft_strjoin(token->value, new_token->value);
	new_token->value = value;
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
	if (ft_isspecial(current_char))
		return (ft_handle_operator(lexer));
	else
	{
		if (current_char == '\'' || current_char == '"')
		{
			token = ft_handle_quotes(lexer, current_char);
			if (!token)
				return (NULL);
		}
		else
			token = ft_handle_word(lexer);
		if (lexer->input[lexer->pos] != ' ' && lexer->input[lexer->pos] != '\0')
			token = ft_jointok(token, &lexer);
		return (token);
	}
}
