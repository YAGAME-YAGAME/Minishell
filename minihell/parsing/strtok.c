/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:08:28 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 04:32:59 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Initializes a lexer structure for tokenizing input.
 * Creates and configures a lexer with the input string, setting the position
 * to the beginning and calculating the total length of the input.
 *
 * @param input: String to be tokenized
 * @return: Pointer to initialized lexer structure
 * Side effects: Allocates memory for lexer structure
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

/*
 * Tokenizes an input string into a linked list of tokens.
 * Main tokenization function that breaks down shell command input into
 * individual tokens (words, operators, quotes) while handling heredoc contexts.
 * Continues tokenizing until all input is processed or an error occurs.
 *
 * @param input: Raw command line string to tokenize
 * @param minienv: Environment variables list for potential variable
 * expansion during tokenization
 * @return: Linked list of tokens, NULL if input is NULL
 * Side effects: Allocates memory for tokens and lexer,
 * frees temporary structures
 */
t_token	*ft_strtok(char *input, t_list *minienv)
{
	t_lexer	*lexer;
	t_token	*token;
	t_token	*token_list;
	bool	heredoc;

	if (input == NULL)
		return (NULL);
	lexer = ft_lexer_init(input);
	token_list = NULL;
	heredoc = false;
	token = ft_get_next_token(lexer, minienv, &heredoc);
	while (token)
	{
		if (!token)
			break ;
		if (token->value)
			ft_tokadd_back(&token_list, ft_newtok(token));
		ft_free_token(token);
		token = ft_get_next_token(lexer, minienv, &heredoc);
	}
	free(lexer);
	return (token_list);
}
