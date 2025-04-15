/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:22 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/15 15:18:07 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdarg	*ft_newnode(t_cmdarg *node)
{
	t_cmdarg	*new;

	new = malloc(sizeof(t_cmdarg));
	if (!new)
		return (NULL);
	new->strags = node->strags;
	new->is_builtin = node->is_builtin;
	new->input = node->input;
	new->output = node->output;
	new->next = NULL;
	return (new);
}

void	ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new)
{
	t_cmdarg	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_cmdarg	*ft_init_node(void)
{
	t_cmdarg	*node;

	node = malloc(sizeof(t_cmdarg));
	if (!node)
		return (NULL);
	node->input = NULL;
	node->is_builtin = false;
	node->next = NULL;
	node->output = NULL;
	node->strags = NULL;
	return (node);
}

t_cmdarg	*ft_get_next_node(t_token *token_list)
{
	t_cmdarg	*node;

	node = ft_init_node();
	if (!token_list->current)
		return (NULL);
	if (token_list->current->type == PIPE)
		token_list->current = token_list->current->next;
	while (token_list->current && token_list->current->type != PIPE)
	{
		if (token_list->current->type == WORD)
			ft_parse_word(&node, token_list);
		else if (ft_isredi(token_list->current))
			ft_parse_redi(&node, token_list);
		else if (token_list->current->type == SINGLE_QUOTE)
			ft_parse_squote(&node, token_list);
		else if (token_list->current->type == DOUBLE_QUOTE)
			ft_parse_dquote(&node, token_list);
		token_list->current = token_list->current->next;
	}
	return (node);
}
