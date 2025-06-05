/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:19:52 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 13:35:13 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_copy_list(t_list *env)
{
	t_list	*new_list;
	t_list	*current;
	char	*key_cp;
	char	*value_cp;

	current = env;
	new_list = NULL;
	if (!env)
		return (NULL);
	while (current)
	{
		if (current->key)
			key_cp = ft_strdup(current->key);
		else
			key_cp = ft_strdup("");
		if (current->value)
			value_cp = ft_strdup(current->value);
		else
			value_cp = NULL;
		ft_lstadd_back(&new_list, ft_lstnew(key_cp, value_cp));
		current = current->next;
	}
	return (new_list);
}

void	alloc_key_value(char *cmd, char **key, char **value, char *equals_pos)
{
	*key = ft_substr(cmd, 0, equals_pos - cmd);
	*value = ft_substr(cmd, equals_pos - cmd + 1, ft_strlen(cmd)
		- (equals_pos - cmd));
}
