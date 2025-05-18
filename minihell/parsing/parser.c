/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/09 13:40:45 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*val;
	char	*tmp;

	tmp = NULL;
	val = ft_strdup(token_list->current->value);
	if (val == NULL)
		return (perror("malloc failure"), g_exit_status = 1, NULL);
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
	return (NULL);
}

void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD)
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, true));
		else
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current,
					false));
	}
	else
		ft_rediradd(&(*node)->output, ft_redinew(token_list->current, true));
	token_list->current = token_list->current->next;
}
