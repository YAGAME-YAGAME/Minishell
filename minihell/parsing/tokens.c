/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:13:37 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/09 20:17:31 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Creates a new token by copying data from an existing token.
 * Allocates memory for a new token structure and duplicates the type,
 * value, and spacing information from the source token. Initializes
 * pointers and variable flag for the new token.
 *
 * @param token: Source token to copy from
 * @return: Pointer to newly created token, NULL if allocation fails
 * Side effects: Allocates memory for new token and duplicates value string
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
	new_token->addspace = token->addspace;
	new_token->variable = false;
	return (new_token);
}

/*
 * Handles joining tokens when a dollar sign precedes a quoted string.
 * Special case handler for variable expansion where a token ending with '$'
 * is followed by a quoted token. Either removes the single '$' or joins
 * the prefix before '$' with the new token's value.
 *
 * @param token: Token ending with '$' to be processed
 * @param new_token: Following quoted token to be joined
 * @return: Modified new_token with joined content, original token is freed
 * Side effects: Frees original token, modifies new_token's value
 */
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

/*
 * Joins adjacent tokens into a single token when appropriate.
 * Handles token concatenation for cases where multiple tokens should be
 * treated as one word, including variable expansion and special dollar-quote
 * combinations. Manages type inheritance and value concatenation.
 *
 * @param token: First token to be joined
 * @param lexer: Pointer to lexer structure for getting next token
 * @param minienv: Environment variables for expansion
 * @param heredoc: Pointer to heredoc context flag
 * @return: Joined token with concatenated values, NULL on error
 * Side effects: Frees original tokens, may modify global exit status
 */
t_token	*ft_jointok(t_token *token, t_lexer **lexer, t_list *minienv,
		bool *heredoc)
{
	t_token	*new_token;
	char	*value;

	if (*heredoc == false)
		ft_expand_variables(&token, minienv);
	new_token = ft_get_next_token(*lexer, minienv, heredoc);
	if (!new_token && g_exit_status == 258)
		return (ft_free_token(token), NULL);
	if (*heredoc == false)
		ft_expand_variables(&new_token, minienv);
	if (ft_strlen(token->value) > 0
		&& token->value[ft_strlen(token->value) - 1] == '$'
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

/*
 * Extracts the next token from the input stream.
 * Main tokenization function that identifies and creates tokens based on
 * the current character: handles whitespace skipping, special operators,
 * quoted strings, and regular words. Manages token joining for compound tokens.
 *
 * @param lexer: Lexer structure containing input string and current position
 * @param minienv: Environment variables list for potential expansion
 * @param heredoc: Pointer to heredoc context flag for proper parsing
 * @return: Next token from input stream, NULL if end of input or error
 * Side effects: Advances lexer position, may modify heredoc flag
 */
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
