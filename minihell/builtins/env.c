/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:35 by yagame            #+#    #+#             */
/*   Updated: 2025/04/26 23:43:57 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    ft_clear()
{
    printf("\033[H\033[2J");
    return (1);
}

int    ft_env(t_list **env)
{
    t_list *tmp;

    tmp = *env;
	if (!tmp)
		return (write(2, "env: No such file or directory\n", 31), 1);
    while (tmp)
    {
        if(tmp->key[0] == '_')
        {
            free(tmp->value);
            tmp->value = ft_strdup("/usr/bin/env");
        }
        if (tmp->value[0] != '\0')
            printf("%s=%s\n", tmp->key, tmp->value);
        
        tmp = tmp->next;
    }
    return (1);
}