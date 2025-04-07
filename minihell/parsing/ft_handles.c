/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/07 20:12:29 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_handle_quotes(t_lexer *lexer, char quote_char)
{
	int		start;
	char	*content;
	t_token	*token;

	lexer->pos++;
	start = lexer->pos;
	while (lexer->pos < lexer->len
		&& lexer->input[lexer->pos] != quote_char)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (printf(RED"unexpected EOF while looking \
for matching `\"'"RESET), NULL);
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
	token->quote_type = quote_char;
	return (token);
}

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
	if (!token || !(token->value = ft_strdup(op)))//FIXME:Assignment in control structure
		return (NULL);
	if (op_len == 2)
		token->type = (op[0] == '>') ? APPEND : HEREDOC;//FIXME:Ternaries are forbidden
	else if (op[0] == '|')
		token->type = PIPE;
	else
		token->type = (op[0] == '>') ? OUTPUT : INPUT;
	token->quote_type = '\0';
	lexer->pos += op_len;
	return (token);
}

t_token	*ft_handle_word(t_lexer *lexer)
{
	int		start;
	char	*value;
	t_token	*token;

	start = lexer->pos;
	while (lexer->pos < lexer->len && !ft_isspace(lexer->input[lexer->pos])
		&& !ft_isspecial(lexer->input[lexer->pos]))
		lexer->pos++;
	value = ft_substr(lexer->input, start, lexer->pos - start);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token || !value)
		return (NULL);
	token->type = WORD;
	token->value = value;
	token->quote_type = '\0';
	return (token);
}
