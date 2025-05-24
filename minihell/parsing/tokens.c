/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/24 12:07:09 by codespace        ###   ########.fr       */
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
	new_token->addspace = token->addspace;
	new_token->variable = false;
	return (new_token);
}

t_token	*ft_dollar_joining(t_token *token, t_token *new_token)
{
	char	*prefix;
	char	*value;

	if (ft_strlen(token->value) == 1)
		return (ft_free_token(token), new_token);
	else
	{
		prefix = ft_substr(token->value, 0, ft_strlen(token->value) - 1);
		value = ft_strjoin(prefix, new_token->value);
		free(prefix);
		ft_free_token(token);
		free(new_token->value);
		new_token->value = value;
		return (new_token);
	}
}

t_token	*ft_jointok(t_token *token, t_lexer **lexer, t_list *minienv, bool *heredoc)
{
	t_token	*new_token;
	char	*value;

	if (*heredoc == false)
		ft_expand_variables(&token, minienv);
	new_token = ft_get_next_token(*lexer, minienv, heredoc);
	if (!new_token)
		return (NULL);
	if (*heredoc == false)
		ft_expand_variables(&new_token, minienv);

	if (ft_strlen(token->value) > 0 && token->value[ft_strlen(token->value) - 1] == '$'
		&& (new_token->type == DOUBLE_QUOTE || new_token->type == SINGLE_QUOTE))
	{
		return (ft_dollar_joining(token, new_token));
	}

	if (token->type != WORD)
		new_token->type = token->type;
	value = ft_strjoin(token->value, new_token->value);
	ft_free_token(token);
	free(new_token->value);
	new_token->value = value;
	return (new_token);
}

bool	ft_tojoin(t_lexer *lexer)
{
	return (lexer->input[lexer->pos] != ' ' && lexer->input[lexer->pos] != '\0'
		&& !ft_isspecial(lexer->input[lexer->pos]));
}

t_token	*ft_get_next_token(t_lexer *lexer, t_list *minienv, bool *heredoc)
{
	char	current_char;
	t_token	*token;

	while (lexer->pos < lexer->len && ft_isspace(lexer->input[lexer->pos]))
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (NULL);
	current_char = lexer->input[lexer->pos];
	if (ft_isspecial(current_char))
		return (ft_handle_operator(lexer, heredoc));
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
		if (ft_tojoin(lexer))
			token = ft_jointok(token, &lexer, minienv, heredoc);
		return (token);
	}
}
