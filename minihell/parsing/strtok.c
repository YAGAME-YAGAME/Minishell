/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:08:28 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/19 20:33:10 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexer	*ft_lexer_init(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

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
