/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/06 19:05:44 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Ensures there is enough space in the command array for a new word.
 * Resizes the array if necessary to accommodate additional commands.
 *
 * @param node: Pointer to command argument node containing the command array
 * @return: 1 on success, 0 on failure
 * Side effects: May resize command array, sets global exit status on failure
 */
static int	ft_extend_cmd_for_word(t_cmdarg **node)
{
	if ((*node)->cmdsize >= (*node)->cmd_capacity)
	{
		if (!ft_resize_cmd_array(node, (*node)->cmd_capacity * 2 + 1))
		{
			perror("Failed to resize cmd array");
			g_exit_status = 1;
			return (0);
		}
	}
	return (1);
}

/*
 * Adds a word to the command array, or joins it to the previous word
 * depending on the addspace property of the previous token.
 * Handles token concatenation logic for building complete command arguments.
 *
 * @param node: Pointer to command argument node to modify
 * @param token_list: Token list containing spacing context information
 * @param val: String value to add or join to command array
 * Side effects: Modifies command array, allocates/frees memory
 * for string operations
 */
static void	ft_add_or_join_word(t_cmdarg **node, t_token *token_list, char *val)
{
	char	*tmp;

	if (token_list->current->prev != NULL
		&& token_list->current->prev->addspace == true)
		(*node)->cmd[(*node)->cmdsize++] = val;
	else
	{
		if ((*node)->cmdsize > 0)
		{
			tmp = (*node)->cmd[(*node)->cmdsize - 1];
			(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, val);
			free(tmp);
			free(val);
		}
		else
			(*node)->cmd[(*node)->cmdsize++] = val;
	}
}

/*
 * Parses a word token and adds it to the command array in the node.
 * Handles variable expansion and splitting if necessary, managing both
 * simple words and complex expansions that result in multiple arguments.
 *
 * @param node: Pointer to command argument node to modify
 * @param token_list: Token list containing current token being processed
 * @return: Always returns NULL after processing
 * Side effects: Modifies command array, handles memory allocation,
 * may set global exit status
 */
void	*ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*val;

	val = ft_strdup(token_list->current->value);
	if (val == NULL)
		return (perror("malloc failure"), g_exit_status = 1, NULL);
	if (token_list->current->variable == true
		&& token_list->current->type == WORD)
	{
		ft_handle_split_word(node, token_list, val);
	}
	else
	{
		if (!ft_extend_cmd_for_word(node))
		{
			free(val);
			return (NULL);
		}
		ft_add_or_join_word(node, token_list, val);
	}
	return (NULL);
}

/*
 * Parses redirection tokens and adds them to the appropriate
 * redirection list.
 * Handles both input redirections (< and <<)
 * and output redirections (> and >>).
 * Creates redirection list entries and advances the token pointer
 * to the filename.
 *
 * @param node: Pointer to command argument node to modify
 * @param token_list: Token list containing redirection tokens
 * Side effects: Modifies input/output redirection lists,
 * advances current token pointer
 */
void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD)
			ft_rediradd(&(*node)->redirections, ft_redinew(token_list->current, true));
		else
			ft_rediradd(&(*node)->redirections, ft_redinew(token_list->current,
					false));
	}
	else
		ft_rediradd(&(*node)->redirections, ft_redinew(token_list->current, true));
	token_list->current = token_list->current->next;
}
