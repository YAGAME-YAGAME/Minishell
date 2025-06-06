/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:22 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/06 19:31:33 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Creates a deep copy of a command argument node.
 * Duplicates all fields including the command array with all its strings,
 * redirection lists, and metadata. Used for creating independent copies
 * of command structures.
 *
 * @param node: Source command argument node to copy
 * @return: Pointer to new command argument node, NULL on allocation failure
 * Side effects: Allocates memory for new node and duplicates all string content
 */
t_cmdarg	*ft_newnode(t_cmdarg *node)
{
	t_cmdarg	*new;
	int			i;

	new = malloc(sizeof(t_cmdarg));
	if (!new)
		return (NULL);
	new->cmdsize = node->cmdsize;
	new->cmd_capacity = node->cmd_capacity;
	new->cmd = malloc(sizeof(char *) * (node->cmd_capacity + 1));
	if (!new->cmd)
		return (free(new), NULL);
	i = -1;
	while (++i < node->cmdsize)
	{
		if (node->cmd[i])
			new->cmd[i] = ft_strdup(node->cmd[i]);
		else
			new->cmd[i] = NULL;
	}
	new->cmd[i] = NULL;
	new->is_builtin = node->is_builtin;
	new->redirections = node->redirections;
	new->input = node->input;
	new->output = node->output;
	new->next = NULL;
	return (new);
}

/*
 * Adds a command argument node to the end of a linked list.
 * Traverses to the end of the list and appends the new node.
 * Handles the case where the list is initially empty.
 *
 * @param lst: Pointer to the head of the command argument list
 * @param new: New command argument node to add
 * Side effects: Modifies the linked list structure
 */
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

/*
 * Initializes a new command argument node with default values.
 * Sets up a clean command argument structure with null pointers,
 * zero sizes, and default boolean values for a new command.
 *
 * @return: Pointer to initialized command argument node,
 * NULL on allocation failure
 * Side effects: Allocates memory for new node structure
 */
t_cmdarg	*ft_init_node(void)
{
	t_cmdarg	*node;

	node = malloc(sizeof(t_cmdarg));
	if (!node)
		return (NULL);
	node->input = NULL;
	node->redirections = NULL;
	node->is_builtin = false;
	node->next = NULL;
	node->output = NULL;
	node->cmd = NULL;
	node->cmdsize = 0;
	node->cmd_capacity = 0;
	return (node);
}

/*
 * Extracts and parses the next complete command from the token stream.
 * Creates a command argument node by processing all tokens until a pipe
 * or end of stream. Handles command words and redirection operators,
 * building the complete command structure with arguments and redirections.
 *
 * @param token_list: Token list with current position tracking
 * @return: Complete command argument node, NULL if no more commands or error
 * Side effects: Advances token list current pointer,
 * allocates memory for command structure
 */
t_cmdarg	*ft_get_next_node(t_token *token_list)
{
	t_cmdarg	*node;
	int			initial_capacity;

	if (!token_list || !token_list->current)
		return (NULL);
	node = ft_init_node();
	initial_capacity = ft_toksize(token_list);
	node->cmd = malloc(sizeof(char *) * (initial_capacity + 1));
	if (!node->cmd)
		return (free(node), NULL);
	node->cmd_capacity = initial_capacity;
	if (token_list->current->type == PIPE)
		token_list->current = token_list->current->next;
	if (!token_list->current)
		return (free(node->cmd), free(node), NULL);
	while (token_list->current && token_list->current->type != PIPE)
	{
		if (token_list->current && ft_is_cmd(token_list->current))
			ft_parse_word(&node, token_list);
		else if (token_list->current && ft_isredi(token_list->current))
			ft_parse_redi(&node, token_list);
		token_list->current = token_list->current->next;
	}
	node->cmd[node->cmdsize] = NULL;
	return (node);
}

/*
 * Resizes the command array to accommodate more arguments.
 * Increases the capacity of the command array by allocating a new larger array,
 * copying existing command pointers, and updating the node's capacity.
 * Safely handles cases where new capacity is not larger than current.
 *
 * @param node: Pointer to command argument node to resize
 * @param new_capacity: New capacity for the command array
 * @return: true on success, false on allocation failure
 * Side effects: Reallocates command array, updates capacity field
 */
bool	ft_resize_cmd_array(t_cmdarg **node, int new_capacity)
{
	char	**new_cmd;
	int		i;

	if (new_capacity <= (*node)->cmd_capacity)
		return (true);
	new_cmd = malloc(sizeof(char *) * (new_capacity + 1));
	if (!new_cmd)
		return (false);
	i = 0;
	while (i < (*node)->cmdsize)
	{
		new_cmd[i] = (*node)->cmd[i];
		i++;
	}
	while (i <= new_capacity)
	{
		new_cmd[i] = NULL;
		i++;
	}
	free((*node)->cmd);
	(*node)->cmd = new_cmd;
	(*node)->cmd_capacity = new_capacity;
	return (true);
}
