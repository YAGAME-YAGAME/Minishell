/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/15 22:26:56 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*tmp;

	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

void	ft_parse_squote(t_cmdarg **node, t_token *token_list)
{
	char	*tmp;

	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "'");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "'");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

void	ft_parse_dquote(t_cmdarg **node, t_token *token_list)
{
	char	*tmp;

	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "\"");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "\"");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == SINGLE_QUOTE)
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, false));
		else
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, true));
	}
	else
		ft_rediradd(&(*node)->output, ft_redinew(token_list->current, true));
	token_list->current = token_list->current->next;
}
