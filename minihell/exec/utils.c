/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 22:58:34 by codespace         #+#    #+#             */
/*   Updated: 2025/05/22 00:47:29 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_list(t_cmdarg *node)
{
	int	i;

	i = 0;
	if (!node)
		return (0);
	while (node)
	{
		i++;
		node = node->next;
	}
	return (i);
}

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
		if (ft_strcmp(env->key, "SHLVL") == 0)
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
