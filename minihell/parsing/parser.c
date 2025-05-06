/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:15:04 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Parses a word token and adds it to the command array of the node.
 * @param node Pointer to the command argument node.
 * @param token_list The token list containing the current token.
 * @return NULL (used for error handling and consistency).
 */
void	*ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*val;
	char	*tmp;

	tmp = NULL;
	val = ft_strdup(token_list->current->value);
	if (val == NULL)
		return (perror("malloc failure"), g_exit_status = 1, NULL);
	if (token_list->current->prev
		&& token_list->current->prev->addSpace == true)
		(*node)->cmd[(*node)->cmdSize++] = val;
	else
	{
		if ((*node)->cmdSize > 0)
		{
			tmp = (*node)->cmd[(*node)->cmdSize - 1];
			(*node)->cmd[(*node)->cmdSize - 1] = ft_strjoin(tmp, val);
			free(tmp);
			free(val);
		}
		else
			(*node)->cmd[(*node)->cmdSize++] = val;
	}
	return (NULL);
}

/**
 * @brief Parses a redirection token and adds it to the input or output redirection list of the node.
 * @param node Pointer to the command argument node.
 * @param token_list The token list containing the current token.
 */
void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD)
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, true));
		else
			ft_rediradd(&(*node)->input,
				ft_redinew(token_list->current, false));
	}
	else
		ft_rediradd(&(*node)->output, ft_redinew(token_list->current, true));
	token_list->current = token_list->current->next;
}
