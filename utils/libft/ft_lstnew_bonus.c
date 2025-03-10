/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:03:12 by abenajib          #+#    #+#             */
/*   Updated: 2025/03/05 13:16:58 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Minishell.h"

t_list	*ft_lstnew(void *content, t_token token)
{
	if (!content)
		return (NULL);
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node -> content = content;
	new_node -> type = token;
	new_node -> next = NULL;
	new_node -> prev = NULL;
	return (new_node);
}
