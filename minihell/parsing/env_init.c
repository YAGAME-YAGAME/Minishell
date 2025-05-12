/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:21:14 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/09 12:52:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_envinit(char **env)
{
	t_list	*minienv;
	t_list	*new_node;
	char	*key;
	char	*value;
	int		i;

	minienv = NULL;
	if (!env || !*env)
		return (ft_set_env(&minienv), minienv);
	i = 0;
	while (env && env[i])
	{
		if (!ft_parse_env_var(env[i], &key, &value))
		{
			i++;
			continue ;
		}
		new_node = ft_lstnew(key, value);
		if (!new_node)
			return (free(key), free(value), NULL);
		ft_lstadd_back(&minienv, new_node);
		i++;
	}
	return (minienv);
}
