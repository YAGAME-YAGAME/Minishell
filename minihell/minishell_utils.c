/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 04:34:22 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Checks for redirection syntax errors in the token stream.
 * Identifies invalid redirection patterns where a redirection operator
 * is followed by another redirection operator or appears at the end of input.
 *
 * @param current: Current token being examined
 * @return: true if redirection syntax error is found, false otherwise
 */
bool	ft_redierrors(t_token *current)
{
	return (ft_isredi(current) && (current->next == NULL
			|| ft_isredi(current->next)));
}

/*
 * Checks for pipe syntax errors in the token stream.
 * Identifies invalid pipe usage including pipes
 * at the beginning or end of input,
 * consecutive pipes, or pipes following redirection operators.
 *
 * @param current: Current token being examined
 * @return: true if pipe syntax error is found, false otherwise
 */
bool	ft_pipeerrors(t_token *current)
{
	return (current->type == PIPE && ((current->next == NULL
				|| current->next->type == PIPE || current->prev == NULL
				|| ft_isredi(current->prev))));
}

/*
 * Performs cleanup by freeing memory allocated for token and command lists.
 * This utility function ensures proper memory deallocation when the shell
 * processing is complete or encounters an error that requires early termination.
 *
 * @param token_list: Linked list of tokens to be freed
 * @param cmdarg_list: Linked list of command argument structures to be freed
 * Side effects: Deallocates memory for both data structures
 */
void	ft_cleaner(t_token *token_list, t_cmdarg *cmdarg_list)
{
	ft_free_tokenlist(token_list);
	ft_free_cmdlist(cmdarg_list);
}
