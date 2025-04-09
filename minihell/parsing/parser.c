/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/09 20:20:59 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
			|| token->type == HEREDOC || token->type == APPEND);
}

void	ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*tmp;
	tmp = (*node)->strags;//point on the old cmdargs
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);//join the new word
	free(tmp);//free the old
	// add space between the old and the next new word
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

t_cmdarg	*ft_init_node()
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

t_cmdarg	*get_next_node(t_token *token_list)
{
	t_cmdarg	*node;

	node = ft_init_node();
	while (token_list->current && token_list->current->type != PIPE)//inner loop on all tokens till find the pipe
	{
		if (token_list->current->type == WORD)
			ft_parse_word(&node, token_list);
		token_list->current = token_list->current->next;
	}
	printf("cmdargs: [%s]\n", node->strags);
	exit(0);
	return (NULL);
}
