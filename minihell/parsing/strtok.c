/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:08:28 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/07 21:06:42 by abenajib         ###   ########.fr       */
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
	return (new_token);
}

void	print_tokenlist(t_token *token_list)
{
	t_token	*tmp;

	tmp = token_list;
	while (tmp)
	{
		if (tmp->value)
			printf("[%s]: {%s}\n", tmp->value, printtype(tmp->type));
		else
			printf("[NULL]\n");
		tmp = tmp->next;
	}
}

void	ft_free_tokenlist(t_token *token_list)
{
	t_token	*tmp;

	while (token_list)
	{
		tmp = token_list;
		token_list = token_list->next;
		free(tmp->value);
		free(tmp);
	}
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
		if (token->value)
			ft_tokadd_back(&token_list, ft_newtok(token));
		free_token(token);
		token = get_next_token(lexer);
	}
	print_tokenlist(token_list);
	ft_free_tokenlist(token_list);
	return (token_list);
}
