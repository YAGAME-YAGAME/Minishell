/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:12:42 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Adds a token to the end of a token list.
 * @param token_list Pointer to the head of the token list.
 * @param token The token to add.
 */
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

/**
 * @brief Creates a new token by duplicating the given token's data.
 * @param token The token to duplicate.
 * @return Pointer to the new token, or NULL on error.
 */
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

/**
 * @brief Joins the value of the given token with the next token from the lexer.
 * @param token The token whose value will be joined.
 * @param lexer Pointer to the lexer structure.
 * @return Pointer to the new joined token, or NULL on error.
 */
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

/**
 * @brief Retrieves the next token from the lexer, handling words, quotes, and operators.
 * @param lexer The lexer structure.
 * @return Pointer to the next token, or NULL if no more tokens.
 */
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
