/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:22 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/04 19:54:22 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdarg	*ft_newnode(t_cmdarg *node)
{
	t_cmdarg	*new;
	int			i;

	new = malloc(sizeof(t_cmdarg));
	if (!new)
		return (NULL);
	new->cmdSize = node->cmdSize;
	new->cmd = malloc(sizeof(char *) * (node->cmdSize + 1));
	if (!new->cmd)
		return (free(new), NULL);
	i = 0;
	while (i < node->cmdSize)
	{
		if (node->cmd[i])
			new->cmd[i] = ft_strdup(node->cmd[i]);
		else
			new->cmd[i] = NULL;
		i++;
	}
	new->cmd[i] = NULL;
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
	node->cmd = NULL;
	node->cmdSize = 0;
	return (node);
}

bool	ft_is_cmd(t_token *current)
{
	return (current->type == WORD || current->type == DOUBLE_QUOTE
		|| current->type == SINGLE_QUOTE);
}

t_cmdarg	*ft_get_next_node(t_token *token_list)
{
	t_cmdarg	*node;

	if (!token_list || !token_list->current)
		return (NULL);
	node = ft_init_node();
	node->cmd = malloc(sizeof(char *) * (ft_toksize(token_list) + 1));
	if (!node->cmd)
		return (free(node), NULL);
	if (token_list->current->type == PIPE)
		token_list->current = token_list->current->next;
	if (!token_list->current)
		return (free(node->cmd), free(node), NULL);
	while (token_list->current && token_list->current->type != PIPE)
	{
		if (ft_is_cmd(token_list->current))
			ft_parse_word(&node, token_list);
		else if (ft_isredi(token_list->current))
			ft_parse_redi(&node, token_list);
		token_list->current = token_list->current->next;
	}
	node->cmd[node->cmdSize] = NULL;
	return (node);
}
