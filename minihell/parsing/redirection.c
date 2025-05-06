/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:18:40 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:13:47 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Checks if a token is a redirection operator (input, output, heredoc, append).
 * @param token The token to check.
 * @return true if the token is a redirection, false otherwise.
 */
bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
		|| token->type == HEREDOC || token->type == APPEND);
}

/**
 * @brief Creates a new redirection node from a token.
 * @param token The token representing the redirection.
 * @param expand Whether to expand variables in the file name.
 * @return Pointer to the new redirection node, or NULL on error.
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

/**
 * @brief Adds a redirection node to the end of a redirection list.
 * @param redi Pointer to the head of the redirection list.
 * @param new The redirection node to add.
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

/**
 * @brief Prints the redirection list for debugging purposes.
 * @param redi The head of the redirection list.
 */
void	ft_printredi(t_redi_list *redi)
{
	t_redi_list	*tmp;

	tmp = redi;
	while (tmp)
	{
		printf("type: {%s}, file: [%s]:%d\n", ft_printtype(tmp->type),
			tmp->file, tmp->variable);
		tmp = tmp->next;
	}
}
