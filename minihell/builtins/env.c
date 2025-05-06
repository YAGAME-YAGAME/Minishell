/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:35 by yagame            #+#    #+#             */
/*   Updated: 2025/05/03 18:54:26 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    ft_env(t_list **env)
{
	t_list *tmp;

	tmp = *env;
	if (!*env || !env)
	{
		g_exit_status = 1;
		return (1);
	}
	while (tmp)
	{
		if(tmp->key && tmp->key[0] == '_' && tmp->value)
		{
			free(tmp->value);
			tmp->value = ft_strdup("/usr/bin/env");
		}
		// Only print if both key and value exist and value is not empty
		if (tmp->key && tmp->value && tmp->value[0] != '\0')
			printf("%s=%s\n", tmp->key, tmp->value);

		tmp = tmp->next;
	}
	return (1);
}
