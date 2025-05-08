/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:35 by yagame            #+#    #+#             */
/*   Updated: 2025/05/07 00:57:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_list **env)
{
	t_list	*tmp;

	tmp = *env;
	if (!*env || !env)
	{
		g_exit_status = 1;
		return (1);
	}
	while (tmp)
	{
		if (tmp->key && tmp->key[0] == '_' && tmp->value)
		{
			free(tmp->value);
			tmp->value = ft_strdup("/usr/bin/env");
		}
		if (tmp->key && tmp->value && tmp->value[0] != '\0')
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (1);
}
