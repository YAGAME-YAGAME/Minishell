/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 22:58:34 by codespace         #+#    #+#             */
/*   Updated: 2025/05/07 17:20:15 by codespace        ###   ########.fr       */
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

void	ft_alloc(char **envp, int *i, char *key_equals, t_list *env)
{
	char	*str;

	while (env)
	{
		if (strcmp(env->key, "SHLVL") == 0)
			env->value = ft_itoa(ft_atoi(env->value) + 1);
		key_equals = ft_strjoin(env->key, "=");
		str = ft_strjoin(key_equals, env->value);
		free(key_equals);
		envp[*i] = ft_strdup(str);
		free(str);
		env = env->next;
		(*i)++;
	}
}

char	**get_env(t_list *env)
{
	char	**envp;
	int		i;
	t_list	*tmp;
	char	*key_equals;

	i = 0;
	key_equals = NULL;
	tmp = env;
	if (!env)
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
	ft_alloc(envp, &i, key_equals, env);
	envp[i] = NULL;
	return (envp);
}
