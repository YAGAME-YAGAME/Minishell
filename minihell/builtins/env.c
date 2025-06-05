/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:35 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 04:35:51 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Implements the env builtin command functionality.
 * Displays all environment variables in the format "KEY=VALUE".
 * Handles special case for the '_' variable by setting it to "/usr/bin/env"
 * when displaying. Provides a way to view the current shell environment.
 *
 * @param env: Pointer to environment variables linked list
 * @return: 1 on success or error (sets g_exit_status appropriately)
 * Side effects: Prints environment variables to stdout,
 * may modify '_' variable value
 */
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
		if (tmp->key && ft_strcmp(tmp->key, "_") == 0 && tmp->value)
		{
			free(tmp->value);
			tmp->value = ft_strdup("/usr/bin/env");
		}
		if (tmp->key && tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (1);
}
