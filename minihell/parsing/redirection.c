/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:18:40 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 19:07:32 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a token represents a redirection operator.
 * Tests whether the token type is any of the four redirection operators:
 * input (<), output (>), heredoc (<<), or append (>>).
 *
 * @param token: Token to test for redirection type
 * @return: true if token is a redirection operator, false otherwise
 */
bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
		|| token->type == HEREDOC || token->type == APPEND);
}

/*
 * Creates a new redirection list entry from a token.
 * Extracts redirection information from a redirection token and its
 * following filename token, creating a redirection structure with
 * type, filename, and expansion metadata.
 *
 * @param token: Redirection operator token (next token contains filename)
 * @param expand: Boolean indicating if filename should be expanded
 * @return: New redirection list entry, NULL on allocation failure
 * Side effects: Allocates memory for redirection structure
 */
t_redi_list	*ft_redinew(t_token *token, bool expand)
{
	t_redi_list	*new;

	new = malloc(sizeof(t_redi_list));
	if (!new)
		return (NULL);
	new->type = token->type;
	new->file = token->next->value;
	new->expand = expand;
	new->variable = token->next->variable;
	new->next = NULL;
	return (new);
}

/*
 * Adds a redirection entry to the end of a redirection linked list.
 * Appends the new redirection to the end of the existing list,
 * or sets it as the head if the list is empty.
 *
 * @param redi: Pointer to the head of the redirection linked list
 * @param new: New redirection entry to add
 * Side effects: Modifies the linked list structure
 */
void	ft_rediradd(t_redi_list **redi, t_redi_list *new)
{
	t_redi_list	*tmp;

	if (!*redi)
	{
		*redi = new;
		return ;
	}
	tmp = *redi;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
