/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:10:23 by yagame            #+#    #+#             */
/*   Updated: 2025/04/20 20:45:43 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	    remove_env_node(t_list **env_list, t_list *node)
{
	if (!node)
		return (1);

	if (node->prev)
		node->prev->next = node->next;
	else
		*env_list = node->next; 
	if (node->next)
		node->next->prev = node->prev;
	ft_free_list(&node);
	return (1);
}

/**
 * Unset environment variables specified in cmd arguments
 * @param cmd Array of command and arguments
 * @param env The environment variables linked list
 * @return 1 on success
 */
int	ft_unset(char **cmd, t_list *env)
{
	t_list	*tmp;
	t_list  *env_head;
	int     i;

	if (!cmd || !env)
		return (1);
	
	env_head = env;
	i = 1; // Skip the 'unset' command name
	
	while (cmd[i])
	{
		tmp = env_head;
		while (tmp)
		{
			if (ft_strcmp(tmp->key, cmd[i]) == 0)
			{
				t_list *to_remove = tmp;
				tmp = tmp->next; // Move tmp before removing the node
				remove_env_node(&env_head, to_remove);
				break;
			}
			else
				tmp = tmp->next;
		}
		i++;
	}
	return (1);
}
