/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:08:28 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/07 20:18:31 by abenajib         ###   ########.fr       */
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

t_token	*get_next_token(t_lexer *lexer)
{
	char	current_char;

	while (lexer->pos < lexer->len
		&& ft_isspace(lexer->input[lexer->pos]))
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (NULL);
	current_char = lexer->input[lexer->pos];
	if (current_char == '\'' || current_char == '"')
		return (ft_handle_quotes(lexer, current_char));
	else if (ft_isspecial(current_char))
		return (ft_handle_operator(lexer));
	else
		return (ft_handle_word(lexer));
}

char	*printtype(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == INPUT)
		return ("INPUT");
	else if (type == OUTPUT)
		return ("OUTPUT");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("UNKNOWN");
}

t_token	*ft_strtok(char *input)
{
	t_lexer	*lexer;
	t_token	*token;
	t_token	*token_list;

	lexer = ft_lexer_init(input);
	token_list = NULL;
	token = get_next_token(lexer);
	while (token)
	{
		// ft_tokadd_back(&token_list, token);
		if (token->value)
		{
			printf("[%s]-", token->value);
			printf("[%s]\n", printtype(token->type));
		}
		free_token(token);
		token = get_next_token(lexer);
	}
	printf("[NULL]\n");
	return (token_list);
}
