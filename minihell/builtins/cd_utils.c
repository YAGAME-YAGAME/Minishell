/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 01:10:53 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/22 01:12:03 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_dp(char **c)
{
	int	i;

	i = 0;
	while (*c)
	{
		c++;
		i++;
	}
	return (i);
}

t_list	*ft_find_node(t_list *env, char *key)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
