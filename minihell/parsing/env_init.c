/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:21:14 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/03 20:17:20 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_envinit(char **env)
{
	t_list	*minienv;
	char	*key;
	char	*value;
	int		i;

	minienv = NULL;
	if (!env || !*env)
	{
		ft_set_env(&minienv);
		return (minienv);
	}
	i = 0;
	while (env && env[i])
	{
		key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		value = ft_substr(env[i], ft_strchr(env[i], '=') - env[i] + 1,
				ft_strlen(env[i]) - (ft_strchr(env[i], '=') - env[i]));
		if (!key || !value)
			return (NULL);
		ft_lstadd_back(&minienv, ft_lstnew(key, value));
		i++;
	}
	return (minienv);
}
