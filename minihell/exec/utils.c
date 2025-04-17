/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 22:58:34 by codespace         #+#    #+#             */
/*   Updated: 2025/04/15 20:39:07 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all(char **bf, int j)
{
	while (j >= 0)
	{
		free(bf[j]);
		bf[j] = NULL;
		j--;
	}
	free(bf);
}
char **get_env(t_list *env)
{
	char **envp;
	int i = 0;
	t_list *tmp = env;
	char *str;

	if(!env)
		return (NULL);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		str = ft_strjoin(env->key, env->value);
		envp[i] = ft_strdup(str);
		free(str);
		env = env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

/*
how to unset tmp:
tmp->next->prev = tmp->prev;
tmp->prev->next = tmp->next;
free(tmp);
*/
