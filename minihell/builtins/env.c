/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:35 by yagame            #+#    #+#             */
/*   Updated: 2025/04/19 18:06:15 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    ft_clear()
{
    printf("\033[H\033[J");
    return (1);
}

int    ft_env(t_list *env)
{
    t_list *tmp;

    tmp = env;
    while (tmp)
    {
        if (tmp->value)
            printf("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->next;
        
    }
    return (1);
}