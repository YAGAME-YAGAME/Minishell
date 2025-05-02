/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/02 20:36:26 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*val;
	char	*tmp;

	val = ft_strdup(token_list->current->value);
	if ((*node)->cmdSize != 0)
	{
		token_list->current = token_list->current->next;
		while (token_list->current && isCmd(token_list->current))
		{
			if (token_list->current->addSpace == true)
			{
				(*node)->cmd[(*node)->cmdSize++] = ft_strjoin(val, token_list->current->value);
				free(val);
				return ;
			}
			else
			{
				tmp = val;
				val = ft_strjoin(tmp, token_list->current->value);
				free(tmp);
			}
			token_list->current = token_list->current->next;
		}
	}
	(*node)->cmd[(*node)->cmdSize++] = val;
	// exit(0);
}

// void	ft_parse_squote(t_cmdarg **node, t_token *token_list)
// {
// 	char	*tmp;

// 	// tmp = (*node)->strags;
// 	// (*node)->strags = ft_strjoin((*node)->strags, "'");
// 	// free(tmp);
// 	tmp = (*node)->strags;
// 	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
// 	free(tmp);
// 	// tmp = (*node)->strags;
// 	// (*node)->strags = ft_strjoin((*node)->strags, "'");
// 	// free(tmp);
// 	if (token_list->current->addSpace == true)
// 	{
// 		tmp = (*node)->strags;
// 		(*node)->strags = ft_strjoin((*node)->strags, " ");
// 		free(tmp);
// 	}
// }

// void	ft_parse_dquote(t_cmdarg **node, t_token *token_list)
// {
// 	char	*tmp;

// 	// tmp = (*node)->strags;
// 	// (*node)->strags = ft_strjoin((*node)->strags, "\"");
// 	// free(tmp);
// 	tmp = (*node)->strags;
// 	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
// 	free(tmp);
// 	// tmp = (*node)->strags;
// 	// (*node)->strags = ft_strjoin((*node)->strags, "\"");
// 	// free(tmp);
// 	if (token_list->current->addSpace == true)
// 	{
// 		tmp = (*node)->strags;
// 		(*node)->strags = ft_strjoin((*node)->strags, " ");
// 		free(tmp);
// 	}
// }

void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD)
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, true));
		else
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, false));
	}
	else
		ft_rediradd(&(*node)->output, ft_redinew(token_list->current, true));
	token_list->current = token_list->current->next;
}
