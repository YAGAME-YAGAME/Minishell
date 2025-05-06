/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:08:28 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:07:41 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Initializes a lexer structure for the given input string.
 * @param input The input string to tokenize.
 * @return Pointer to the initialized lexer structure.
 */
t_lexer	*ft_lexer_init(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

/**
 * @brief Tokenizes the input string into a linked list of tokens.
 * @param input The input string to tokenize.
 * @return Pointer to the head of the token list, or NULL on error.
 */
t_token	*ft_strtok(char *input)
{
	t_lexer	*lexer;
	t_token	*token;
	t_token	*token_list;

	if (input == NULL)
		return (NULL);
	lexer = ft_lexer_init(input);
	token_list = NULL;
	token = ft_get_next_token(lexer);
	while (token)
	{
		if (!token)
			break ;
		if (token->value)
			ft_tokadd_back(&token_list, ft_newtok(token));
		ft_free_token(token);
		token = ft_get_next_token(lexer);
	}
	free(lexer);
	return (token_list);
}
