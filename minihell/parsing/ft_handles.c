/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:08:27 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Checks if a character is a shell operator (|, <, >).
 * @param c The character to check.
 * @return true if the character is an operator, false otherwise.
 */
bool	isoperator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

/**
 * @brief Handles quoted substrings in the lexer and creates a token for them.
 * @param lexer The lexer structure.
 * @param quote_char The quote character (' or ").
 * @return Pointer to the created token, or NULL on error.
 */
t_token	*ft_handle_quotes(t_lexer *lexer, char quote_char)
{
	int		start;
	char	*content;
	t_token	*token;

	lexer->pos++;
	start = lexer->pos;
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote_char)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (printf(UNCLOSED), NULL);
	content = ft_substr(lexer->input, start, lexer->pos - start);
	lexer->pos++;
	token = (t_token *)malloc(sizeof(t_token));
	if (!token || !content)
		return (NULL);
	if (quote_char == '\'')
		token->type = SINGLE_QUOTE;
	else
		token->type = DOUBLE_QUOTE;
	token->value = content;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addSpace = true;
	else
		token->addSpace = false;
	return (token);
}

/**
 * @brief Sets the type of a token based on the operator string.
 * @param token The token to set the type for.
 * @param op_len The length of the operator string.
 * @param op The operator string.
 */
void	ft_set_token_type(t_token *token, int op_len, char *op)
{
	if (op_len == 2)
	{
		if (op[0] == '>')
			token->type = APPEND;
		else
			token->type = HEREDOC;
	}
	else if (op[0] == '|')
		token->type = PIPE;
	else
	{
		if (op[0] == '>')
			token->type = OUTPUT;
		else
			token->type = INPUT;
	}
}

/**
 * @brief Handles operator tokens (|, <, >, <<, >>) in the lexer.
 * @param lexer The lexer structure.
 * @return Pointer to the created operator token, or NULL on error.
 */
t_token	*ft_handle_operator(t_lexer *lexer)
{
	char	op[3];
	int		op_len;
	t_token	*token;

	op[0] = lexer->input[lexer->pos];
	op[1] = '\0';
	op_len = 1;
	if (ft_is_duplicated(lexer, op))
	{
		op[1] = lexer->input[lexer->pos + 1];
		op[2] = '\0';
		op_len = 2;
	}
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(op);
	if (!token->value)
		return (free(token), NULL);
	ft_set_token_type(token, op_len, op);
	lexer->pos += op_len;
	return (token);
}

/**
 * @brief Handles word tokens in the lexer (non-operator, non-quoted, non-whitespace).
 * @param lexer The lexer structure.
 * @return Pointer to the created word token, or NULL on error.
 */
t_token	*ft_handle_word(t_lexer *lexer)
{
	int		start;
	char	*value;
	t_token	*token;

	start = lexer->pos;
	while (lexer->pos < lexer->len && !ft_isspace(lexer->input[lexer->pos])
		&& !ft_isspecial(lexer->input[lexer->pos])
		&& lexer->input[lexer->pos] != '\''
		&& lexer->input[lexer->pos] != '"')
		lexer->pos++;
	value = ft_substr(lexer->input, start, lexer->pos - start);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token || !value)
		return (NULL);
	token->type = WORD;
	token->value = value;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addSpace = true;
	else
		token->addSpace = false;
	return (token);
}
