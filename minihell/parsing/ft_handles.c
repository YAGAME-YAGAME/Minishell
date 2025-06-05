/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 16:44:17 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Initializes the structural fields of a token to default values.
 * Sets up pointer fields and boolean flags for a newly created token,
 * preparing it for use in the token linked list structure.
 *
 * @param token: Token structure to initialize
 * Side effects: Modifies token's variable, current, next, and prev fields
 */
void	ft_init_token_fields(t_token *token)
{
	token->variable = false;
	token->current = NULL;
	token->next = NULL;
	token->prev = NULL;
}

/*
 * Handles parsing of quoted strings in shell input.
 * Extracts content between matching quotes (single or double), creating
 * appropriate token types. Handles unclosed quote errors and manages
 * spacing information for proper token concatenation.
 *
 * @param lexer: Lexer structure containing input and current position
 * @param quote_char: The quote character (''' or '"') to match
 * @return: Token containing quoted content, NULL on error or unclosed quote
 * Side effects: Advances lexer position, prints error on unclosed quotes,
 * sets exit status
 */
t_token	*ft_handle_quotes(t_lexer *lexer, char quote_char)
{
	int		start;
	char	*content;
	t_token	*token;

	start = ++lexer->pos;
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote_char)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (printf(RED UNCLOSED RESET), g_exit_status = 258, NULL);
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
		token->addspace = true;
	else
		token->addspace = false;
	ft_init_token_fields(token);
	return (token);
}

/*
 * Sets the appropriate token type for shell operators.
 * Determines the correct token type based on operator length and character:
 * handles single operators (|, <, >) and double operators (<<, >>).
 * Updates heredoc context when encountering heredoc operator.
 *
 * @param token: Token structure to set the type for
 * @param op_len: Length of the operator (1 or 2 characters)
 * @param op: Operator string being processed
 * @param heredoc: Pointer to heredoc context flag,
 * set to true for heredoc operator
 * Side effects: Modifies token type and may set heredoc flag
 */
void	ft_set_token_type(t_token *token, int op_len, char *op, bool *heredoc)
{
	if (op_len == 2)
	{
		if (op[0] == '>')
			token->type = APPEND;
		else
		{
			token->type = HEREDOC;
			*heredoc = true;
		}
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

/*
 * Handles parsing of shell operator characters.
 * Identifies and creates tokens for shell operators including pipes and
 * redirection operators. Supports both single and double character operators
 * (like >> for append and << for heredoc).
 *
 * @param lexer: Lexer structure containing input and current position
 * @param heredoc: Pointer to heredoc context flag for state management
 * @return: Token representing the parsed operator, NULL on allocation failure
 * Side effects: Advances lexer position, may modify heredoc flag
 */
t_token	*ft_handle_operator(t_lexer *lexer, bool *heredoc)
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
	token->addspace = false;
	ft_init_token_fields(token);
	ft_set_token_type(token, op_len, op, heredoc);
	lexer->pos += op_len;
	return (token);
}

/*
 * Handles parsing of regular word tokens in shell input.
 * Extracts sequences of characters that form command names, arguments,
 * or file names. Stops at whitespace, special operators, or quote characters.
 * Manages spacing information for proper token handling.
 *
 * @param lexer: Lexer structure containing input and current position
 * @return: Token containing the parsed word, NULL on allocation failure
 * Side effects: Advances lexer position to end of word
 */
t_token	*ft_handle_word(t_lexer *lexer)
{
	int		start;
	char	*value;
	t_token	*token;

	start = lexer->pos;
	while (lexer->pos < lexer->len && !ft_isspace(lexer->input[lexer->pos])
		&& !ft_isspecial(lexer->input[lexer->pos])
		&& lexer->input[lexer->pos] != '\'' && lexer->input[lexer->pos] != '"')
		lexer->pos++;
	value = ft_substr(lexer->input, start, lexer->pos - start);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token || !value)
		return (NULL);
	token->type = WORD;
	token->value = value;
	if (ft_isspace(lexer->input[lexer->pos]))
		token->addspace = true;
	else
		token->addspace = false;
	ft_init_token_fields(token);
	return (token);
}
