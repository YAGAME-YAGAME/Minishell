/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:00:00 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 19:07:35 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Frees an entire linked list of command argument structures.
 * Iterates through the command list, freeing each node and its
 * associated resources including command arrays and redirection lists.
 *
 * @param cmdarg_list: Head of the command argument linked list to free
 * Side effects: Deallocates memory for entire command list structure
 */
void	ft_free_cmdlist(t_cmdarg *cmdarg_list)
{
	t_cmdarg	*tmp;

	while (cmdarg_list)
	{
		tmp = cmdarg_list;
		cmdarg_list = cmdarg_list->next;
		ft_free_node(tmp);
	}
}

/*
 * Frees an entire linked list of tokens.
 * Iterates through the token list, freeing each token's value string
 * and the token structure itself, ensuring complete memory cleanup.
 *
 * @param token_list: Head of the token linked list to free
 * Side effects: Deallocates memory for entire token list structure
 */
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

/*
 * Frees a single token structure and its value.
 * Safely deallocates memory for a single token, including its
 * value string. Handles null token pointers gracefully.
 *
 * @param token: Token structure to free, may be NULL
 * Side effects: Deallocates memory for token and its value string
 */
void	ft_free_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

/*
 * Frees a single command argument node and all its resources.
 * Deallocates the command array, all command strings, input and output
 * redirection lists, and the node structure itself. Handles null nodes safely.
 *
 * @param node: Command argument node to free, may be NULL
 * Side effects: Deallocates memory for node, command array,
 * and redirection lists
 */
void	ft_free_node(t_cmdarg *node)
{
	int	i;

	i = 0;
	if (node)
	{
		if (node->cmd)
		{
			while (i < node->cmdsize)
				free(node->cmd[i++]);
			free(node->cmd);
		}
		ft_free_redi_list(node->input);
		ft_free_redi_list(node->output);
		free(node);
	}
}

/*
 * Frees a linked list of redirection structures.
 * Iterates through the redirection list, freeing each redirection
 * node. Note: this function only frees the structure, not the
 * file or content strings which may be shared or managed elsewhere.
 *
 * @param redi: Head of the redirection linked list to free
 * Side effects: Deallocates memory for redirection list structures
 */
void	ft_free_redi_list(t_redi_list *redi)
{
	t_redi_list	*tmp;

	while (redi)
	{
		tmp = redi;
		redi = redi->next;
		free(tmp);
	}
}
